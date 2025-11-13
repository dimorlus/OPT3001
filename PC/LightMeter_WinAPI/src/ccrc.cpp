//---------------------------------------------------------------------------
// CRC calculation module - WinAPI version
//---------------------------------------------------------------------------

#include "../include/ccrc.h"

//---------------------------------------------------------------------------

static const unsigned short crc_table[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

//---------------------------------------------------------------------------

unsigned short CalculateCrc(char* data, unsigned int len)
{
    unsigned int i;
    unsigned short crc = 0;

    while (len--)
    {
        i = (crc >> 12) ^ (*data >> 4);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        i = (crc >> 12) ^ (*data >> 0);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        data++;
    }
    return (crc & 0xFFFF);
}

//---------------------------------------------------------------------------

unsigned short UpdateCrc(unsigned short crc, char* data, unsigned int len)
{
    unsigned int i;

    crc &= 0xFFFF;
    while (len--)
    {
        i = (crc >> 12) ^ (*data >> 4);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        i = (crc >> 12) ^ (*data >> 0);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        data++;
    }
    return (crc & 0xFFFF);
}

//---------------------------------------------------------------------------
