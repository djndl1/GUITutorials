#include <windows.h>


/**
* @brief The entry of Win32 GUI apps
*
* called with the __stdcall calling convention
*
* @param hInstance the program's instance handle
* @param hPrevInstance always `NULL`. Initially for multi-instance detection in Win16
* @param lpCmdLine command line arguments
* @param nShowCmd how the program should be initially displayed: maximized, minimized or normal
* @return the error status
*/
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    MessageBoxW(NULL, L"Hello, Windows!", L"Hello Windows", MB_YESNO | MB_ICONINFORMATION);

    return 0;
}