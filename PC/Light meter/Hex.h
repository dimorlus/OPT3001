//---------------------------------------------------------------------------
#ifndef HexH
#define HexH
//---------------------------------------------------------------------------
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
}T_HEX_RECORD;

// Hex Manager class
class CHexManager
{
public:
	CHexManager();
	~CHexManager();

	unsigned int HexTotalLines;
	unsigned int HexCurrLineNo;

	bool ResetHexFilePointer(void);
	bool LoadHexFile(const AnsiString fName);
	unsigned short GetNextHexRecord(char *HexRec, unsigned int BuffLen);
	unsigned short ConvertAsciiToHex(void *VdAscii, void *VdHexRec);
	void VerifyFlash(unsigned int* StartAdress, unsigned int* ProgLen, unsigned short* crc);
private:
	AnsiString HexFilePath;
	FILE *HexFilePtr;
        char Ascii[1000];
};

#endif
