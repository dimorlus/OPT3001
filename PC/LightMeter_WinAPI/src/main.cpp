//---------------------------------------------------------------------------
// Main entry point
// OPT3001 Light Meter - WinAPI version
//---------------------------------------------------------------------------

#include <windows.h>
#include <commctrl.h>
#include "../include/LightMeter.h"
#include "../resource/resource.h"

#pragma comment(lib, "comctl32.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize common controls
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    CLightMeter lightMeter;
    
    if (!lightMeter.Create(hInstance))
    {
        MessageBoxA(NULL, "Failed to create main window", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    lightMeter.Show();
    lightMeter.MessageLoop();

    return 0;
}
