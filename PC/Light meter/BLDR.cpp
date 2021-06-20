//---------------------------------------------------------------------------
#include <windows.h>
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#pragma warn -8027
#pragma warn -8004

#include "ccrc.h"
#include "USB_HID_CLASS.h"
#include "Hex.h"
#include "BLDR.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBootloader *Bootloader;

//---------------------------------------------------------------------------
#define SOH 01
#define EOT 04
#define DLE 16

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
__fastcall TBootloader::TBootloader(TComponent* Owner)
        : TForm(Owner)
{
 Connected = false;
 HexLoad = false;
 USBPORT = NULL;
 ExitThread = TRUE;
 ThreadKilled = FALSE;
 EraseProgVer = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/****************************************************************************
 * Create	Thread
 *
 * \param   hWnd  Main Window Handle
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::CreateRxTxThread(void)
{
 ExitThread = false;
}

//---------------------------------------------------------------------------

/****************************************************************************
 * Shut down the Receive transmit thread.
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::ShutdownThread(void)
{
 // Set a flag to exit the thread
 ExitThread = TRUE;
 ThreadKilled = FALSE;
}

//---------------------------------------------------------------------------

/****************************************************************************
 *  This thread calls receive and transmit tasks
 *
 * \param lpParam  this Pointer


 * \return  0 on exit.
 *****************************************************************************/
void TBootloader::RxTxThread(void)
{
  if (ExitThread == FALSE)
   {
    ReceiveTask();
    TransmitTask();
    Sleep(1);
   }
  else ThreadKilled = TRUE;
}

//---------------------------------------------------------------------------
/****************************************************************************
 *  Handle no response situation
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::HandleNoResponse(void)
{
  // Handle no response situation depending on the last sent command.
  switch(LastSentCommand)
   {
    case READ_BOOT_INFO:
    case ERASE_FLASH:
    case PROGRAM_FLASH:
    case READ_CRC:
      // Notify main window that there was no reponse.
      Memo->Lines->Add("No Response from the device. Operation failed");
      Buttons();
    break;
   }
}
//---------------------------------------------------------------------------

/****************************************************************************
 *  Handle Response situation
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::HandleResponse(void)
{
  unsigned char cmd = RxData[0];

  switch(cmd)
  {
    case READ_BOOT_INFO:
     Memo->Lines->Add(AnsiString().sprintf("Bootloader Firmware Version: %d.%d", (int)RxData[1], (int)RxData[2]));
    break;
    case ERASE_FLASH:
     Memo->Lines->Add("Flash Erased");
     if (EraseProgVer)// Operation Erase->Program->Verify
      {
      	// Erase completed. Next operation is programming.
        Memo->Lines->Add("programming");
      	SendCommand(PROGRAM_FLASH, 3, 500); // 500ms delay
      }
    break;
    case READ_CRC:
     {
      // Notify main window that command received successfully.
      unsigned short crc = ((RxData[2] << 8) & 0xFF00) | (RxData[1] & 0x00FF);
      if (crc == CalculateFlashCRC())
        Memo->Lines->Add("Verification successfull");
      else
	Memo->Lines->Add("Verification failed");
      if (EraseProgVer) EraseProgVer = false;
     }
    break;

    case PROGRAM_FLASH:
      // If there is a hex record, send next hex record.
      ResetHexFilePtr = false; // No need to reset hex file pointer.
      if(!SendCommand(PROGRAM_FLASH, MaxRetry, TxRetryDelay))
       {
        // Notify main window that programming operation completed.
        Memo->Lines->Add("Programming completed");
        if (EraseProgVer)// Operation Erase->Program->Verify
	 {
          Memo->Lines->Add("verifing");
	  // Programming completed. Next operation is verification.
	  SendCommand(READ_CRC, 3, 5000);// 5 second delay
	 }
       }
      ResetHexFilePtr = true;
    break;
  }
}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Read communication port (USB/COM/ETH)
 *
 * \param Buffer, Len
 * \return
 *****************************************************************************/
unsigned short TBootloader::ReadPort(char *buffer, int bufflen)
{
 if (USBPORT->Read(buffer, bufflen)) return 64;
 else return 0;
}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Write communication port (USB/COM/ETH)
 *
 * \param Buffer, Len
 * \return
 *****************************************************************************/
void TBootloader::WritePort(char *buffer, int bufflen)
{
 while(bufflen > 0)
  {
   USBPORT->Write(buffer, bufflen);
   bufflen -= USB_BUFFER_SIZE;
   buffer += USB_BUFFER_SIZE;
  }
}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Receive Task
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::ReceiveTask(void)
{
  unsigned short BuffLen;
  char Buff[255];

  BuffLen = ReadPort((char*)Buff, (sizeof(Buff) - 10));
  BuildRxFrame((unsigned char*)Buff, BuffLen);
  if(RxFrameValid)
   {
    // Valid frame is received.
    // Disable further retries.
    StopTxRetries();
    RxFrameValid = false;
    // Handle Response
    HandleResponse();
   }
  else
   {
    // Retries exceeded. There is no reponse from the device.
    if(NoResponseFromDevice)
     {
      // Reset flags
      NoResponseFromDevice = false;
      RxFrameValid = false;
      // Handle no response situation.
      HandleNoResponse();
     }
   }
}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Transmit task: Transmits frame if there is a frame to send.
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::TransmitTask(void)
{
  static unsigned long NextRetryTimeInMs;

  switch(TxState)
   {
    case FIRST_TRY:
      if(RetryCount)
       {
        // There is something to send.
        WritePort(TxPacket, TxPacketLen);
        RetryCount--;
        // If there is no response to "first try", the command will be retried.
        TxState = RE_TRY;
        // Next retry should be attempted only after a delay.
        NextRetryTimeInMs = GetTickCount() + TxRetryDelay;
       }
    break;

    case RE_TRY:
      if(RetryCount)
       {
        if(NextRetryTimeInMs < GetTickCount())
         {
          // Delay elapsed. Its time to retry.
          NextRetryTimeInMs = GetTickCount() + TxRetryDelay;
          WritePort(TxPacket, TxPacketLen);
          // Decrement retry count.
          RetryCount--;
         }
       }
      else
       {
        // Retries Exceeded
        NoResponseFromDevice = true;
        // Reset the state
        TxState = FIRST_TRY;
       }
    break;
  }
}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Stops transmission retries
 *
 * \param
 * \param
 * \param
 * \return
 *****************************************************************************/
void TBootloader::StopTxRetries(void)
{
  // Reset state.
  TxState = FIRST_TRY;
  RetryCount = 0;
}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Send Command
 *
 * \param		cmd:  Command
 * \param		data: Pointer to data buffer if any
 * \param 		dataLen: Data length
 * \param		retries: Number of retries allowed
 * \param		retryDelayInMs: Delay between retries in milisecond
 * \return
 *****************************************************************************/

bool TBootloader::SendCommand(char cmd, unsigned short Retries, unsigned short DelayInMs)
{
  unsigned short crc;
  unsigned int StartAddress,  Len;
  unsigned short BuffLen = 0;
  unsigned short HexRecLen;
  UINT totalRecords = 10;
  TxPacketLen = 0;

  // Store for later use.
  LastSentCommand = cmd;

  switch((unsigned char)cmd)
   {
    case READ_BOOT_INFO:
      Buff[BuffLen++] = cmd;
      MaxRetry = RetryCount = Retries;
      TxRetryDelay = DelayInMs; // in ms
    break;

    case ERASE_FLASH:
      Buff[BuffLen++] = cmd;
      MaxRetry = RetryCount = Retries;
      TxRetryDelay = DelayInMs; // in ms
    break;

    case JMP_TO_APP:
      Buff[BuffLen++] = cmd;
      MaxRetry = RetryCount = 1;
      TxRetryDelay = 10; // in ms
    break;

    case PROGRAM_FLASH:
      Buff[BuffLen++] = cmd;
      if(ResetHexFilePtr)
       {
        if(!HexManager->ResetHexFilePointer())
         {
          // Error in resetting the file pointer
          return false;
         }
       }
      HexRecLen = HexManager->GetNextHexRecord(&Buff[BuffLen], (sizeof(Buff) - 5));
      if(HexRecLen == 0)
       {
        //Not a valid hex file.
        return false;
       }

      BuffLen = BuffLen + HexRecLen;
      while(totalRecords)
       {
        HexRecLen = HexManager->GetNextHexRecord(&Buff[BuffLen], (sizeof(Buff) - 5));
        BuffLen = BuffLen + HexRecLen;
        totalRecords--;
       }
      MaxRetry = RetryCount = Retries;
      TxRetryDelay = DelayInMs; // in ms
    break;
    case READ_CRC:
      Buff[BuffLen++] = cmd;
      HexManager->VerifyFlash((unsigned int*)&StartAddress, (unsigned int*)&Len, (unsigned short*)&crc);
      Buff[BuffLen++] = (StartAddress);
      Buff[BuffLen++] = (StartAddress >> 8);
      Buff[BuffLen++] = (StartAddress >> 16);
      Buff[BuffLen++] = (StartAddress >> 24);
      Buff[BuffLen++] = (Len);
      Buff[BuffLen++] = (Len >> 8);
      Buff[BuffLen++] = (Len >> 16);
      Buff[BuffLen++] = (Len >> 24);
      Buff[BuffLen++] =  (char)crc;
      Buff[BuffLen++] =  (char)(crc >> 8);
      MaxRetry = RetryCount = Retries;
      TxRetryDelay = DelayInMs; // in ms
    break;

    default:
      return false;
    //break;
   }

  // Calculate CRC for the frame.
  crc = CalculateCrc(Buff, BuffLen);
  Buff[BuffLen++] = (char)crc;
  Buff[BuffLen++] = (char)(crc >> 8);

  // SOH: Start of header
  TxPacket[TxPacketLen++] = SOH;

  // Form TxPacket. Insert DLE in the data field whereever SOH and EOT are present.
  for (int i = 0; i < BuffLen; i++)
   {
    if ((Buff[i] == EOT) || (Buff[i] == SOH) || (Buff[i] == DLE))
     {
      TxPacket[TxPacketLen++] = DLE;
     }
    TxPacket[TxPacketLen++] = Buff[i];
   }

  // EOT: End of transmission
  TxPacket[TxPacketLen++] = EOT;

  return true;

}
//---------------------------------------------------------------------------
/****************************************************************************
 *  Builds the response frame
 *
 * \param  buff: Pointer to the data buffer
 * \param  buffLen: Buffer length
 * \param
 * \return
 *****************************************************************************/

void TBootloader::BuildRxFrame(unsigned char *buff, unsigned short buffLen)
{
  static bool Escape = false;
  unsigned short crc;


  while ((buffLen > 0) && (RxFrameValid == false))
   {
     buffLen --;
     if(RxDataLen >= (sizeof(RxData)-2))
      {
       RxDataLen = 0;
      }
     switch(*buff)
      {
       case SOH: //Start of header
         if(Escape)
          {
            // Received byte is not SOH, but data.
            RxData[RxDataLen++] = *buff;
            // Reset Escape Flag.
            Escape = FALSE;
          }
         else
          {
           // Received byte is indeed a SOH which indicates start of new frame.
           RxDataLen = 0;
          }
       break;

       case EOT: // End of transmission
         if(Escape)
          {
           // Received byte is not EOT, but data.
           RxData[RxDataLen++] = *buff;
           // Reset Escape Flag.
           Escape = FALSE;
          }
         else
          {
            // Received byte is indeed a EOT which indicates end of frame.
            // Calculate CRC to check the validity of the frame.
            if(RxDataLen > 1)
             {
              crc = (RxData[RxDataLen-2]) & 0x00ff;
              crc = crc | ((RxData[RxDataLen-1] << 8) & 0xFF00);
              if((CalculateCrc(RxData, (RxDataLen-2)) == crc) && (RxDataLen > 2))
               {
                // CRC matches and frame received is valid.
                RxFrameValid = TRUE;
               }
             }
          }
       break;

       case DLE: // Escape character received.
         if(Escape)
          {
           // Received byte is not ESC but data.
           RxData[RxDataLen++] = *buff;
           // Reset Escape Flag.
           Escape = FALSE;
          }
         else
          {
           // Received byte is an escape character. Set Escape flag to escape next byte.
           Escape = TRUE;
          }
       break;

       default: // Data field.
          RxData[RxDataLen++] = *buff;
          // Reset Escape Flag.
          Escape = FALSE;
       break;
      }
     // Increment the pointer.
     buff++;
  }
}

//---------------------------------------------------------------------------
/****************************************************************************
 *  Gets the progress of each command. This function can be used for progress
	bar.
 *
 * \param	Lower: Pointer to current count of the progress bar.
 * \param	Upper: Pointer to max count.
 * \param
 * \return
 *****************************************************************************/
void TBootloader::GetProgress(int* Lower, int* Upper)
{
  switch(LastSentCommand)
   {
    case READ_BOOT_INFO:
    case ERASE_FLASH:
    case READ_CRC:
    case JMP_TO_APP:
      // Progress with respect to retry count.
      *Lower = (MaxRetry - RetryCount);
      *Upper = MaxRetry;
    break;

    case PROGRAM_FLASH:
      // Progress with respect to line counts in hex file.
      *Lower = HexManager->HexCurrLineNo;
      *Upper = HexManager->HexTotalLines;
    break;
   }
}

//---------------------------------------------------------------------------
/****************************************************************************
 *  Gets locally calculated CRC
 *
 * \param
 * \param
 * \param
 * \return 16 bit CRC
 *****************************************************************************/
unsigned short TBootloader::CalculateFlashCRC(void)
{
  unsigned int StartAddress,  Len;
  unsigned short crc;
  HexManager->VerifyFlash((unsigned int*)&StartAddress, (unsigned int*)&Len, (unsigned short*)&crc);
  return crc;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TBootloader::FormCreate(TObject *Sender)
{
 //Check if some program with same mutex already run
 //   1240      Vendor ID
 //     60      Product ID: Custom HID device demo
 //      0      Device release number in BCD format
 HANDLE mutex = CreateMutex(NULL,true,"MutexOfHID_1240_60_0");
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
 HexManager = new CHexManager();
 ProgressBar->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBootloader::FormDestroy(TObject *Sender)
{
 delete USBPORT;
 delete HexManager;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TBootloader::Delay(DWORD ms, int SLEEP)
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

AnsiString __fastcall TBootloader::ConnectString(void)
{
 wchar_t str[256];
 AnsiString Res = "Not connected";
 if (!USBPORT->Product(str, 255)) return Res;
 Res = "Connected to " + WideCharToString(str) + " (c) ";
 if (!USBPORT->Manufacturer(str, 255)) return Res;
 return Res + WideCharToString(str);
}

//---------------------------------------------------------------------------
void __fastcall TBootloader::BConnectClick(TObject *Sender)
{
 if (!USBPORT->isConnected()) USBPORT->Connect(1240, 60, 0);
 else USBPORT->Disconnect();
 Buttons();
 if (USBPORT->isConnected())
  {
   Memo->Lines->Clear();
   Memo->Lines->Add("Device connected");
   BVerClick(Sender);
   Timer->Enabled = true;
   EraseProgVer = false;
  }
 else
  {
   ProgressBar->Position = 0;
   Timer->Enabled = false;
   Memo->Lines->Add("Device disconnected");
  }
}
//---------------------------------------------------------------------------
void __fastcall TBootloader::Buttons(void)
{
 if (USBPORT->isConnected())
  {
   BConnect->Caption = "&Disconnect";
   BHex->Enabled = true;
   BVer->Enabled = true;
   BErase->Enabled = true;
   if (HexLoad)
    {
     BAll->Enabled = true;
     BProgram->Enabled = true;
     BRun->Enabled = true;
     BVerify->Enabled = true;
    }
   else
    {
     BAll->Enabled = false;
     BProgram->Enabled = false;
     BRun->Enabled = false;
     BVerify->Enabled = false;
    }
  }
 else
  {
   BConnect->Caption = "&Connect";
   BHex->Enabled = false;
   BVer->Enabled = false;
   BAll->Enabled = false;
   BErase->Enabled = false;
   BProgram->Enabled = false;
   BRun->Enabled = false;
   BVerify->Enabled = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TBootloader::BVerClick(TObject *Sender)
{
 EraseProgVer = false;
 if (USBPORT->isConnected())
  {
    AnsiString Ver = ConnectString();
    Memo->Lines->Add(Ver);
    SendCommand(READ_BOOT_INFO, 50, 200);
    ExitThread = FALSE;
  }
}
//---------------------------------------------------------------------------


void __fastcall TBootloader::BHexClick(TObject *Sender)
{
 OpenDialog->InitialDir = ExtractFileDir(IniFile);
 OpenDialog->Filter = "Hex File (*.hex)|*.hex|";
 //OpenDialog->FileName = EeFile;
 OpenDialog->Title = "Open HEX file";
 Timer->Enabled = false;
 EraseProgVer = false;
 if (OpenDialog->Execute())
  {
   HexLoad = HexManager->LoadHexFile(OpenDialog->FileName);
   if (HexLoad)
    {
     int lines = HexManager->HexTotalLines;
     Memo->Lines->Add(AnsiString().sprintf("Hex file %d lines loaded successfully", lines));
    }
   else Memo->Lines->Add("Hex file not loaded");
   ResetHexFilePtr = HexLoad;
   Buttons();
  }
 Timer->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TBootloader::OnTimer(void)
{
  int Pos;
  int Upper;

  // Update progress bar.
  GetProgress(&Pos, &Upper);
  ProgressBar->Min = 0;
  ProgressBar->Max = Upper;
  ProgressBar->Position = Pos;
}
//---------------------------------------------------------------------------
void __fastcall TBootloader::TimerTimer(TObject *Sender)
{
 Timer->Enabled = false;
 RxTxThread();
 OnTimer();
 Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TBootloader::BEraseClick(TObject *Sender)
{
 ProgressBar->Min = 0;
 EraseProgVer = false;
 Memo->Lines->Add("erasing");
 SendCommand(ERASE_FLASH, 3, 5000); //5s retry delay, becuse erase takes considerable time.
 ExitThread = FALSE;
 Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TBootloader::BProgramClick(TObject *Sender)
{
 ProgressBar->Min = 0;
 ResetHexFilePtr = true;
 EraseProgVer = false;
 Memo->Lines->Add("programming");
 if (SendCommand(PROGRAM_FLASH, 3, 5000)) // 500ms delay
  {
   ExitThread = FALSE;
   Timer->Enabled = true;
  }
 else Memo->Lines->Add("Hex file not loaded");
}
//---------------------------------------------------------------------------

void __fastcall TBootloader::BRunClick(TObject *Sender)
{
 EraseProgVer = false;
 Memo->Lines->Add("Command issued to run application");
 if (SendCommand(JMP_TO_APP, 1, 10)) // 10ms delay
  {
   ExitThread = FALSE;
   Timer->Enabled = true;
  }
 else Memo->Lines->Add("Error");
}
//---------------------------------------------------------------------------

void __fastcall TBootloader::BVerifyClick(TObject *Sender)
{
 EraseProgVer = false;
 Memo->Lines->Add("verifing");
 if (SendCommand(READ_CRC, 3, 5000))
  {
   ExitThread = FALSE;
   Timer->Enabled = true;
  }
 else Memo->Lines->Add("Error");
}
//---------------------------------------------------------------------------

void __fastcall TBootloader::BAllClick(TObject *Sender)
{
 EraseProgVer = true;
 Memo->Lines->Add("erasing");
 SendCommand(ERASE_FLASH, 3, 5000); //5s retry delay, becuse erase takes considerable time.
 ExitThread = FALSE;
 Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TBootloader::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 USBPORT->Disconnect();
 Buttons();
 ProgressBar->Position = 0;
 Timer->Enabled = false;
 Memo->Lines->Add("Device disconnected");
}

//---------------------------------------------------------------------------
void __fastcall TBootloader::WMChangeDev(TMessage &Message)
{
  if (USBPORT->isConnected())
   {
    USBPORT->Connect();
    if (!USBPORT->isConnected())
     {
      ProgressBar->Position = 0;
      Timer->Enabled = false;
      Memo->Lines->Add("Device disconnected");
     }
    Buttons();
   }
  else BConnectClick(this);
  //int Lparam = Message.LParam;
}

//---------------------------------------------------------------------------

