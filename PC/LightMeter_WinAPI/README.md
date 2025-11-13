# OPT3001 Light Meter - WinAPI Version

This is a Windows desktop application for working with OPT3001 Light Meter USB HID device. The project has been ported from Borland C++ Builder to Visual Studio 2022 using pure WinAPI (no MFC/VCL dependencies).

## Features

### Main Window (Light Meter)
- Real-time light measurement display (Lux)
- Automatic device connection detection
- Calibration coefficient (Klux) editing with EEPROM storage
- Master calibration value editing with EEPROM storage
- Timer-based polling (every 100ms)
- USB HID communication with OPT3001 sensor
- Access to Bootloader functionality

### Bootloader Window
- USB HID device firmware update
- HEX file loading and parsing
- Flash operations: Erase, Program, Verify
- Progress bar for operations
- CRC verification
- Run application command

## Project Structure

```
LightMeter_WinAPI/
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── LightMeter.cpp     # Main window implementation
│   ├── Bootloader.cpp     # Bootloader window implementation
│   ├── USB_HID_CLASS.cpp  # USB HID communication class
│   ├── Hex.cpp            # HEX file parser
│   └── ccrc.cpp           # CRC calculation
├── include/               # Header files
│   ├── LightMeter.h
│   ├── Bootloader.h
│   ├── USB_HID_CLASS.h
│   ├── Hex.h
│   └── ccrc.h
├── LightMeter.sln         # Visual Studio solution
└── LightMeter.vcxproj     # Visual Studio project
```

## Build Requirements

- Visual Studio 2022 (or later)
- Windows SDK 10.0 (or later)
- C++17 or later

## Building the Project

1. Open `LightMeter.sln` in Visual Studio 2022
2. Select configuration (Debug/Release) and platform (Win32/x64)
3. Build the solution (F7 or Build → Build Solution)
4. The executable will be in `bin\[Platform]\[Configuration]\` directory

## USB Device Configuration

The application expects the following USB device parameters:

### Main Application Mode
- Vendor ID: 0x04D8 (1240)
- Product ID: 0x0042 (66)
- Version: 0x0001 (1)

### Bootloader Mode
- Vendor ID: 0x04D8 (1240)
- Product ID: 0x003C (60)
- Version: 0x0000 (0)

## Usage

### Light Meter
1. Connect the OPT3001 USB device
2. The application will automatically detect and connect to the device
3. Light measurements will be displayed in real-time
4. Edit Klux or Master values to calibrate the readings
5. Values are automatically saved to device EEPROM

### Bootloader
1. Click "Bootloader" button in the main window
2. Click "Connect" to connect to the device in bootloader mode
3. Click "Hex File" to load a firmware HEX file
4. Use the following operations:
   - **Erase**: Erase device flash memory
   - **Program**: Program the loaded HEX file to device
   - **Verify**: Verify programmed firmware (CRC check)
   - **All**: Perform Erase → Program → Verify sequence
   - **Run**: Exit bootloader and run the application firmware

## OPT3001 Communication

The application communicates with the OPT3001 light sensor through custom USB HID commands:

- Command 14: Write I2C data
- Command 15: Read I2C data
- Command 20: Get version string
- Command 21: LED blink control
- Command 55: Read ADC values (if enabled)

## Differences from Original BCB Version

- Pure WinAPI implementation (no VCL dependencies)
- Native Windows controls instead of VCL components
- Simplified dialog handling for Bootloader window
- Modern C++ standards (VS2022)
- Better error handling with safe string functions

## Notes

- The application uses overlapped I/O for USB HID communication
- Device notification is registered to handle hot-plug events
- EEPROM values are stored with redundancy (two copies with checksum)
- Low-pass filter is applied to light measurements for stable readings

## License

Original code ported from Borland C++ Builder project.
