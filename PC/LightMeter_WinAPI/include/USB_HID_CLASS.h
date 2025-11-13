//---------------------------------------------------------------------------
// USB HID CLASS - WinAPI version
//---------------------------------------------------------------------------

#ifndef USB_HID_CLASSH
#define USB_HID_CLASSH

#include <windows.h>

#define USB_BUFFER_SIZE 64

class CUSBDevice
{
public:
    CUSBDevice();
    CUSBDevice(const DWORD vendorID, const DWORD productID, const DWORD versionNumber, HWND hwnd = NULL);
    ~CUSBDevice();

    bool Connect(const DWORD vendorID, const DWORD productID, const DWORD versionNumber, HWND hwnd = NULL);
    bool Connect(void) { return Connect(VID, PID, VER, hWnd); }
    void Disconnect(void);
    
    bool Write(const BYTE* buffer, int nBuffLen);
    int Read(BYTE* buffer, int nBuffLen, int timeout = 100);

    bool Manufacturer(wchar_t* Buffer, int BufSize);
    bool Product(wchar_t* Buffer, int BufSize);

    bool isConnected(void) { return connected; }
    bool transfer(int outnum, unsigned char* out, int innum, unsigned char* in);

protected:
    bool ConnectToIthDevice(HANDLE& deviceHandle, DWORD deviceIndex);

private:
    bool connected;
    char devicePath[4096];
    HDEVNOTIFY DeviceNotificationHandle;
    HANDLE EventObject;
    OVERLAPPED HIDOverlapped;
    HANDLE handle;
    HANDLE WriteHandle;
    HANDLE ReadHandle;
    GUID hidGUID;

    HWND hWnd;
    DWORD VID;
    DWORD PID;
    DWORD VER;
};

#endif
