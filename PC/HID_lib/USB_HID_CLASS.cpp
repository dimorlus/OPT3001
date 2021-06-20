//---------------------------------------------------------------------------
#include <windows.h>
#include <setupapi.h>
#include <Dbt.h>
extern "C"
{
#include "hidsdi.h"
}
#pragma hdrstop
#pragma warn -8027
#pragma warn -8004

#include "USB_HID_CLASS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma option -a1

#define USB_BUFFER_SIZE         64

#define _OVP_


#define _DYN_DLL_ //Dynamic DLL load
#ifdef _DYN_DLL_
typedef void (WINAPI* t_HidD_GetHidGuid)( OUT LPGUID );
typedef BOOLEAN (WINAPI* t_HidD_GetManufacturerString)(IN HANDLE, OUT PVOID, IN ULONG);
typedef BOOLEAN (WINAPI* t_HidD_GetProductString)(IN HANDLE, OUT PVOID, IN ULONG);
typedef BOOLEAN (WINAPI* t_HidD_GetFeature)(IN HANDLE, OUT PVOID, IN ULONG);
typedef BOOLEAN (WINAPI* t_HidD_SetFeature)(IN HANDLE, IN PVOID, IN ULONG);

//typedef BOOLEAN (WINAPI* t_HidD_GetPreparsedData)(IN HANDLE, IN PVOID);
//typedef BOOLEAN (WINAPI* t_HidP_GetCaps)(IN HANDLE, IN PVOID);
//typedef BOOLEAN (WINAPI* t_HidD_FreePreparsedData)(IN PVOID);


t_HidD_GetHidGuid HidD_GetHidGuid_dll;
t_HidD_GetManufacturerString HidD_GetManufacturerString_dll;
t_HidD_GetProductString HidD_GetProductString_dll;
t_HidD_GetFeature HidD_GetFeature_dll;
t_HidD_SetFeature HidD_SetFeature_dll;

//t_HidD_GetPreparsedData HidD_GetPreparsedData_dll;
//t_HidP_GetCaps HidP_GetCaps_dll;
//t_HidD_FreePreparsedData HidD_FreePreparsedData_dll
#endif
//---------------------------------------------------------------------------

CUSBDevice::CUSBDevice()
 {
#ifdef _DYN_DLL_
  HINSTANCE  hDLL = LoadLibrary("HID.DLL");
  if(hDLL != NULL)
  {
    HidD_GetHidGuid_dll = (t_HidD_GetHidGuid)GetProcAddress(hDLL, "HidD_GetHidGuid");
    HidD_GetManufacturerString_dll = (t_HidD_GetManufacturerString)GetProcAddress(hDLL, "HidD_GetManufacturerString");
    HidD_GetProductString_dll = (t_HidD_GetProductString) GetProcAddress(hDLL, "HidD_GetProductString");
    HidD_GetFeature_dll = (t_HidD_GetFeature) GetProcAddress(hDLL, "HidD_GetFeature");
    HidD_SetFeature_dll = (t_HidD_SetFeature) GetProcAddress(hDLL, "HidD_SetFeature");

    //HidD_GetPreparsedData_dll = (t_HidD_GetPreparsedData) GetProcAddress(hDLL, "HidD_GetPreparsedData");
    //HidD_GetCaps_dll = (t_HidD_GetCaps) GetProcAddress(hDLL, "HidD_GetCaps");
    //HidD_FreePreparsedData_dll = (t_HidD_FreePreparsedData) GetProcAddress(hDLL, "FreePreparsedData");

  }
#endif

  connected = false;
  handle = INVALID_HANDLE_VALUE;
  WriteHandle = INVALID_HANDLE_VALUE;
  ReadHandle = INVALID_HANDLE_VALUE;
  EventObject = INVALID_HANDLE_VALUE;
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
  DeviceNotificationHandle = INVALID_HANDLE_VALUE;
  Connect(vendorID, productID, versionNumber, hwnd);
 }
//---------------------------------------------------------------------------

bool CUSBDevice::ConnectToIthDevice(HANDLE &deviceHandle, DWORD deviceIndex)
 {
  //GUID hidGUID;
  HDEVINFO hardwareDeviceInfoSet;
  SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
  PSP_INTERFACE_DEVICE_DETAIL_DATA deviceDetail;
  ULONG requiredSize = 0;
  deviceHandle = INVALID_HANDLE_VALUE;
  DWORD result;


  //Get the HID GUID value - used as mask to get list of devices
  try
   {
#ifdef _DYN_DLL_
    HidD_GetHidGuid_dll(&hidGUID);
#else
    HidD_GetHidGuid(&hidGUID);
#endif
   }
  catch(...)
   {
    return false;
   }

  //Get a list of devices matching the criteria (hid interface, present)
  try
   {
    hardwareDeviceInfoSet = SetupDiGetClassDevs(&hidGUID,
                                                NULL, // Define no enumerator (global)
                                                NULL, // Define no
                                                (DIGCF_PRESENT | // Only Devices present
                                                DIGCF_DEVICEINTERFACE)); // Function class devices.
   }
  catch(...)
   {
    return false;
   }

  deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

  //Go through the list and get the interface data
  try
   {
    result = SetupDiEnumDeviceInterfaces(hardwareDeviceInfoSet,
                                         NULL, //infoData,
                                         &hidGUID, //interfaceClassGuid,
                                         deviceIndex,
                                         &deviceInterfaceData);
    /* Failed to get a device - possibly the index is larger than the number of devices */
    if (result == FALSE)
     {
      SetupDiDestroyDeviceInfoList(hardwareDeviceInfoSet);
      return false;
     }
   }
  catch(...)
   {
    return false;
   }

  //Get the details with null values to get the required size of the buffer
  try
   {
    SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfoSet,
                                    &deviceInterfaceData,
                                    NULL, //interfaceDetail,
                                    0, //interfaceDetailSize,
                                    &requiredSize,
                                    0); //infoData))
   }
  catch(...)
   {
    return false;
   }

  //Allocate the buffer
  try
   {
    deviceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(requiredSize);
   }
  catch(...)
   {
    return false;
   }
  deviceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

  //Fill the buffer with the device details
  try
   {
    result = SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfoSet,
                                             &deviceInterfaceData,
                                             deviceDetail,
                                             requiredSize,
                                             &requiredSize,
                                             NULL);
   }
  catch(...)
   {
    result = FALSE;
   }

  if (!result)
   {
    SetupDiDestroyDeviceInfoList (hardwareDeviceInfoSet);
    free (deviceDetail);
    return false;
   }

  strcpy(devicePath, deviceDetail->DevicePath);

  //Open file on the device
  try
   {
    deviceHandle = CreateFile(deviceDetail->DevicePath,
                              GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL,        // no SECURITY_ATTRIBUTES structure
                              OPEN_EXISTING, // No special create flags
                              FILE_FLAG_OVERLAPPED,
                              NULL);       // No template file
   }
  catch(...)
   {
   }
  SetupDiDestroyDeviceInfoList(hardwareDeviceInfoSet);
  free (deviceDetail);
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
 DEV_BROADCAST_DEVICEINTERFACE  DevBroadcastDeviceInterface;

 VID = vendorID;
 PID = productID;
 VER = versionNumber;
 hWnd = hwnd;
 connected = false;
 //HidD_GetHidGuid(&HidGuid);
 while (!matched && ConnectToIthDevice(handle, index))
 {
  if (handle != INVALID_HANDLE_VALUE)
    {
     if (!HidD_GetAttributes (handle, &deviceAttributes)) return false;
     CloseHandle (handle);
     handle = INVALID_HANDLE_VALUE;
     if ((vendorID == 0 || (unsigned)deviceAttributes.VendorID == vendorID) &&
         (productID == 0 || (unsigned)deviceAttributes.ProductID == productID) &&
         (versionNumber == 0 || (unsigned)deviceAttributes.VersionNumber == versionNumber))
      {
       //We now have the proper device path, and we can finally open read and write handles to the device.
       //We store the handles in the global variables "WriteHandle" and "ReadHandle", which we will use later to actually communicate.
       WriteHandle = CreateFile(devicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
       ErrorStatus = GetLastError();
       if(ErrorStatus != ERROR_SUCCESS) return false;

       #ifdef _OVP_
       ReadHandle=CreateFile(devicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
			    (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
       #else
       ReadHandle=CreateFile(devicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
			    (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, NULL);
       #endif
       ErrorStatus = GetLastError();
       if(ErrorStatus != ERROR_SUCCESS) return false;

       #ifdef _OVP_
       EventObject = CreateEvent((LPSECURITY_ATTRIBUTES)NULL, FALSE,	TRUE, "");
       //Set the members of the overlapped structure.
       HIDOverlapped.Offset = 0;
       HIDOverlapped.OffsetHigh = 0;
       HIDOverlapped.hEvent = EventObject;
       #endif //_OVP_

       //HidD_GetPreparsedData(ReadHandle, &HidParsedData);

       /* extract the capabilities info */
       //HidP_GetCaps(HidParsedData ,&Capabilities);

       /* Free the memory allocated when getting the preparsed data */
       //HidD_FreePreparsedData(HidParsedData);

        // Register device change notification.
       if (hwnd)
        {
         DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
         DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
         DevBroadcastDeviceInterface.dbcc_classguid = hidGUID;
         DeviceNotificationHandle = RegisterDeviceNotification(hwnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
        }
       connected = true;
       return true; /* matched */
      }
    }
  index++;
  if(index == 10000000) return false;	//Surely there aren't more than 10 million devices attached to any forseeable PC...
 }
 return false;
}
//---------------------------------------------------------------------------

void CUSBDevice::Disconnect(void)
{
 try
  {
   if (EventObject != INVALID_HANDLE_VALUE) CloseHandle(EventObject);
  }
 catch(...)
  {
  }
 EventObject = INVALID_HANDLE_VALUE;

 try
  {
   if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
  }
 catch(...)
  {
  }
 handle = INVALID_HANDLE_VALUE;

 try
  {
   if (WriteHandle != INVALID_HANDLE_VALUE) CloseHandle(WriteHandle);
  }
 catch(...)
  {
  }
 WriteHandle = INVALID_HANDLE_VALUE;

 try
  {
   if (ReadHandle != INVALID_HANDLE_VALUE) CloseHandle(handle);
  }
 catch(...)
  {
  }
 ReadHandle = INVALID_HANDLE_VALUE;

 try
  {
    if (DeviceNotificationHandle != INVALID_HANDLE_VALUE)
      UnregisterDeviceNotification(DeviceNotificationHandle);
  }
 catch(...)
  {
  }
 DeviceNotificationHandle != INVALID_HANDLE_VALUE;

 connected = false;
}
//---------------------------------------------------------------------------

bool CUSBDevice::Manufacturer(wchar_t *Buffer, int BufSize)
 {
  if (!connected) return false;
  memset(Buffer, 0, BufSize);
#ifdef _DYN_DLL_
  return connected = HidD_GetManufacturerString_dll(ReadHandle, Buffer, BufSize);
#else
  return connected = HidD_GetManufacturerString(ReadHandle, Buffer, BufSize);
#endif
 }
//---------------------------------------------------------------------------

bool CUSBDevice::Product(wchar_t *Buffer, int BufSize)
 {
  if (!connected) return false;
  memset(Buffer, 0, BufSize);
#ifdef _DYN_DLL_
  return connected = HidD_GetProductString_dll(ReadHandle, Buffer, BufSize);
#else
  return connected = HidD_GetProductString(ReadHandle, Buffer, BufSize);
#endif
 }
//---------------------------------------------------------------------------


bool CUSBDevice::Write(const BYTE *buffer, int nBuffLen)
{
  unsigned long BytesWritten;
  BYTE UsbReport[USB_BUFFER_SIZE+1]; //Allocate a memory buffer equal to the OUT endpoint size + 1
  //Initialize the rest of the 64-byte packet to "0xFF".  Binary '1' bits
  //do not use as much power, and do not cause as much EMI
  //when they move across the USB cable.  USB traffic is "NRZI" encoded,
  //where '1' bits do not cause the D+/D- signals to toggle states.
  //This initialization is not strictly necessary however.
  //The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
  memset(UsbReport, 0xff, sizeof(UsbReport));
  UsbReport[0] = 0; // Report ID = 0

  if (nBuffLen > USB_BUFFER_SIZE) memcpy(&UsbReport[1], buffer, USB_BUFFER_SIZE);
  else memcpy(&UsbReport[1], buffer, nBuffLen);
  DWORD res = WriteFile(WriteHandle, (LPCVOID)UsbReport, USB_BUFFER_SIZE+1, &BytesWritten, NULL);
  connected = res;
  return res;
}
//---------------------------------------------------------------------------

int CUSBDevice::Read(BYTE *buffer, int nBuffLen, int timeout)
{
  unsigned long BytesRead = 0;
  BYTE UsbReport[USB_BUFFER_SIZE+1];  //Allocate a memory buffer equal to the IN endpoint size + 1

  memset(UsbReport, 0, sizeof(UsbReport));

  #ifdef _OVP_
  //EventObject = CreateEvent((LPSECURITY_ATTRIBUTES)NULL, FALSE,	TRUE, "");
  //HIDOverlapped.hEvent = EventObject;
  //HIDOverlapped.Offset = 0;
  //HIDOverlapped.OffsetHigh = 0;

  (void)ReadFile(ReadHandle, (LPVOID)UsbReport, USB_BUFFER_SIZE+1, &BytesRead, (LPOVERLAPPED)&HIDOverlapped);
  //(void)ReadFile(ReadHandle, (LPVOID)bytes, nBuffLen, &BytesRead, (LPOVERLAPPED)&HIDOverlapped);
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
    //return BytesRead;
   }
  else Result = CancelIo(ReadHandle);
  ResetEvent(EventObject);
  return 0;
  #else // _OVP_
  connected = ReadFile(ReadHandle, (LPVOID)UsbReport, USB_BUFFER_SIZE+1, &BytesRead, NULL);
  //connected = ReadFile(ReadHandle, (LPVOID)bytes, nBuffLen, &BytesRead, NULL);
  if (connected)
   {
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
  else return 0;
  #endif //_OVP_
}
//---------------------------------------------------------------------------


bool CUSBDevice::transfer(int outnum, unsigned char *out, int innum, unsigned char *in)
{
 int i;
 if (connected)
  {
   if (innum && outnum)
    {
     if (Write(out, outnum)) Read(in, innum);
    }
  }
 else Connect(VID, PID, VER);
 return connected;
}

//---------------------------------------------------------------------------

