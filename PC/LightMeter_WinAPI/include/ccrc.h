//---------------------------------------------------------------------------
// CRC calculation module - WinAPI version
//---------------------------------------------------------------------------

#ifndef CCRCH
#define CCRCH

unsigned short CalculateCrc(char* data, unsigned int len);
unsigned short UpdateCrc(unsigned short crc, char* data, unsigned int len);

#endif
