//---------------------------------------------------------------------------
// USB HID CLASS - WinAPI version
//---------------------------------------------------------------------------

#include <windows.h>
#include <setupapi.h>
#include <Dbt.h>

extern "C"
{
    #include <hidsdi.h>
}

#include "../include/USB_HID_CLASS.h"

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

//---------------------------------------------------------------------------

CUSBDevice::CUSBDevice()
{
    connected = false;
    handle = INVALID_HANDLE_VALUE;
    WriteHandle = INVALID_HANDLE_VALUE;
    ReadHandle = INVALID_HANDLE_VALUE;
    EventObject = INVALID_HANDLE_VALUE;
    DeviceNotificationHandle = NULL;
    memset(devicePath, 0, sizeof(devicePath));
}

//---------------------------------------------------------------------------

CUSBDevice::~CUSBDevice()
{
    Disconnect();
}

//---------------------------------------------------------------------------

CUSBDevice::CUSBDevice(const DWORD vendorID, const DWORD productID, const DWORD versionNumber, HWND hwnd)
{
    handle = INVALID_HANDLE_VALUE;
    EventObject = INVALID_HANDLE_VALUE;
    DeviceNotificationHandle = NULL;
    WriteHandle = INVALID_HANDLE_VALUE;
    ReadHandle = INVALID_HANDLE_VALUE;
    memset(devicePath, 0, sizeof(devicePath));
    Connect(vendorID, productID, versionNumber, hwnd);
}

//---------------------------------------------------------------------------

bool CUSBDevice::ConnectToIthDevice(HANDLE& deviceHandle, DWORD deviceIndex)
{
    HDEVINFO hardwareDeviceInfoSet;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A deviceDetail;
    ULONG requiredSize = 0;
    deviceHandle = INVALID_HANDLE_VALUE;
    DWORD result;

    // Get the HID GUID value
    HidD_GetHidGuid(&hidGUID);

    // Get a list of devices matching the criteria
    hardwareDeviceInfoSet = SetupDiGetClassDevs(&hidGUID,
        NULL,
        NULL,
        (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

    if (hardwareDeviceInfoSet == INVALID_HANDLE_VALUE)
        return false;

    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    // Go through the list and get the interface data
    result = SetupDiEnumDeviceInterfaces(hardwareDeviceInfoSet,
        NULL,
        &hidGUID,
        deviceIndex,
        &deviceInterfaceData);

    if (result == FALSE)
    {
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfoSet);
        return false;
    }

    // Get the details with null values to get the required size of the buffer
    SetupDiGetDeviceInterfaceDetailA(hardwareDeviceInfoSet,
        &deviceInterfaceData,
        NULL,
        0,
        &requiredSize,
        0);

    // Allocate the buffer
    deviceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(requiredSize);
    if (deviceDetail == NULL)
    {
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfoSet);
        return false;
    }

    deviceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

    // Fill the buffer with the device details
    result = SetupDiGetDeviceInterfaceDetailA(hardwareDeviceInfoSet,
        &deviceInterfaceData,
        deviceDetail,
        requiredSize,
        &requiredSize,
        NULL);

    if (!result)
    {
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfoSet);
        free(deviceDetail);
        return false;
    }

    // Copy device path
    strcpy_s(devicePath, sizeof(devicePath), deviceDetail->DevicePath);

    // Open file on the device
    deviceHandle = CreateFileA(deviceDetail->DevicePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL);

    SetupDiDestroyDeviceInfoList(hardwareDeviceInfoSet);
    free(deviceDetail);
    return true;
}

//---------------------------------------------------------------------------

bool CUSBDevice::Connect(const DWORD vendorID, const DWORD productID, const DWORD versionNumber, HWND hwnd)
{
    Disconnect();
    handle = INVALID_HANDLE_VALUE;
    DWORD index = 0;
    HIDD_ATTRIBUTES deviceAttributes;
    BOOL matched = FALSE;
    DWORD ErrorStatus;
    DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;

    VID = vendorID;
    PID = productID;
    VER = versionNumber;
    hWnd = hwnd;
    connected = false;

    while (!matched && ConnectToIthDevice(handle, index))
    {
        if (handle != INVALID_HANDLE_VALUE)
        {
            deviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
            if (!HidD_GetAttributes(handle, &deviceAttributes))
            {
                CloseHandle(handle);
                handle = INVALID_HANDLE_VALUE;
                index++;
                continue;
            }

            CloseHandle(handle);
            handle = INVALID_HANDLE_VALUE;

            if ((vendorID == 0 || (unsigned)deviceAttributes.VendorID == vendorID) &&
                (productID == 0 || (unsigned)deviceAttributes.ProductID == productID) &&
                (versionNumber == 0 || (unsigned)deviceAttributes.VersionNumber == versionNumber))
            {
                // Open write handle
                WriteHandle = CreateFileA(devicePath, GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, OPEN_EXISTING, 0, NULL);
                ErrorStatus = GetLastError();
                if (ErrorStatus != ERROR_SUCCESS)
                {
                    index++;
                    continue;
                }

                // Open read handle with overlapped I/O
                ReadHandle = CreateFileA(devicePath, GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
                ErrorStatus = GetLastError();
                if (ErrorStatus != ERROR_SUCCESS)
                {
                    CloseHandle(WriteHandle);
                    WriteHandle = INVALID_HANDLE_VALUE;
                    index++;
                    continue;
                }

                // Create event object for overlapped I/O
                EventObject = CreateEvent(NULL, FALSE, TRUE, NULL);

                // Set the members of the overlapped structure
                memset(&HIDOverlapped, 0, sizeof(OVERLAPPED));
                HIDOverlapped.hEvent = EventObject;

                // Register device change notification
                if (hwnd)
                {
                    DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
                    DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
                    DevBroadcastDeviceInterface.dbcc_classguid = hidGUID;
                    DeviceNotificationHandle = RegisterDeviceNotification(hwnd,
                        &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
                }

                connected = true;
                return true;
            }
        }
        index++;
        if (index == 10000) return false; // Safety limit
    }
    return false;
}

//---------------------------------------------------------------------------

void CUSBDevice::Disconnect(void)
{
    if (EventObject != INVALID_HANDLE_VALUE)
    {
        CloseHandle(EventObject);
        EventObject = INVALID_HANDLE_VALUE;
    }

    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }

    if (WriteHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(WriteHandle);
        WriteHandle = INVALID_HANDLE_VALUE;
    }

    if (ReadHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(ReadHandle);
        ReadHandle = INVALID_HANDLE_VALUE;
    }

    if (DeviceNotificationHandle != NULL)
    {
        UnregisterDeviceNotification(DeviceNotificationHandle);
        DeviceNotificationHandle = NULL;
    }

    connected = false;
}

//---------------------------------------------------------------------------

bool CUSBDevice::Manufacturer(wchar_t* Buffer, int BufSize)
{
    if (!connected) return false;
    memset(Buffer, 0, BufSize);
    return HidD_GetManufacturerString(ReadHandle, Buffer, BufSize);
}

//---------------------------------------------------------------------------

bool CUSBDevice::Product(wchar_t* Buffer, int BufSize)
{
    if (!connected) return false;
    memset(Buffer, 0, BufSize);
    return HidD_GetProductString(ReadHandle, Buffer, BufSize);
}

//---------------------------------------------------------------------------

bool CUSBDevice::Write(const BYTE* buffer, int nBuffLen)
{
    unsigned long BytesWritten;
    BYTE UsbReport[USB_BUFFER_SIZE + 1];

    memset(UsbReport, 0xff, sizeof(UsbReport));
    UsbReport[0] = 0; // Report ID = 0

    if (nBuffLen > USB_BUFFER_SIZE)
        memcpy(&UsbReport[1], buffer, USB_BUFFER_SIZE);
    else
        memcpy(&UsbReport[1], buffer, nBuffLen);

    BOOL res = WriteFile(WriteHandle, (LPCVOID)UsbReport, USB_BUFFER_SIZE + 1, &BytesWritten, NULL);
    connected = (res != 0);
    return (res != 0);
}

//---------------------------------------------------------------------------

int CUSBDevice::Read(BYTE* buffer, int nBuffLen, int timeout)
{
    unsigned long BytesRead = 0;
    BYTE UsbReport[USB_BUFFER_SIZE + 1];

    memset(UsbReport, 0, sizeof(UsbReport));

    ReadFile(ReadHandle, (LPVOID)UsbReport, USB_BUFFER_SIZE + 1, &BytesRead, (LPOVERLAPPED)&HIDOverlapped);

    DWORD Result = WaitForSingleObject(EventObject, timeout);
    if (Result == WAIT_OBJECT_0)
    {
        ResetEvent(EventObject);
        if (nBuffLen > USB_BUFFER_SIZE)
        {
            memcpy(buffer, &UsbReport[1], USB_BUFFER_SIZE);
            return USB_BUFFER_SIZE;
        }
        else
        {
            memcpy(buffer, &UsbReport[1], nBuffLen);
            return nBuffLen;
        }
    }
    else
    {
        CancelIo(ReadHandle);
        ResetEvent(EventObject);
        return 0;
    }
}

//---------------------------------------------------------------------------

bool CUSBDevice::transfer(int outnum, unsigned char* out, int innum, unsigned char* in)
{
    if (connected)
    {
        if (innum && outnum)
        {
            if (Write(out, outnum))
                Read(in, innum);
        }
    }
    else
    {
        Connect(VID, PID, VER);
    }
    return connected;
}

//---------------------------------------------------------------------------
