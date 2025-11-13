//---------------------------------------------------------------------------
// Bootloader Window
// WinAPI version
//---------------------------------------------------------------------------

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>

#include "../include/Bootloader.h"
#include "../include/USB_HID_CLASS.h"
#include "../include/Hex.h"
#include "../include/ccrc.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winmm.lib")

// USB Device IDs for bootloader
#define USB_VID_BOOT 1240
#define USB_PID_BOOT 60
#define USB_VER_BOOT 0

//---------------------------------------------------------------------------

CBootloader::CBootloader()
{
    hWnd = NULL;
    hParent = NULL;
    hInstance = NULL;
    USBPORT = NULL;
    HexManager = NULL;
    Connected = false;
    HexLoad = false;
    ResetHexFilePtr = false;
    ExitThread = TRUE;
    ThreadKilled = FALSE;
    EraseProgVer = false;
    TxPacketLen = 0;
    RxDataLen = 0;
    RetryCount = 0;
    RxFrameValid = false;
    NoResponseFromDevice = false;
    TxState = FIRST_TRY;
    MaxRetry = 0;
    TxRetryDelay = 0;
}

//---------------------------------------------------------------------------

CBootloader::~CBootloader()
{
    if (USBPORT)
    {
        USBPORT->Disconnect();
        delete USBPORT;
    }
    if (HexManager)
        delete HexManager;
}

//---------------------------------------------------------------------------

void CBootloader::Delay(DWORD ms)
{
    timeBeginPeriod(1);
    DWORD time1 = GetTickCount();
    DWORD time2;
    do
    {
        Sleep(1);
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        time2 = GetTickCount();
    } while ((time2 - time1) < ms);
    timeEndPeriod(1);
}

//---------------------------------------------------------------------------

void CBootloader::AppendText(const char* text)
{
    int len = GetWindowTextLengthA(hMemo);
    SendMessageA(hMemo, EM_SETSEL, len, len);
    SendMessageA(hMemo, EM_REPLACESEL, FALSE, (LPARAM)text);
    SendMessageA(hMemo, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
}

//---------------------------------------------------------------------------

unsigned short CBootloader::ReadPort(char* buffer, int bufflen)
{
    if (USBPORT->Read((BYTE*)buffer, bufflen, 100))
        return 64;
    else
        return 0;
}

//---------------------------------------------------------------------------

void CBootloader::WritePort(char* buffer, int bufflen)
{
    while (bufflen > 0)
    {
        USBPORT->Write((BYTE*)buffer, bufflen);
        bufflen -= USB_BUFFER_SIZE;
        buffer += USB_BUFFER_SIZE;
    }
}

//---------------------------------------------------------------------------

void CBootloader::BuildRxFrame(unsigned char* buff, unsigned short buffLen)
{
    static bool Escape = false;
    unsigned short crc;

    while ((buffLen > 0) && (RxFrameValid == false))
    {
        buffLen--;
        if (RxDataLen >= (sizeof(RxData) - 2))
        {
            RxDataLen = 0;
        }

        switch (*buff)
        {
        case SOH: // Start of header
            if (Escape)
            {
                RxData[RxDataLen++] = *buff;
                Escape = FALSE;
            }
            else
            {
                RxDataLen = 0;
            }
            break;

        case EOT: // End of transmission
            if (Escape)
            {
                RxData[RxDataLen++] = *buff;
                Escape = FALSE;
            }
            else
            {
                if (RxDataLen > 1)
                {
                    crc = (RxData[RxDataLen - 2]) & 0x00ff;
                    crc = crc | ((RxData[RxDataLen - 1] << 8) & 0xFF00);
                    if ((CalculateCrc(RxData, (RxDataLen - 2)) == crc) && (RxDataLen > 2))
                    {
                        RxFrameValid = TRUE;
                    }
                }
            }
            break;

        case DLE: // Escape character
            if (Escape)
            {
                RxData[RxDataLen++] = *buff;
                Escape = FALSE;
            }
            else
            {
                Escape = TRUE;
            }
            break;

        default: // Data field
            RxData[RxDataLen++] = *buff;
            Escape = FALSE;
            break;
        }
        buff++;
    }
}

//---------------------------------------------------------------------------

void CBootloader::HandleNoResponse()
{
    switch (LastSentCommand)
    {
    case READ_BOOT_INFO:
    case ERASE_FLASH:
    case PROGRAM_FLASH:
    case READ_CRC:
        AppendText("No Response from the device. Operation failed");
        Buttons();
        break;
    }
}

//---------------------------------------------------------------------------

void CBootloader::HandleResponse()
{
    unsigned char cmd = RxData[0];
    char text[256];

    switch (cmd)
    {
    case READ_BOOT_INFO:
        sprintf_s(text, sizeof(text), "Bootloader Firmware Version: %d.%d", (int)RxData[1], (int)RxData[2]);
        AppendText(text);
        break;

    case ERASE_FLASH:
        AppendText("Flash Erased");
        if (EraseProgVer) // Operation Erase->Program->Verify
        {
            AppendText("programming");
            SendCommand(PROGRAM_FLASH, 3, 500);
        }
        break;

    case READ_CRC:
    {
        unsigned short crc = ((RxData[2] << 8) & 0xFF00) | (RxData[1] & 0x00FF);
        if (crc == CalculateFlashCRC())
            AppendText("Verification successfull");
        else
            AppendText("Verification failed");
        if (EraseProgVer)
            EraseProgVer = false;
    }
    break;

    case PROGRAM_FLASH:
        ResetHexFilePtr = false;
        if (!SendCommand(PROGRAM_FLASH, MaxRetry, TxRetryDelay))
        {
            AppendText("Programming completed");
            if (EraseProgVer)
            {
                AppendText("verifing");
                SendCommand(READ_CRC, 3, 5000);
            }
        }
        ResetHexFilePtr = true;
        break;
    }
}

//---------------------------------------------------------------------------

void CBootloader::ReceiveTask()
{
    unsigned short BuffLen;
    char Buff[255];

    BuffLen = ReadPort((char*)Buff, (sizeof(Buff) - 10));
    BuildRxFrame((unsigned char*)Buff, BuffLen);
    if (RxFrameValid)
    {
        StopTxRetries();
        RxFrameValid = false;
        HandleResponse();
    }
    else
    {
        if (NoResponseFromDevice)
        {
            NoResponseFromDevice = false;
            RxFrameValid = false;
            HandleNoResponse();
        }
    }
}

//---------------------------------------------------------------------------

void CBootloader::StopTxRetries()
{
    TxState = FIRST_TRY;
    RetryCount = 0;
}

//---------------------------------------------------------------------------

void CBootloader::TransmitTask()
{
    static unsigned long NextRetryTimeInMs;

    switch (TxState)
    {
    case FIRST_TRY:
        if (RetryCount)
        {
            WritePort(TxPacket, TxPacketLen);
            RetryCount--;
            TxState = RE_TRY;
            NextRetryTimeInMs = GetTickCount() + TxRetryDelay;
        }
        break;

    case RE_TRY:
        if (RetryCount)
        {
            if (NextRetryTimeInMs < GetTickCount())
            {
                NextRetryTimeInMs = GetTickCount() + TxRetryDelay;
                WritePort(TxPacket, TxPacketLen);
                RetryCount--;
            }
        }
        else
        {
            NoResponseFromDevice = true;
            TxState = FIRST_TRY;
        }
        break;
    }
}

//---------------------------------------------------------------------------

void CBootloader::RxTxThread()
{
    if (ExitThread == FALSE)
    {
        ReceiveTask();
        TransmitTask();
        Sleep(1);
    }
    else
        ThreadKilled = TRUE;
}

//---------------------------------------------------------------------------

unsigned short CBootloader::CalculateFlashCRC()
{
    unsigned int StartAddress, Len;
    unsigned short crc;
    HexManager->VerifyFlash((unsigned int*)&StartAddress, (unsigned int*)&Len, (unsigned short*)&crc);
    return crc;
}

//---------------------------------------------------------------------------

bool CBootloader::SendCommand(char cmd, unsigned short Retries, unsigned short DelayInMs)
{
    unsigned short crc;
    unsigned int StartAddress, Len;
    unsigned short BuffLen = 0;
    unsigned short HexRecLen;
    UINT totalRecords = 10;
    TxPacketLen = 0;

    LastSentCommand = cmd;

    switch ((unsigned char)cmd)
    {
    case READ_BOOT_INFO:
        Buff[BuffLen++] = cmd;
        MaxRetry = RetryCount = Retries;
        TxRetryDelay = DelayInMs;
        break;

    case ERASE_FLASH:
        Buff[BuffLen++] = cmd;
        MaxRetry = RetryCount = Retries;
        TxRetryDelay = DelayInMs;
        break;

    case JMP_TO_APP:
        Buff[BuffLen++] = cmd;
        MaxRetry = RetryCount = 1;
        TxRetryDelay = 10;
        break;

    case PROGRAM_FLASH:
        Buff[BuffLen++] = cmd;
        if (ResetHexFilePtr)
        {
            if (!HexManager->ResetHexFilePointer())
            {
                return false;
            }
        }
        HexRecLen = HexManager->GetNextHexRecord(&Buff[BuffLen], (sizeof(Buff) - 5));
        if (HexRecLen == 0)
        {
            return false;
        }

        BuffLen = BuffLen + HexRecLen;
        while (totalRecords)
        {
            HexRecLen = HexManager->GetNextHexRecord(&Buff[BuffLen], (sizeof(Buff) - 5));
            BuffLen = BuffLen + HexRecLen;
            totalRecords--;
        }
        MaxRetry = RetryCount = Retries;
        TxRetryDelay = DelayInMs;
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
        Buff[BuffLen++] = (char)crc;
        Buff[BuffLen++] = (char)(crc >> 8);
        MaxRetry = RetryCount = Retries;
        TxRetryDelay = DelayInMs;
        break;

    default:
        return false;
    }

    // Calculate CRC for the frame
    crc = CalculateCrc(Buff, BuffLen);
    Buff[BuffLen++] = (char)crc;
    Buff[BuffLen++] = (char)(crc >> 8);

    // SOH: Start of header
    TxPacket[TxPacketLen++] = SOH;

    // Form TxPacket
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

void CBootloader::Buttons()
{
    if (USBPORT->isConnected())
    {
        SetWindowTextA(hBtnConnect, "&Disconnect");
        EnableWindow(hBtnHex, TRUE);
        EnableWindow(hBtnVer, TRUE);
        EnableWindow(hBtnErase, TRUE);

        if (HexLoad)
        {
            EnableWindow(hBtnAll, TRUE);
            EnableWindow(hBtnProgram, TRUE);
            EnableWindow(hBtnRun, TRUE);
            EnableWindow(hBtnVerify, TRUE);
        }
        else
        {
            EnableWindow(hBtnAll, FALSE);
            EnableWindow(hBtnProgram, FALSE);
            EnableWindow(hBtnRun, FALSE);
            EnableWindow(hBtnVerify, FALSE);
        }
    }
    else
    {
        SetWindowTextA(hBtnConnect, "&Connect");
        EnableWindow(hBtnHex, FALSE);
        EnableWindow(hBtnVer, FALSE);
        EnableWindow(hBtnAll, FALSE);
        EnableWindow(hBtnErase, FALSE);
        EnableWindow(hBtnProgram, FALSE);
        EnableWindow(hBtnRun, FALSE);
        EnableWindow(hBtnVerify, FALSE);
    }
}

//---------------------------------------------------------------------------

INT_PTR CALLBACK CBootloader::DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CBootloader* pThis = NULL;

    if (msg == WM_INITDIALOG)
    {
        pThis = (CBootloader*)lParam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->hWnd = hwnd;
    }
    else
    {
        pThis = (CBootloader*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
        return pThis->HandleMessage(msg, wParam, lParam);
    else
        return FALSE;
}

//---------------------------------------------------------------------------

LRESULT CBootloader::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // Controls will be created in ShowModal, just initialize here
        return 0;
    }

    case WM_TIMER:
        if (wParam == IDC_TIMER_BOOT)
        {
            RxTxThread();

            // Update progress bar
            int Pos = 0, Upper = 0;
            switch (LastSentCommand)
            {
            case READ_BOOT_INFO:
            case ERASE_FLASH:
            case READ_CRC:
            case JMP_TO_APP:
                Pos = (MaxRetry - RetryCount);
                Upper = MaxRetry;
                break;

            case PROGRAM_FLASH:
                Pos = HexManager->HexCurrLineNo;
                Upper = HexManager->HexTotalLines;
                break;
            }

            SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, Upper));
            SendMessage(hProgress, PBM_SETPOS, Pos, 0);
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_CONNECT:
        {
            AppendText("Connect button pressed");
            if (!USBPORT->isConnected())
            {
                AppendText("Attempting to connect to device...");
                char debugMsg[256];
                sprintf_s(debugMsg, sizeof(debugMsg), "VID: 0x%04X, PID: 0x%04X, VER: 0x%04X", USB_VID_BOOT, USB_PID_BOOT, USB_VER_BOOT);
                AppendText(debugMsg);
                
                USBPORT->Connect(USB_VID_BOOT, USB_PID_BOOT, USB_VER_BOOT, hWnd);
                
                if (USBPORT->isConnected())
                {
                    SetWindowTextA(hMemo, "");
                    AppendText("Device connected");

                    // Get version
                    wchar_t str[256];
                    if (USBPORT->Product(str, 255))
                    {
                        char product[256];
                        WideCharToMultiByte(CP_ACP, 0, str, -1, product, 256, NULL, NULL);
                        char connectStr[512];
                        sprintf_s(connectStr, sizeof(connectStr), "Connected to %s", product);
                        AppendText(connectStr);
                    }

                    SendCommand(READ_BOOT_INFO, 50, 200);
                    ExitThread = FALSE;
                    SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
                    EraseProgVer = false;
                }
                else
                {
                    AppendText("Failed to connect to device");
                    AppendText("Make sure device is in bootloader mode");
                }
            }
            else
            {
                USBPORT->Disconnect();
                KillTimer(hWnd, IDC_TIMER_BOOT);
                SendMessage(hProgress, PBM_SETPOS, 0, 0);
                AppendText("Device disconnected");
            }
            Buttons();
            return 0;
    }        case IDC_BTN_VER:
        {
            EraseProgVer = false;
            if (USBPORT->isConnected())
            {
                SendCommand(READ_BOOT_INFO, 50, 200);
                ExitThread = FALSE;
            }
            return 0;
        }

        case IDC_BTN_HEX:
        {
            OPENFILENAMEA ofn;
            char szFile[MAX_PATH] = "";

            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "Hex Files (*.hex)\0*.hex\0All Files (*.*)\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrTitle = "Open HEX file";
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            KillTimer(hWnd, IDC_TIMER_BOOT);
            EraseProgVer = false;

            if (GetOpenFileNameA(&ofn))
            {
                HexLoad = HexManager->LoadHexFile(szFile);
                if (HexLoad)
                {
                    char text[256];
                    sprintf_s(text, sizeof(text), "Hex file %d lines loaded successfully", HexManager->HexTotalLines);
                    AppendText(text);
                }
                else
                {
                    AppendText("Hex file not loaded");
                }
                ResetHexFilePtr = HexLoad;
                Buttons();
            }

            SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
            return 0;
        }

        case IDC_BTN_ERASE:
        {
            SendMessage(hProgress, PBM_SETPOS, 0, 0);
            EraseProgVer = false;
            AppendText("erasing");
            SendCommand(ERASE_FLASH, 3, 5000);
            ExitThread = FALSE;
            SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
            return 0;
        }

        case IDC_BTN_PROGRAM:
        {
            SendMessage(hProgress, PBM_SETPOS, 0, 0);
            ResetHexFilePtr = true;
            EraseProgVer = false;
            AppendText("programming");
            if (SendCommand(PROGRAM_FLASH, 3, 5000))
            {
                ExitThread = FALSE;
                SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
            }
            else
            {
                AppendText("Hex file not loaded");
            }
            return 0;
        }

        case IDC_BTN_VERIFY:
        {
            EraseProgVer = false;
            AppendText("verifing");
            if (SendCommand(READ_CRC, 3, 5000))
            {
                ExitThread = FALSE;
                SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
            }
            else
            {
                AppendText("Error");
            }
            return 0;
        }

        case IDC_BTN_ALL:
        {
            EraseProgVer = true;
            AppendText("erasing");
            SendCommand(ERASE_FLASH, 3, 5000);
            ExitThread = FALSE;
            SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
            return 0;
        }

        case IDC_BTN_RUN:
        {
            EraseProgVer = false;
            AppendText("Command issued to run application");
            if (SendCommand(JMP_TO_APP, 1, 10))
            {
                ExitThread = FALSE;
                SetTimer(hWnd, IDC_TIMER_BOOT, 10, NULL);
            }
            else
            {
                AppendText("Error");
            }
            return 0;
        }

        case IDCANCEL:
            if (USBPORT)
            {
                USBPORT->Disconnect();
            }
            KillTimer(hWnd, IDC_TIMER_BOOT);
            DestroyWindow(hWnd);
            return 0;
        }
        break;

    case WM_DEVICECHANGE:
        if (USBPORT && USBPORT->isConnected())
        {
            USBPORT->Connect(USB_VID_BOOT, USB_PID_BOOT, USB_VER_BOOT, hWnd);
            if (!USBPORT->isConnected())
            {
                SendMessage(hProgress, PBM_SETPOS, 0, 0);
                KillTimer(hWnd, IDC_TIMER_BOOT);
                AppendText("Device disconnected");
            }
            Buttons();
        }
        return 0;

    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
    {
        // Check if it's our Memo control
        if ((HWND)lParam == hMemo)
        {
            HDC hdcEdit = (HDC)wParam;
            SetBkColor(hdcEdit, RGB(255, 255, 255));
            SetTextColor(hdcEdit, RGB(0, 0, 0));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }
        break;
    }

    case WM_CLOSE:
        if (USBPORT)
        {
            USBPORT->Disconnect();
        }
        KillTimer(hWnd, IDC_TIMER_BOOT);
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

//---------------------------------------------------------------------------

bool CBootloader::Create(HINSTANCE hInst, HWND hParentWnd)
{
    hInstance = hInst;
    hParent = hParentWnd;
    return true;
}

//---------------------------------------------------------------------------

// Static window procedure wrapper
static LRESULT CALLBACK BootloaderWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CBootloader* pThis = NULL;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (CBootloader*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        if (pThis)
            pThis->hWnd = hwnd;
    }
    else
    {
        pThis = (CBootloader*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
        return pThis->HandleMessage(msg, wParam, lParam);
    else
        return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void CBootloader::ShowModal()
{
    // Create window class
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = BootloaderWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"BootloaderWindow";

    RegisterClassExW(&wc);

    hWnd = CreateWindowExW(
        0,
        L"BootloaderWindow",
        L"Bootloader",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 500,
        hParent, NULL, hInstance, this);

    if (!hWnd)
        return;

    // Create controls
    hMemo = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        10, 10, 560, 200,
        hWnd, (HMENU)IDC_MEMO, hInstance, NULL);

    hProgress = CreateWindowEx(0, PROGRESS_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        10, 220, 560, 25,
        hWnd, (HMENU)IDC_PROGRESS, hInstance, NULL);

    int btnY = 255;
    int btnW = 95;
    int btnH = 25;
    int btnGap = 5;

    hBtnConnect = CreateWindowA("BUTTON", "&Connect",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_CONNECT, hInstance, NULL);

    hBtnVer = CreateWindowA("BUTTON", "&Version",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10 + btnW + btnGap, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_VER, hInstance, NULL);

    hBtnHex = CreateWindowA("BUTTON", "&Hex File",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10 + (btnW + btnGap) * 2, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_HEX, hInstance, NULL);

    btnY += btnH + btnGap;

    hBtnErase = CreateWindowA("BUTTON", "&Erase",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_ERASE, hInstance, NULL);

    hBtnProgram = CreateWindowA("BUTTON", "&Program",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10 + btnW + btnGap, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_PROGRAM, hInstance, NULL);

    hBtnVerify = CreateWindowA("BUTTON", "V&erify",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10 + (btnW + btnGap) * 2, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_VERIFY, hInstance, NULL);

    hBtnAll = CreateWindowA("BUTTON", "&All",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10 + (btnW + btnGap) * 3, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_ALL, hInstance, NULL);

    hBtnRun = CreateWindowA("BUTTON", "&Run",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10 + (btnW + btnGap) * 4, btnY, btnW, btnH,
        hWnd, (HMENU)IDC_BTN_RUN, hInstance, NULL);

    // Initialize
    if (!USBPORT)
        USBPORT = new CUSBDevice();
    if (!HexManager)
        HexManager = new CHexManager();
    SendMessage(hProgress, PBM_SETPOS, 0, 0);
    Buttons();
    
    AppendText("Bootloader initialized");

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!IsDialogMessage(hWnd, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

//---------------------------------------------------------------------------
