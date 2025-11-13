//---------------------------------------------------------------------------
// Main Window - Light Meter
// WinAPI version
//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>

#include "../include/LightMeter.h"
#include "../include/USB_HID_CLASS.h"
#include "../include/Bootloader.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winmm.lib")

//---------------------------------------------------------------------------
// Convert to engineering format (1e3 = 1k)
//---------------------------------------------------------------------------
void d2scistr(char* str, double d)
{
    const char csci[] = { 'y', 'z', 'a', 'f', 'p', 'n', 'u', 'm', ' ',
                         'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' };

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
        sprintf_s(str, 64, "%.2f", d);
    else if ((rng >= 0) && (rng < 17))
        sprintf_s(str, 64, "%.2f%c", dd, csci[rng]);
    else
        sprintf_s(str, 64, "%.2e", d);
}

//---------------------------------------------------------------------------
// Union for double to bytes conversion
//---------------------------------------------------------------------------
typedef union udbl
{
    double d;
    unsigned char cd[sizeof(double)];
} tudbl;

//---------------------------------------------------------------------------

CLightMeter::CLightMeter()
{
    hWnd = NULL;
    hInstance = NULL;
    USBPORT = NULL;
    Connected = false;
    Klum = -1.0;
    Master = -1.0;
    AvgLux = 0;
    timer_tick = 0;
    SaveKlum = false;
    SaveMaster = false;
    hLargeFont = NULL;
}

//---------------------------------------------------------------------------

CLightMeter::~CLightMeter()
{
    if (USBPORT)
    {
        USBPORT->Disconnect();
        delete USBPORT;
    }
    if (hLargeFont)
    {
        DeleteObject(hLargeFont);
    }
}

//---------------------------------------------------------------------------

void CLightMeter::Delay(DWORD ms)
{
    timeBeginPeriod(1);
    DWORD time1 = GetTickCount();
    DWORD time2;
    do
    {
        Sleep(1);
        time2 = GetTickCount();
    } while ((time2 - time1) < ms);
    timeEndPeriod(1);
}

//---------------------------------------------------------------------------

void CLightMeter::Init()
{
    SetWindowTextA(hLabelConnect, "Not connected");
    EnableWindow(hLabelConnect, FALSE);
    SetWindowTextA(hLabelLux, "");
    SetWindowTextA(hLabelLum, "");
    SetWindowTextA(hLabelVersion, "");
    EnableWindow(hEditKlux, FALSE);
    EnableWindow(hEditMaster, FALSE);
    Klum = -1.0;
    Master = -1.0;
    timer_tick = 0;
    AvgLux = 0;
}

//---------------------------------------------------------------------------

const char* CLightMeter::VersionString()
{
    static char version[256];
    unsigned char cmd[1];
    unsigned char res[64];

    cmd[0] = 20; // GetVer
    Delay(2);
    if (!USBPORT->transfer(1, cmd, 63, res))
        return "";

    sprintf_s(version, sizeof(version), "%s", &res[1]);
    return version;
}

//---------------------------------------------------------------------------

const char* CLightMeter::ConnectString()
{
    static char connectStr[512];
    wchar_t str[256];

    strcpy_s(connectStr, sizeof(connectStr), "Not connected");
    if (!USBPORT->Product(str, 255))
        return connectStr;

    sprintf_s(connectStr, sizeof(connectStr), "Connected to ");
    char product[256];
    WideCharToMultiByte(CP_ACP, 0, str, -1, product, 256, NULL, NULL);
    strcat_s(connectStr, sizeof(connectStr), product);
    strcat_s(connectStr, sizeof(connectStr), " (c) ");

    if (!USBPORT->Manufacturer(str, 255))
        return connectStr;

    char manufacturer[256];
    WideCharToMultiByte(CP_ACP, 0, str, -1, manufacturer, 256, NULL, NULL);
    strcat_s(connectStr, sizeof(connectStr), manufacturer);

    return connectStr;
}

//---------------------------------------------------------------------------

void CLightMeter::ConnectStr()
{
    Connected = USBPORT->isConnected();
    if (Connected)
        SetWindowTextA(hLabelConnect, ConnectString());
    else
        SetWindowTextA(hLabelConnect, "Not connected");
}

//---------------------------------------------------------------------------

bool CLightMeter::ConnectTo()
{
    if (!USBPORT->isConnected())
        USBPORT->Connect(USB_VID, USB_PID, USB_VER, hWnd);

    if (USBPORT->isConnected())
    {
        EnableWindow(hLabelConnect, TRUE);
        EnableWindow(hEditKlux, TRUE);
        EnableWindow(hEditMaster, TRUE);
        return true;
    }
    else
    {
        Init();
        return false;
    }
}

//---------------------------------------------------------------------------
// Get light data from OPT3001
//---------------------------------------------------------------------------
double CLightMeter::GetOPT(int chip)
{
    unsigned char cmd[8];
    unsigned char res[8];

    cmd[0] = 15;         // srd_11
    cmd[1] = 0x80;       // Ctl (I2C address of the OPT3001 = 0x88)
    cmd[2] = 0x00;       // Register Addr
    cmd[3] = 4 + (chip & 3); // Addr
    cmd[4] = 2;          // Size

    Delay(2);
    if (USBPORT->transfer(5, cmd, 2 + 1, res) && (res[0] == cmd[0]))
    {
        int data = res[1] * 256 + res[2];
        int mantissa = data & 0x0fff;
        int exponent = (data & 0xf000) >> 12;
        double Res = 0.01 * mantissa * (1 << exponent);
        Delay(10);
        return Res;
    }
    else
        return -1.0;
}

//---------------------------------------------------------------------------
// Get 16 bit register from OPT3001
//---------------------------------------------------------------------------
int CLightMeter::GetOPTreg(int chip, unsigned char Reg)
{
    unsigned char cmd[8];
    unsigned char res[8];

    cmd[0] = 15;
    cmd[1] = 0x80;
    cmd[2] = Reg;
    cmd[3] = 4 + (chip & 3);
    cmd[4] = 2;

    Delay(2);
    if (USBPORT->transfer(5, cmd, 2 + 1, res) && (res[0] == cmd[0]))
    {
        int data = res[1] * 256 + res[2];
        Delay(10);
        return data;
    }
    else
        return -1;
}

//---------------------------------------------------------------------------
// Set 16 bit register of OPT3001
//---------------------------------------------------------------------------
bool CLightMeter::SetOPTreg(int chip, unsigned char Reg, int Data)
{
    unsigned char cmd[8];
    unsigned char res[8];

    Data &= 0xffff;
    cmd[0] = 14;
    cmd[1] = 0x80;
    cmd[2] = Reg;
    cmd[3] = 4 + (chip & 3);
    cmd[4] = 2;
    cmd[5] = Data / 256;
    cmd[6] = Data % 256;

    Delay(2);
    if (USBPORT->transfer(7, cmd, 1, res) && (res[0] == cmd[0]))
    {
        Delay(10);
        return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------
// Setup OPT3001
//---------------------------------------------------------------------------
bool CLightMeter::SetOPTcfg(int chip)
{
    unsigned char cmd[8];
    unsigned char res[8];

    int CFG =
        (0x0c << 12) + // RN[3:0]   Range number field (automatic full-scale-range)
        (0x01 << 11) + // CT        Conversion time field (800 ms)
        (0x02 << 9) +  // M[1:0]    Mode of conversion operation field (Continuous conversions)
        (0x00 << 8) +  // OVF       Overflow flag field (read-only)
        (0x00 << 7) +  // CRF       Conversion ready field (read-only)
        (0x00 << 6) +  // FH        Flag high field (read-only)
        (0x00 << 5) +  // FL        Flag low field (read-only)
        (0x00 << 4) +  // L         Latch field
        (0x00 << 3) +  // POL       Polarity field
        (0x00 << 2) +  // ME        Mask exponent field
        (0x00 << 0);   // FC[1:0]   Fault count field

    cmd[0] = 14;
    cmd[1] = 0x80;
    cmd[2] = 0x01;
    cmd[3] = 4 + (chip & 3);
    cmd[4] = 2;
    cmd[5] = CFG / 256;
    cmd[6] = CFG % 256;

    Delay(2);
    if (USBPORT->transfer(7, cmd, 1, res) && (res[0] == cmd[0]))
    {
        Delay(10);
        return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------
// LEDs indicator control
//---------------------------------------------------------------------------
bool CLightMeter::SetBlink(bool blink, unsigned char bst)
{
    unsigned char cmd[8];
    unsigned char res[8];

    cmd[0] = 21;
    cmd[1] = blink ? 1 : 0;
    cmd[2] = bst;

    Delay(2);
    if (USBPORT->transfer(3, cmd, 1, res) && (res[0] == cmd[0]))
    {
        Delay(10);
        return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------
// Put double value to EEPROM
//---------------------------------------------------------------------------
bool CLightMeter::PutKlum(unsigned char chip, double dd)
{
    unsigned char cmd[5 + sizeof(tudbl)];
    unsigned char res[8];
    tudbl d0;

    int eeAddr = chip * 2 * sizeof(tudbl);
    unsigned char size = sizeof(tudbl);

    cmd[0] = 14;
    cmd[1] = 0xa0;
    cmd[2] = eeAddr % 256;
    cmd[3] = eeAddr / 256;
    cmd[4] = size;

    Delay(2);

    d0.d = dd;
    for (int i = 0; i < sizeof(tudbl); i++)
        cmd[i + 5] = d0.cd[i];

    if (USBPORT->transfer(size + 5, cmd, 1, res) && (res[0] == cmd[0]))
    {
        Delay(10);

        eeAddr = (chip * 2 + 1) * sizeof(tudbl);
        cmd[2] = eeAddr % 256;
        cmd[3] = eeAddr / 256;
        for (int i = 0; i < sizeof(tudbl); i++)
            cmd[i + 5] = (d0.cd[i] + 85) & 255;

        if (USBPORT->transfer(size + 5, cmd, 1, res) && (res[0] == cmd[0]))
        {
            Delay(10);
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
// Get double value from EEPROM
//---------------------------------------------------------------------------
double CLightMeter::GetKlum(unsigned char chip)
{
    unsigned char cmd[8];
    unsigned char res[2 * sizeof(tudbl) + 2];

    int eeAddr = chip * 2 * sizeof(tudbl);
    unsigned char size = 2 * sizeof(tudbl);

    cmd[0] = 15;
    cmd[1] = 0xa0;
    cmd[2] = eeAddr % 256;
    cmd[3] = eeAddr / 256;
    cmd[4] = size;

    Delay(2);
    if (USBPORT->transfer(5, cmd, size + 1, res) && (res[0] == cmd[0]))
    {
        tudbl d0, d1;
        unsigned char chk = 0;
        for (int i = 0; i < sizeof(tudbl); i++)
            d0.cd[i] = res[i + 1];
        for (int i = 0; i < sizeof(tudbl); i++)
            d1.cd[i] = res[i + 1 + sizeof(tudbl)];
        for (int i = 0; i < sizeof(tudbl); i++)
            chk += (d0.cd[i] - d1.cd[i] + 85) & 255;

        Delay(10);
        if (chk)
            return -1.0;
        else
            return d0.d;
    }
    else
        return -1.0;
}

//---------------------------------------------------------------------------
// Main timer handler
//---------------------------------------------------------------------------
void CLightMeter::TimerHandle()
{
    double lux = -1.0;
    double lum = -1.0;
    char str[64];

    if (SaveKlum)
    {
        SaveKlum = false;
        if ((Klum > 0) && PutKlum(0, Klum))
            SetWindowTextA(hLabelKluxOk, "Ok");
        return;
    }

    if (SaveMaster)
    {
        SaveMaster = false;
        if ((Master > 0) && PutKlum(4, Master))
            SetWindowTextA(hLabelMasterOk, "Ok");
    }

    if (GetWindowTextLengthA(hLabelVersion) == 0)
    {
        SetWindowTextA(hLabelVersion, VersionString());
        return;
    }

    if (timer_tick++ % 10 == 0) // Check each 1s
    {
        if (Klum < 0)
        {
            Klum = GetKlum(0);
            sprintf_s(str, sizeof(str), "%.8g", Klum);
            SetWindowTextA(hEditKlux, str);
        }
        if (Master < 0)
        {
            Master = GetKlum(4);
            if (Master < 0)
                Master = 1.0;
            sprintf_s(str, sizeof(str), "%.8g", Master);
            SetWindowTextA(hEditMaster, str);
        }
    }

    int oSt = GetOPTreg(0, 1); // Read Configuration Register
    if (oSt < 0)
    {
        SetWindowTextA(hLabelLux, "");
        SetWindowTextA(hLabelLum, "");
        SetBlink(true, 1);
    }
    else if (((oSt & 0xfe00) != 0xcc00) && SetOPTreg(0, 1, 0xcc00))
        SetBlink(false, 0);
    else if (oSt & (1 << 7)) // Conversion ready flag
    {
        lux = Master * GetOPT(0);
        if (lux >= 0)
        {
            // Low pass filter
            if (fabs(lux - AvgLux) > 0.1 * lux)
                AvgLux = lux;
            else
                AvgLux += 0.05 * (lux - AvgLux);

            d2scistr(str, AvgLux);
            char luxStr[128];
            if (oSt & (1 << 8)) // Overflow flag
                sprintf_s(luxStr, sizeof(luxStr), "Lx0: >%sLux", str);
            else
                sprintf_s(luxStr, sizeof(luxStr), "Lx0: %sLux", str);
            SetWindowTextA(hLabelLux, luxStr);
            SendMessage(hLabelLux, WM_SETFONT, (WPARAM)hLargeFont, TRUE);

            if (Klum > 0)
            {
                lum = Klum * AvgLux;
                d2scistr(str, lum);
                char lumStr[128];
                if (oSt & (1 << 8)) // Overflow flag
                    sprintf_s(lumStr, sizeof(lumStr), "Lm0: >%sLum", str);
                else
                    sprintf_s(lumStr, sizeof(lumStr), "Lm0: %sLum", str);
                SetWindowTextA(hLabelLum, lumStr);
                SendMessage(hLabelLum, WM_SETFONT, (WPARAM)hLargeFont, TRUE);
            }
        }
    }
}

//---------------------------------------------------------------------------
// Window procedure
//---------------------------------------------------------------------------
LRESULT CALLBACK CLightMeter::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CLightMeter* pThis = NULL;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (CLightMeter*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->hWnd = hwnd;
    }
    else
    {
        pThis = (CLightMeter*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
        return pThis->HandleMessage(msg, wParam, lParam);
    else
        return DefWindowProc(hwnd, msg, wParam, lParam);
}

//---------------------------------------------------------------------------

LRESULT CLightMeter::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // Create controls
        hLabelConnect = CreateWindowA("STATIC", "Not connected",
            WS_CHILD | WS_VISIBLE,
            10, 10, 400, 20,
            hWnd, (HMENU)IDC_LABEL_CONNECT, hInstance, NULL);

        hLabelVersion = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE,
            10, 35, 400, 20,
            hWnd, (HMENU)IDC_LABEL_VERSION, hInstance, NULL);

        hLabelLux = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            135, 60, 200, 30,
            hWnd, (HMENU)IDC_LABEL_LUX, hInstance, NULL);

        hLabelLum = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            135, 90, 400, 30,
            hWnd, (HMENU)IDC_LABEL_LUM, hInstance, NULL);

        CreateWindowA("STATIC", "Lumen multiplier:",
            WS_CHILD | WS_VISIBLE,
            10, 125, 120, 20,
            hWnd, (HMENU)IDC_LABEL_KLUX, hInstance, NULL);

        hEditKlux = CreateWindowA("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            135, 125, 100, 20,
            hWnd, (HMENU)IDC_EDIT_KLUX, hInstance, NULL);

        hLabelKluxOk = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE,
            245, 125, 50, 20,
            hWnd, (HMENU)IDC_LABEL_KLUX_OK, hInstance, NULL);

        CreateWindowA("STATIC", "Master multiplier:",
            WS_CHILD | WS_VISIBLE,
            10, 155, 120, 20,
            hWnd, (HMENU)IDC_LABEL_MASTER, hInstance, NULL);

        hEditMaster = CreateWindowA("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            135, 155, 100, 20,
            hWnd, (HMENU)IDC_EDIT_MASTER, hInstance, NULL);

        hLabelMasterOk = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE,
            245, 155, 50, 20,
            hWnd, (HMENU)IDC_LABEL_MASTER_OK, hInstance, NULL);

        hButtonBootloader = CreateWindowA("BUTTON", "Bootloader",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 190, 100, 25,
            hWnd, (HMENU)IDC_BUTTON_BOOTLOADER, hInstance, NULL);
        
        // Create larger font for measurement labels
        hLargeFont = CreateFontA(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
        SendMessage(hLabelLux, WM_SETFONT, (WPARAM)hLargeFont, TRUE);
        SendMessage(hLabelLum, WM_SETFONT, (WPARAM)hLargeFont, TRUE);

        // Initialize USB
        USBPORT = new CUSBDevice();
        Init();
        ConnectTo();
        ConnectStr();

        // Start timer (100ms interval)
        SetTimer(hWnd, IDC_TIMER, 100, NULL);
        return 0;
    }

    case WM_TIMER:
        if (wParam == IDC_TIMER)
        {
            if (ConnectTo())
            {
                ConnectStr();
                TimerHandle();
            }
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_BOOTLOADER:
        {
            // Switch device to bootloader mode
            KillTimer(hWnd, IDC_TIMER);
            
            if (USBPORT && USBPORT->isConnected())
            {
                unsigned char cmd = 25; // Switch to bootloader mode
                USBPORT->Write(&cmd, 1);
                Sleep(100); // Give device time to reboot
            }
            
            // Open bootloader window
            CBootloader bootloader;
            bootloader.Create(hInstance, hWnd);
            bootloader.ShowModal();
            
            // Restart timer after bootloader closes
            SetTimer(hWnd, IDC_TIMER, 100, NULL);
            return 0;
        }

        case IDC_EDIT_KLUX:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                char text[64];
                GetWindowTextA(hEditKlux, text, sizeof(text));
                double val;
                if (sscanf_s(text, "%lf", &val) == 1)
                {
                    Klum = val;
                    SaveKlum = true;
                    SetWindowTextA(hLabelKluxOk, "");
                }
            }
            return 0;

        case IDC_EDIT_MASTER:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                char text[64];
                GetWindowTextA(hEditMaster, text, sizeof(text));
                double val;
                if (sscanf_s(text, "%lf", &val) == 1)
                {
                    Master = val;
                    SaveMaster = true;
                    SetWindowTextA(hLabelMasterOk, "");
                }
            }
            return 0;
        }
        break;

    case WM_DEVICECHANGE:
        // Handle device connect/disconnect
        ConnectTo();
        ConnectStr();
        return 0;

    case WM_DESTROY:
        KillTimer(hWnd, IDC_TIMER);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//---------------------------------------------------------------------------

bool CLightMeter::Create(HINSTANCE hInst)
{
    hInstance = hInst;

    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"LightMeterWindow";
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(101));

    if (!RegisterClassExW(&wc))
        return false;

    hWnd = CreateWindowExW(
        0,
        L"LightMeterWindow",
        L"OPT3001 Light Meter",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        420, 280,
        NULL, NULL, hInstance, this);

    return (hWnd != NULL);
}

//---------------------------------------------------------------------------

void CLightMeter::Show()
{
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

//---------------------------------------------------------------------------

void CLightMeter::MessageLoop()
{
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

//---------------------------------------------------------------------------
