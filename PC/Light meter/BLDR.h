//---------------------------------------------------------------------------

#ifndef BLDRH
#define BLDRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
// Commands
typedef enum
{
  READ_BOOT_INFO = 1,
  ERASE_FLASH,
  PROGRAM_FLASH,
  READ_CRC,
  JMP_TO_APP
}T_COMMANDS;

typedef enum
{
 FIRST_TRY = 0,
 RE_TRY = 1
}T_TRY;


class TBootloader : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo;
        TProgressBar *ProgressBar;
        TButton *BVer;
        TButton *BHex;
        TButton *BConnect;
        TButton *BErase;
        TButton *BProgram;
        TButton *BVerify;
        TButton *BAll;
        TButton *BRun;
        TTimer *Timer;
        TOpenDialog *OpenDialog;
        void __fastcall BConnectClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall BVerClick(TObject *Sender);
        void __fastcall BHexClick(TObject *Sender);
        void __fastcall TimerTimer(TObject *Sender);
        void __fastcall BEraseClick(TObject *Sender);
        void __fastcall BProgramClick(TObject *Sender);
        void __fastcall BRunClick(TObject *Sender);
        void __fastcall BVerifyClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BAllClick(TObject *Sender);
private:	// User declarations
        CUSBDevice *USBPORT;
        bool Connected;

        bool EraseProgVer;
        
        CHexManager *HexManager;
        bool ResetHexFilePtr;
        bool HexLoad;

        AnsiString IniFile;

        char Buff[1000];
	char TxPacket[1000];
	unsigned short TxPacketLen;
	char RxData[255];
	unsigned short RxDataLen;
	unsigned short RetryCount;

	bool RxFrameValid;
	unsigned char LastSentCommand;
	bool NoResponseFromDevice;
	unsigned int TxState;
	unsigned short MaxRetry;
	unsigned short TxRetryDelay;
       	bool ExitThread;
	bool ThreadKilled;


        void __fastcall Buttons(void);

        void __fastcall Delay(DWORD ms, int SLEEP);
        AnsiString __fastcall ConnectString(void);

        void RxTxThread(void);
        void ShutdownThread(void);
        void TBootloader::CreateRxTxThread(void);

        unsigned short ReadPort(char *buffer, int bufflen);
        void WritePort(char *buffer, int bufflen);
        void HandleResponse(void);
        void HandleNoResponse(void);
        void ReceiveTask(void);
        void TransmitTask(void);
        void StopTxRetries(void);
        bool SendCommand(char cmd, unsigned short Retries, unsigned short DelayInMs);
        void BuildRxFrame(unsigned char *buff, unsigned short buffLen);
        void GetProgress(int* Lower, int* Upper);
        unsigned short CalculateFlashCRC(void);

        void __fastcall OnTimer(void);
protected:
        void __fastcall WMChangeDev(TMessage &Message);
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_DEVICECHANGE, TMessage, WMChangeDev)
END_MESSAGE_MAP(TControl)


public:		// User declarations
        __fastcall TBootloader(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBootloader *Bootloader;
//---------------------------------------------------------------------------
#endif
