//---------------------------------------------------------------------------
// Hex file manager - WinAPI version
//---------------------------------------------------------------------------

#ifndef HEXH
#define HEXH

#include <windows.h>
#include <stdio.h>

typedef struct
{
    unsigned char RecDataLen;
    unsigned int Address;
    unsigned int MaxAddress;
    unsigned int MinAddress;
    unsigned char RecType;
    unsigned char* Data;
    unsigned char CheckSum;
    unsigned int ExtSegAddress;
    unsigned int ExtLinAddress;
} T_HEX_RECORD;

// Hex Manager class
class CHexManager
{
public:
    CHexManager();
    ~CHexManager();

    unsigned int HexTotalLines;
    unsigned int HexCurrLineNo;

    bool ResetHexFilePointer(void);
    bool LoadHexFile(const char* fName);
    unsigned short GetNextHexRecord(char* HexRec, unsigned int BuffLen);
    unsigned short ConvertAsciiToHex(void* VdAscii, void* VdHexRec);
    void VerifyFlash(unsigned int* StartAdress, unsigned int* ProgLen, unsigned short* crc);

private:
    char HexFilePath[MAX_PATH];
    FILE* HexFilePtr;
    char Ascii[1000];
};

#endif
