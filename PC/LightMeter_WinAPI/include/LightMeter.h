//---------------------------------------------------------------------------
// Main Window - Light Meter
// WinAPI version
//---------------------------------------------------------------------------

#ifndef LIGHTMETERH
#define LIGHTMETERH

#include <windows.h>
#include "USB_HID_CLASS.h"

// Window class name
#define WND_CLASS_NAME L"LightMeterWindow"

// Control IDs
#define IDC_LABEL_CONNECT       1001
#define IDC_LABEL_VERSION       1002
#define IDC_LABEL_LUX           1003
#define IDC_LABEL_LUM           1004
#define IDC_EDIT_KLUX           1005
#define IDC_LABEL_KLUX          1006
#define IDC_LABEL_KLUX_OK       1007
#define IDC_EDIT_MASTER         1008
#define IDC_LABEL_MASTER        1009
#define IDC_LABEL_MASTER_OK     1010
#define IDC_BUTTON_BOOTLOADER   1011
#define IDC_TIMER               1012

// USB Device IDs
#define USB_VID 1240
#define USB_PID 66
#define USB_VER 1

class CLightMeter
{
public:
    CLightMeter();
    ~CLightMeter();

    bool Create(HINSTANCE hInst);
    void Show();
    void MessageLoop();

private:
    HWND hWnd;
    HINSTANCE hInstance;
    
    // Controls
    HWND hLabelConnect;
    HWND hLabelVersion;
    HWND hLabelLux;
    HWND hLabelLum;
    HWND hEditKlux;
    HWND hLabelKlux;
    HWND hLabelKluxOk;
    HWND hEditMaster;
    HWND hLabelMaster;
    HWND hLabelMasterOk;
    HWND hButtonBootloader;
    
    // Font
    HFONT hLargeFont;

    // USB Device
    CUSBDevice* USBPORT;
    bool Connected;

    // Data
    double Klum;
    double Master;
    double AvgLux;
    int timer_tick;
    bool SaveKlum;
    bool SaveMaster;

    // Methods
    void Init();
    bool ConnectTo();
    void ConnectStr();
    void TimerHandle();
    void UpdateDisplay();

    // OPT3001 methods
    double GetOPT(int chip);
    bool SetOPTcfg(int chip);
    int GetOPTreg(int chip, unsigned char Reg);
    bool SetOPTreg(int chip, unsigned char Reg, int Data);
    bool SetBlink(bool blink, unsigned char bst);
    
    // EEPROM methods
    bool PutKlum(unsigned char chip, double dd);
    double GetKlum(unsigned char chip);

    // Utility methods
    const char* VersionString();
    const char* ConnectString();
    void Delay(DWORD ms);

    // Window procedure
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

void d2scistr(char* str, double d);

#endif
