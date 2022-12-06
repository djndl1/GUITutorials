#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    MessageBoxW(NULL, L"Hello, Windows!", L"Hello Windows", 0);

    return 0;
}