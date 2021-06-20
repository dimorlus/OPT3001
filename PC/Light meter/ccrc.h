//---------------------------------------------------------------------------

#ifndef ccrcH
#define ccrcH
//---------------------------------------------------------------------------
unsigned short CalculateCrc(char *data, unsigned int len);
unsigned short UpdateCrc(unsigned short crc, char *data, unsigned int len);

#endif
