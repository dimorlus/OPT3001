//---------------------------------------------------------------------------

#ifndef LightMtrFrmH
#define LightMtrFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TOPT : public TForm
{
__published:	// IDE-managed Components
        TBevel *Bevel;
        TLabel *LConnect;
        TLabel *LVer;
        TEdit *EdKlux;
        TLabel *Led;
        TLabel *Llux0;
        TLabel *Llum0;
        TTimer *Timer;
        TLabel *LEdOk;
        TEdit *EdMaster;
        TLabel *LMaster;
        TLabel *LMOk;
        TButton *BBldr;
        void __fastcall TimerTimer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall EdKluxKeyPress(TObject *Sender, char &Key);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall EdMasterKeyPress(TObject *Sender, char &Key);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall BBldrClick(TObject *Sender);
private:	// User declarations
        bool Connected;
        CUSBDevice *USBPORT;
        int timer_tick;
        double Klum;
        double Master;
        double AvgLux;
        bool SaveKlum;
        bool SaveMaster;
        AnsiString IniFile;

        void __fastcall Init(void);

        void __fastcall Delay(DWORD ms, int SLEEP);
        bool __fastcall GetAD(int &ad0, int &ad1);
        bool __fastcall ConnectTo(void);
        void __fastcall ConnectStr(void);
        AnsiString __fastcall ConnectString(void);
        AnsiString __fastcall VersionString(void);

        double GetOPT(int chip);
        bool SetOPTcfg(int chip);
        int GetOPTreg(int chip, unsigned char Reg);
        bool SetOPTreg(int chip, unsigned char Reg, int Data);

        bool SetBlink(bool blink, unsigned char bst);

        bool __fastcall PutKlum(unsigned char chip, double dd);
        double __fastcall GetKlum(unsigned char chip);
        void __fastcall TimerHandle(void);

protected:
        void __fastcall WMChangeDev(TMessage &Message);
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_DEVICECHANGE, TMessage, WMChangeDev)
END_MESSAGE_MAP(TControl)

public:		// User declarations
        __fastcall TOPT(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOPT *OPT;
//---------------------------------------------------------------------------
#endif
