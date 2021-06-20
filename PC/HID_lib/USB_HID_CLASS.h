//---------------------------------------------------------------------------

#ifndef USB_HID_CLASSH
#define USB_HID_CLASSH
//---------------------------------------------------------------------------
/* Both of these functions return INVALID_HANDLE_VALUE on an error, if there is no error
    then the HANDLE must be closed using 'CloseHandle' */

#define USB_BUFFER_SIZE         64

class CUSBDevice
{
public:
  CUSBDevice();
  /* Connects to the USB HID described by the combination of vendor id, product id
     If the attribute is null, it will connect to first device satisfying the remaining
     attributes. */
  CUSBDevice(const DWORD vendorID, const DWORD productID, const DWORD versionNumber, HWND hwnd=NULL);
  ~CUSBDevice();
  bool Connect(const DWORD vendorID, const DWORD productID, const DWORD versionNumber, HWND hwnd=NULL);
  bool Connect(void) {return Connect(VID, PID, VER, hWnd);};
  void Disconnect(void);
  bool Write(const BYTE *buffer, int nBuffLen);
  int Read(BYTE *buffer, int nBuffLen, int timeout = 100);

  bool Manufacturer(wchar_t *Buffer, int BufSize);
  bool Product(wchar_t *Buffer, int BufSize);

  bool isConnected(void) {return connected;};

  bool transfer(int outnum, unsigned char *out, int innum, unsigned char *in);
protected:
  /* Connects to the ith USB HID device connected to the computer */
  bool ConnectToIthDevice(HANDLE &deviceHandle, DWORD deviceIndex);
private:
  bool connected;
  char devicePath[4096];
  HDEVNOTIFY DeviceNotificationHandle;
  //HIDP_CAPS Capabilities;
  //PHIDP_PREPARSED_DATA HidParsedData;
  HANDLE EventObject;
  OVERLAPPED HIDOverlapped;
  HANDLE handle;
  HANDLE WriteHandle;	//Need to get a write "handle" to our device before we can write to it.
  HANDLE ReadHandle;	//Need to get a read "handle" to our device before we can read from it.
  GUID hidGUID;

  HWND hWnd;
  DWORD VID;
  DWORD PID;
  DWORD VER;
};
#endif
