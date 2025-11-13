//---------------------------------------------------------------------------
// Hex file manager - WinAPI version
//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "../include/ccrc.h"
#include "../include/Hex.h"

//---------------------------------------------------------------------------

#define KB (1024)
#define MB (KB*KB)

#define BOOT_SECTOR_BEGIN 0x9FC00000
#define APPLICATION_START 0x9D000000
#define PA_TO_VFA(x)    (x-APPLICATION_START)
#define PA_TO_KVA0(x)   (x|0x80000000)

#define DATA_RECORD             0
#define END_OF_FILE_RECORD      1
#define EXT_SEG_ADRS_RECORD     2
#define EXT_LIN_ADRS_RECORD     4

//---------------------------------------------------------------------------

CHexManager::CHexManager()
{
    HexFilePtr = NULL;
    memset(HexFilePath, 0, sizeof(HexFilePath));
    memset(Ascii, 0, sizeof(Ascii));
    HexTotalLines = 0;
    HexCurrLineNo = 0;
}

//---------------------------------------------------------------------------

CHexManager::~CHexManager()
{
    if (HexFilePtr)
        fclose(HexFilePtr);
}

//---------------------------------------------------------------------------

bool CHexManager::LoadHexFile(const char* fName)
{
    char HexRec[255];

    strcpy_s(HexFilePath, sizeof(HexFilePath), fName);

    // Open file
    fopen_s(&HexFilePtr, HexFilePath, "r");
    if (HexFilePtr == NULL)
    {
        return false;
    }
    else
    {
        HexTotalLines = 0;
        while (!feof(HexFilePtr))
        {
            fgets(HexRec, sizeof(HexRec), HexFilePtr);
            HexTotalLines++;
        }
    }
    return true;
}

//---------------------------------------------------------------------------

unsigned short CHexManager::GetNextHexRecord(char* HexRec, unsigned int BuffLen)
{
    unsigned short len = 0;

    if (!feof(HexFilePtr))
    {
        fgets(Ascii, BuffLen, HexFilePtr);
        if (Ascii[0] != ':')
        {
            return 0;
        }
        // Convert rest to hex
        len = ConvertAsciiToHex((void*)&Ascii[1], (void*)HexRec);
        HexCurrLineNo++;
    }
    return len;
}

//---------------------------------------------------------------------------

bool CHexManager::ResetHexFilePointer(void)
{
    if (HexFilePtr == NULL)
    {
        return false;
    }
    else
    {
        fseek(HexFilePtr, 0, 0);
        HexCurrLineNo = 0;
        return true;
    }
}

//---------------------------------------------------------------------------

unsigned short CHexManager::ConvertAsciiToHex(void* VdAscii, void* VdHexRec)
{
    char temp[5] = { '0','x',0, 0, 0 };
    unsigned int i = 0;
    char* Ascii;
    char* HexRec;
    unsigned int value;

    Ascii = (char*)VdAscii;
    HexRec = (char*)VdHexRec;

    while (1)
    {
        temp[2] = Ascii[i++];
        temp[3] = Ascii[i++];
        if ((temp[2] == 0) || (temp[3] == 0))
        {
            i -= 2;
            break;
        }
        else
        {
            sscanf_s(temp, "%x", &value);
            *HexRec = (char)value;
            HexRec++;
        }
    }
    return (i / 2);
}

//---------------------------------------------------------------------------

void CHexManager::VerifyFlash(unsigned int* StartAdress, unsigned int* ProgLen, unsigned short* crc)
{
    unsigned short HexRecLen;
    char HexRec[255];
    T_HEX_RECORD HexRecordSt;
    unsigned int VirtualFlashAdrs;
    unsigned int ProgAddress;

    char* VirtualFlash = NULL;
    *crc = 0;
    *StartAdress = 0;
    *ProgLen = 0;

    VirtualFlash = (char*)malloc(5 * MB);
    if (!VirtualFlash) return;

    // Virtual Flash Erase (Set all bytes to 0xFF)
    memset((void*)VirtualFlash, 0xFF, 5 * MB);

    // Start decoding the hex file and write into virtual flash
    fseek(HexFilePtr, 0, 0);

    // Reset max address and min address
    HexRecordSt.MaxAddress = 0;
    HexRecordSt.MinAddress = 0xFFFFFFFF;
    HexRecordSt.ExtLinAddress = 0;
    HexRecordSt.ExtSegAddress = 0;

    while ((HexRecLen = GetNextHexRecord(&HexRec[0], 255)) != 0)
    {
        HexRecordSt.RecDataLen = HexRec[0];
        HexRecordSt.RecType = HexRec[3];
        HexRecordSt.Data = (unsigned char*)&HexRec[4];

        switch (HexRecordSt.RecType)
        {
        case DATA_RECORD:  // Record Type 00, data record
            HexRecordSt.Address = (((HexRec[1] << 8) & 0x0000FF00) | (HexRec[2] & 0x000000FF)) & (0x0000FFFF);
            HexRecordSt.Address = HexRecordSt.Address + HexRecordSt.ExtLinAddress + HexRecordSt.ExtSegAddress;
            ProgAddress = PA_TO_KVA0(HexRecordSt.Address);

            if (ProgAddress < BOOT_SECTOR_BEGIN) // Make sure we are not writing boot sector
            {
                if (HexRecordSt.MaxAddress < (ProgAddress + HexRecordSt.RecDataLen))
                {
                    HexRecordSt.MaxAddress = ProgAddress + HexRecordSt.RecDataLen;
                }
                if (HexRecordSt.MinAddress > ProgAddress)
                {
                    HexRecordSt.MinAddress = ProgAddress;
                }
                VirtualFlashAdrs = PA_TO_VFA(ProgAddress);
                memcpy((void*)&VirtualFlash[VirtualFlashAdrs], HexRecordSt.Data, HexRecordSt.RecDataLen);
            }
            break;

        case EXT_SEG_ADRS_RECORD:  // Record Type 02
            HexRecordSt.ExtSegAddress = ((HexRecordSt.Data[0] << 16) & 0x00FF0000) |
                ((HexRecordSt.Data[1] << 8) & 0x0000FF00);
            HexRecordSt.ExtLinAddress = 0;
            break;

        case EXT_LIN_ADRS_RECORD:  // Record Type 04
            HexRecordSt.ExtLinAddress = ((HexRecordSt.Data[0] << 24) & 0xFF000000) |
                ((HexRecordSt.Data[1] << 16) & 0x00FF0000);
            HexRecordSt.ExtSegAddress = 0;
            break;

        case END_OF_FILE_RECORD:  // Record Type 01
        default:
            HexRecordSt.ExtSegAddress = 0;
            HexRecordSt.ExtLinAddress = 0;
            break;
        }
    }

    HexRecordSt.MinAddress -= HexRecordSt.MinAddress % 4;
    HexRecordSt.MaxAddress += HexRecordSt.MaxAddress % 4;
    *ProgLen = HexRecordSt.MaxAddress - HexRecordSt.MinAddress;
    *StartAdress = HexRecordSt.MinAddress;
    VirtualFlashAdrs = PA_TO_VFA(HexRecordSt.MinAddress);
    *crc = CalculateCrc((char*)&VirtualFlash[VirtualFlashAdrs], *ProgLen);

    free(VirtualFlash);
}

//---------------------------------------------------------------------------
