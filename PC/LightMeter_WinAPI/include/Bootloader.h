//---------------------------------------------------------------------------
// Bootloader Window
// WinAPI version
//---------------------------------------------------------------------------

#ifndef BOOTLOADERH
#define BOOTLOADERH

#include <windows.h>
#include "USB_HID_CLASS.h"
#include "Hex.h"

#define WND_BOOTLOADER_CLASS L"BootloaderWindow"

// Control IDs
#define IDC_MEMO                2001
#define IDC_PROGRESS            2002
#define IDC_BTN_CONNECT         2003
#define IDC_BTN_VER             2004
#define IDC_BTN_HEX             2005
#define IDC_BTN_ERASE           2006
#define IDC_BTN_PROGRAM         2007
#define IDC_BTN_VERIFY          2008
#define IDC_BTN_ALL             2009
#define IDC_BTN_RUN             2010
#define IDC_TIMER_BOOT          2011

// Commands
typedef enum
{
    READ_BOOT_INFO = 1,
    ERASE_FLASH,
    PROGRAM_FLASH,
    READ_CRC,
    JMP_TO_APP
} T_COMMANDS;

typedef enum
{
    FIRST_TRY = 0,
    RE_TRY = 1
} T_TRY;

#define SOH 01
#define EOT 04
#define DLE 16

class CBootloader
{
public:
    CBootloader();
    ~CBootloader();

    bool Create(HINSTANCE hInst, HWND hParent);
    void ShowModal();
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hWnd;

private:
    HWND hParent;
    HINSTANCE hInstance;

    // Controls
    HWND hMemo;
    HWND hProgress;
    HWND hBtnConnect;
    HWND hBtnVer;
    HWND hBtnHex;
    HWND hBtnErase;
    HWND hBtnProgram;
    HWND hBtnVerify;
    HWND hBtnAll;
    HWND hBtnRun;

    // USB Device
    CUSBDevice* USBPORT;
    bool Connected;

    // Hex Manager
    CHexManager* HexManager;
    bool ResetHexFilePtr;
    bool HexLoad;

    // Communication
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
    bool EraseProgVer;

    // Methods
    void Buttons();
    void Delay(DWORD ms);
    void AppendText(const char* text);

    // Communication
    void RxTxThread();
    void ReceiveTask();
    void TransmitTask();
    unsigned short ReadPort(char* buffer, int bufflen);
    void WritePort(char* buffer, int bufflen);
    void HandleResponse();
    void HandleNoResponse();
    void StopTxRetries();
    bool SendCommand(char cmd, unsigned short Retries, unsigned short DelayInMs);
    unsigned short CalculateFlashCRC();
    void BuildRxFrame(unsigned char* RxBuff, unsigned short RxBuffLen);

    // Window procedure
    static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif
