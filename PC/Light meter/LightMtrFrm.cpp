//---------------------------------------------------------------------------
#include <windows.h>
#include <vcl.h>
#include <math.h>
#include <stdio.h>
#pragma hdrstop

#pragma warn -8027
#pragma warn -8004

#include "USB_HID_CLASS.h"
#include "Hex.h"
#include "BLDR.h"
#include "LightMtrFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOPT *OPT;
//---------------------------------------------------------------------------
__fastcall TOPT::TOPT(TComponent* Owner)
    : TForm(Owner)
{
 Init();
 Connected = false;
 USBPORT = NULL;
 Klum = -1.0;
 Master = -1.0;
 SaveKlum = false;
 SaveMaster = false;
 timer_tick = 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TOPT::Delay(DWORD ms, int SLEEP)
{
  DWORD time, time1, time2;
  timeBeginPeriod(1);
  time1 = GetTickCount();
  do
   {
    if (SLEEP < 0) Sleep(ms);
    else Sleep(SLEEP);
    if (ms > 50) Application->ProcessMessages();
    time2 = GetTickCount();
    time = time2 - time1;
   }
  while (time < ms);
  timeEndPeriod(1);
}

//---------------------------------------------------------------------------
// Get light data from OPT3001
//---------------------------------------------------------------------------
double TOPT::GetOPT(int chip)
{
 unsigned char cmd[8];
 unsigned char res[8];

 cmd[0] = 15;         // srd_11(srd_11(u8 Ctl, u8 Size, u16 Addr, u8 *Data)
 cmd[1] = 0x80;       // Ctl (I2C address of the OPT3001 = 0x88)
 cmd[2] = 0x00;       // Register Addr
 cmd[3] = 4+(chip&3); // Addr (low part of I2C address will be << 1)
 cmd[4] = 2;          // Size;
 Delay(2, 0);
 if (USBPORT->transfer(5, cmd, 2+1, res) && (res[0] == cmd[0]))
  {
   int data = res[1]*256+res[2];
   int  mantissa = data & 0x0fff;
   int  exponent = (data & 0xf000)>>12;
   double Res = 0.01*mantissa*(1<<exponent);
   Delay(10, 0);
   return Res;
  }
 else return -1.0;
}

//---------------------------------------------------------------------------
// Get 16 bit register from OPT3001
//---------------------------------------------------------------------------
int TOPT::GetOPTreg(int chip, unsigned char Reg)
{
 unsigned char cmd[8];
 unsigned char res[8];

 cmd[0] = 15;         // srd_11(srd_11(u8 Ctl, u8 Size, u16 Addr, u8 *Data)
 cmd[1] = 0x80;       // Ctl (I2C address of the OPT3001 = 0x88)
 cmd[2] = Reg;        // Register Addr
 cmd[3] = 4+(chip&3); // Addr (low part of I2C address will be << 1)
 cmd[4] = 2;          // Size;
 Delay(2, 0);
 if (USBPORT->transfer(5, cmd, 2+1, res) && (res[0] == cmd[0]))
  {
   int data = res[1]*256+res[2];
   Delay(10, 0);
   return data;
  }
 else return -1;
}

//---------------------------------------------------------------------------
// Set 16 bit register of OPT3001
//---------------------------------------------------------------------------
bool TOPT::SetOPTreg(int chip, unsigned char Reg, int Data)
{
 unsigned char cmd[8];
 unsigned char res[8];

 Data &= 0xffff;
 cmd[0] = 14;          // pwr_11(u8 Ctl, u8 Size, u16 Addr, const u8 *Data)
 cmd[1] = 0x80;        // Ctl (I2C address of the OPT3001 = 0x88)
 cmd[2] = Reg;         // Register Addr
 cmd[3] = 4+(chip&3);  // Addr (low part of I2C address will be << 1)
 cmd[4] = 2;           // Size;
 cmd[5] = Data/256;    // Data MSB
 cmd[6] = Data%256;    // Data LSB
 Delay(2, 0);
 if (USBPORT->transfer(7, cmd, 1, res) && (res[0] == cmd[0]))
  {
   Delay(10, 0);
   return true;
  }
 else return false;
}

//---------------------------------------------------------------------------
// Setup OPT3001
//---------------------------------------------------------------------------
bool TOPT::SetOPTcfg(int chip)
{
 unsigned char cmd[8];
 unsigned char res[8];

 int CFG =     // 0xcc00
 (0x0c << 12)+ //RN[3:0]   Range number field (automatic full-scale-range)
 (0x01 << 11)+ //CT        Conversion time field (800 ms)
 (0x02 <<  9)+ //M[1:0]    Mode of conversion operation field (Continuous conversions)
 (0x00 <<  8)+ //OVF       Overflow flag field (read-only)
 (0x00 <<  7)+ //CRF       Conversion ready field (read-only)
 (0x00 <<  6)+ //FH        Flag high field (read-only)
 (0x00 <<  5)+ //FL        Flag low field (read-only)
 (0x00 <<  4)+ //L         Latch field
 (0x00 <<  3)+ //POL       Polarity field
 (0x00 <<  2)+ //ME        Mask exponent field
 (0x00 <<  0); //FC[1:0]   Fault count field

 cmd[0] = 14;          // pwr_11(u8 Ctl, u8 Size, u16 Addr, const u8 *Data)
 cmd[1] = 0x80;        // Ctl (I2C address of the OPT3001 = 0x88)
 cmd[2] = 0x01;        // Register Addr
 cmd[3] = 4+(chip&3);  // Addr (low part of I2C address will be << 1)
 cmd[4] = 2;           // Size;
 cmd[5] = CFG/256;     // 0xcc;
 cmd[6] = CFG%256;     // 0x00;
 Delay(2, 0);
 if (USBPORT->transfer(7, cmd, 1, res) && (res[0] == cmd[0]))
  {
   Delay(10, 0);
   return true;
  }
 else return false;
}

//---------------------------------------------------------------------------
// LEDs indicator control
//---------------------------------------------------------------------------
bool TOPT::SetBlink(bool blink, unsigned char bst)
{
 unsigned char cmd[8];
 unsigned char res[8];

 cmd[0] = 21;          // Blink control
 cmd[1] = blink?1:0;
 cmd[2] = bst;
 Delay(2, 0);
 if (USBPORT->transfer(3, cmd, 1, res) && (res[0] == cmd[0]))
  {
   Delay(10, 0);
   return true;
  }
 else return false;
}

//---------------------------------------------------------------------------
// Read version string
//---------------------------------------------------------------------------
AnsiString __fastcall TOPT::VersionString(void)
{
 unsigned char cmd[1];
 unsigned char res[64];

 cmd[0] = 20; // GetVer
 Delay(2, 0);
 if (!USBPORT->transfer(1, cmd, 63, res)) return "";
 return AnsiString().sprintf("%s", &res[1]);
}

//---------------------------------------------------------------------------
// Read ADC value (2 channel)
//---------------------------------------------------------------------------
bool __fastcall TOPT::GetAD(int &ad0, int &ad1)
{
 unsigned char cmd[8];
 unsigned char res[8];

 cmd[0] = 55;
 Delay(2, 0);
 if (USBPORT->transfer(1, cmd, 5, res) && (res[0] == cmd[0]))
  {
   ad0 = res[1]+256*res[2];
   ad1 = res[3]+256*res[4];
   Delay(10, 0);
   return true;
  }
 else return false;
}

//---------------------------------------------------------------------------
 typedef union udbl
 {
  double d;
  unsigned char cd[sizeof(double)];
 } tudbl;

//---------------------------------------------------------------------------
// Put double value to EEPROM as two bytes sequence. First one as is,
// and second one with each byte + 85. Than check it when reading.
//---------------------------------------------------------------------------
bool __fastcall TOPT::PutKlum(unsigned char chip, double dd)
{
 unsigned char cmd[5+sizeof(tudbl)];
 unsigned char res[8];
 tudbl d0;

 int eeAddr = chip*2*sizeof(tudbl);
 unsigned char size = sizeof(tudbl);

 cmd[0] = 14;            // pwr_11(u8 Ctl, u8 Size, u16 Addr, const u8 *Data)
 cmd[1] = 0xa0;          // Ctl (I2C address of the 24lc02)
 cmd[2] = eeAddr % 256;  // Addr LSB
 cmd[3] = eeAddr / 256;  // Addr MSB
 cmd[4] = size;          // Size;
 Delay(2, 0);

 d0.d=dd;
 for(int i=0; i < sizeof(tudbl); i++) cmd[i+5] = d0.cd[i];
 if (USBPORT->transfer(size+5, cmd, 1, res) && (res[0] == cmd[0]))
  {
   Delay(10, 0);

   eeAddr = (chip*2+1)*sizeof(tudbl);
   cmd[2] = eeAddr % 256;  // Addr LSB
   cmd[3] = eeAddr / 256;  // Addr MSB
   for(int i=0; i < sizeof(tudbl); i++) cmd[i+5] = (d0.cd[i]+85)&255;
   if (USBPORT->transfer(size+5, cmd, 1, res) && (res[0] == cmd[0]))
    {
     Delay(10, 0);
     return true;
    }
  }
 return false;
}

//---------------------------------------------------------------------------
// Get double value from EEPROM. Return -1 if error.
//---------------------------------------------------------------------------
double __fastcall TOPT::GetKlum(unsigned char chip)
{
 unsigned char cmd[8];
 unsigned char res[2*sizeof(tudbl)+2];

 int eeAddr = chip*2*sizeof(tudbl);
 unsigned char size = 2*sizeof(tudbl);

 cmd[0] = 15;            // srd_11(srd_11(u8 Ctl, u8 Size, u16 Addr, u8 *Data)
 cmd[1] = 0xa0;          // Ctl (I2C address of the 24lc02)
 cmd[2] = eeAddr % 256;  // Addr LSB
 cmd[3] = eeAddr / 256;  // Addr MSB
 cmd[4] = size;          // Size;
 Delay(2, 0);
 if (USBPORT->transfer(5, cmd, size+1, res) && (res[0] == cmd[0]))
  {
   tudbl d0, d1;
   unsigned char chk = 0;
   for(int i=0; i < sizeof(tudbl); i++) d0.cd[i] = res[i+1];
   for(int i=0; i < sizeof(tudbl); i++) d1.cd[i] = res[i+1+sizeof(tudbl)];
   for(int i=0; i < sizeof(tudbl); i++) chk += (d0.cd[i] - d1.cd[i] + 85)&255;

   Delay(10, 0);
   if (chk) return -1.0;
   else return d0.d;
  }
 else return -1.0;
}
//---------------------------------------------------------------------------

AnsiString __fastcall TOPT::ConnectString(void)
{
 wchar_t str[256];
 AnsiString Res = "Not connected";
 if (!USBPORT->Product(str, 255)) return Res;
 Res = "Connected to " + WideCharToString(str) + " (c) ";
 if (!USBPORT->Manufacturer(str, 255)) return Res;
 return Res + WideCharToString(str);
}

//---------------------------------------------------------------------------

void __fastcall TOPT::ConnectStr(void)
{
 Connected = USBPORT->isConnected();
 if (Connected) LConnect->Caption = ConnectString();
 else LConnect->Caption = "Not connected";
}
//---------------------------------------------------------------------------
void __fastcall TOPT::Init(void)
{
 LConnect->Enabled = false;
 Llux0->Caption = "";
 Llum0->Caption = "";
 LVer->Caption = "";
 EdKlux->Enabled = false;
 EdMaster->Enabled = false;
 Klum = -1.0;
 Master = -1.0;
 timer_tick = 0;
 AvgLux = 0;
}

//---------------------------------------------------------------------------
bool __fastcall TOPT::ConnectTo(void)
{
 // from usb_descriptors.c
 //   1240      Vendor ID
 //     66      Product ID: Custom HID device demo
 //      1      Device release number in BCD format
 if (!USBPORT->isConnected()) USBPORT->Connect(1240, 66, 1);
 if (USBPORT->isConnected())
  {
   LConnect->Enabled = true;
   EdKlux->Enabled = true;
   EdMaster->Enabled = true;
   return true;
  }
 else
  {
   Init();
   return false;
  }
}

//---------------------------------------------------------------------------
// Convert to engineering format (1e3 = 1k)
//---------------------------------------------------------------------------
void d2scistr(char *str, double d)
{
 const char csci[] = {'y', 'z', 'a', 'f', 'p', 'n', 'u', 'm', ' ',
                      'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y'};

 double dd = fabs(d);
 int rng = 8;
 if (dd > 0)
  {
   while (dd > 1000)
    {
      rng++;
      dd /= 1000;
    }
   while (dd < 1)
    {
     rng--;
     dd *= 1000;
     if (rng < 0) break;
    }
  }
 if (d < 0) dd = -dd;
 if (rng == 8)
  sprintf(str, "%.2f", d);
 else
 if ((rng >=0) && (rng < 17))
  sprintf(str, "%.2f%c", dd, csci[rng]);
 else
  sprintf(str, "%.2e", d);
}
//---------------------------------------------------------------------------

AnsiString AddBackslash(const AnsiString Str)
{
 if (Str != "") return IncludeTrailingPathDelimiter(Str);
 else return Str;
}

//---------------------------------------------------------------------------

AnsiString GetAppFolder(void)
{
 char AppData[1024];
 AnsiString AppDataFolder;

 if (GetEnvironmentVariable("APPDATA", AppData, 1024))
  AppDataFolder.sprintf("%s\\GTK\\LightMtr\\", AppData);
 if (DirectoryExists(AppDataFolder)) return AppDataFolder;
 else return AddBackslash(ExtractFilePath(Application->ExeName));
}

//---------------------------------------------------------------------------
// Main functionality (main loop)
//---------------------------------------------------------------------------
void __fastcall TOPT::TimerHandle(void)
{
 int ad0, ad1;
 double lux = -1.0;
 double lum = -1.0;

 if (SaveKlum)
  {
   SaveKlum = false;
   if ((Klum > 0) && PutKlum(0, Klum)) LEdOk->Caption = "Ok";
   return;
  }

 if (SaveMaster)
  {
   SaveMaster = false;
   if ((Master > 0) && PutKlum(4, Master)) LMOk->Caption = "Ok";
  }

 if (LVer->Caption.IsEmpty())
  {
   LVer->Caption = VersionString();
   return;
  }

 if (timer_tick++%10==0) //Check each 1s
 {
  if (Klum < 0)
   {
    Klum = GetKlum(0);
    EdKlux->Text = AnsiString().sprintf("%.8g", Klum);
   }
  if (Master < 0)
   {
     Master = GetKlum(4);
     if (Master < 0) Master = 1.0;
     EdMaster->Text = AnsiString().sprintf("%.8g", Master);
   }
  //if (((GetOPTreg(0, 1) & 0xff00) != 0xcc00) && SetOPTreg(0, 1, 0xcc00)) SetBlink(false, 0);
  //return;
 }

#ifdef _ADC_
 if (ADC->Checked)
  {
   if (GetAD(ad0, ad1))
    {
     Lad0->Caption = AnsiString().sprintf("ad0: %d", ad0);
     Lad1->Caption = AnsiString().sprintf("ad1: %d", ad1);
    }
  }
 else
  {
   Lad0->Caption = "";
   Lad1->Caption = "";
  }
#endif /* _ADC_ */


 int oSt = GetOPTreg(0, 1); // Read Reg @1 (Configuration Register)
 if (oSt < 0)
  {
   Llux0->Caption = "";
   Llum0->Caption = "";
   SetBlink(true, 1);
  }
 else
 if (((oSt & 0xfe00) != 0xcc00) && SetOPTreg(0, 1, 0xcc00)) SetBlink(false, 0);
 else
 if (oSt & (1<<7)) //Conversion ready flag
  {
   lux = Master*GetOPT(0);
   if (lux >= 0)
    {
      char str[64];

      //Low pass filter
      if (fabs(lux - AvgLux) > 0.1*lux) AvgLux = lux;
      else AvgLux += 0.05*(lux - AvgLux);

      d2scistr(str, AvgLux);
      if (oSt & (1<<8)) //Overflow flag
       Llux0->Caption = AnsiString().sprintf("Lx0: >%sLux", str);
      else
       Llux0->Caption = AnsiString().sprintf("Lx0: %sLux", str);
      if (Klum > 0)
       {
        lum = Klum*AvgLux;
        d2scistr(str, lum);
        if (oSt & (1<<8)) //Overflow flag
         Llum0->Caption = AnsiString().sprintf("Lx0: >%sLum", str);
        else
         Llum0->Caption = AnsiString().sprintf("Lx0: %sLum", str);
       }
      else Llum0->Caption = "";
      SetBlink(false, 0);
    }
   else
    {
     Llux0->Caption = "";
     Llum0->Caption = "";
     SetBlink(true, 1);
    }
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TOPT::TimerTimer(TObject *Sender)
{
 Timer->Enabled = false;

 Sleep(0);
 Application->ProcessMessages();
 if (!USBPORT->isConnected()) ConnectTo();
 else TimerHandle();
 ConnectStr();

 Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TOPT::FormCreate(TObject *Sender)
{
 //Check if some program with same mutex already run
 //   1240      Vendor ID
 //     66      Product ID: Custom HID device demo
 //      1      Device release number in BCD format
 HANDLE mutex = CreateMutex(NULL,true,"MutexOfHID_1240_66_1");
 DWORD result = WaitForSingleObject(mutex,0);
 if(result != WAIT_OBJECT_0)
  {
   ShowMessage("Program already run");
   Application->ShowMainForm = false;
   Application->Terminate();
  }
 Timer->Enabled = false;
 AnsiString AppDataFolder = GetAppFolder();

 IniFile = AppDataFolder+ExtractFileName(ChangeFileExt(Application->ExeName, ".ini"));
 if (!FileExists(IniFile))
  {
   AppDataFolder = ExtractFilePath(Application->ExeName);
   IniFile = ChangeFileExt(Application->ExeName, ".ini");
  }

 USBPORT = new CUSBDevice();
 Klum = -1.0;
 timer_tick = 0;

 //{
 // char str[64];
 // d2scistr(str, 0);
 //}
}
//---------------------------------------------------------------------------

void __fastcall TOPT::FormShow(TObject *Sender)
{
 Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TOPT::FormClose(TObject *Sender, TCloseAction &Action)
{
 if (USBPORT->isConnected()) SetBlink(true, 1);
}
//---------------------------------------------------------------------------

void __fastcall TOPT::EdKluxKeyPress(TObject *Sender, char &Key)
{
  LEdOk->Caption = "";
  if (Key == VK_RETURN)
  {
   Key = 0;
   try
    {
     Klum = EdKlux->Text.ToDouble();
     SaveKlum = true;
    }
   catch(...)
    {
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TOPT::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_ESCAPE)
  {
   Key = 0;
   Close();
  }
  else
  if (Key == 'B')
  {
   Key = 0;
   Timer->Enabled = false;
   Bootloader->ShowModal();
   Timer->Enabled = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TOPT::EdMasterKeyPress(TObject *Sender, char &Key)
{
  LMOk->Caption = "";
  if (Key == VK_RETURN)
  {
   Key = 0;
   try
    {
     Master = EdMaster->Text.ToDouble();
     SaveMaster = true;
    }
   catch(...)
    {
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TOPT::WMChangeDev(TMessage &Message)
{
  if (USBPORT->isConnected())
   {
    USBPORT->Connect();
    if (!USBPORT->isConnected())
     {
     }
   }
  else Init();
  //int Lparam = Message.LParam;
}

//---------------------------------------------------------------------------

void __fastcall TOPT::FormDestroy(TObject *Sender)
{
 delete USBPORT;
}
//---------------------------------------------------------------------------

void __fastcall TOPT::BBldrClick(TObject *Sender)
{
 Timer->Enabled = false;

 unsigned char cmd = 25; //Swith to bootloader mode
 USBPORT->Write(&cmd, 1);
 Bootloader->ShowModal();
 Timer->Enabled = true;
}
//---------------------------------------------------------------------------

