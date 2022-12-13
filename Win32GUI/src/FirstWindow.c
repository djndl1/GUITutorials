#include <windows.h>

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief the window procedure handles window messages
 *        MS Windows itself calls into this function
 *
 * @param window_handle the handle of the window being processed
 * @param message message identifier
 * @param wparam  a 32-bit parameter used by specific messages
 * @param lParam  another 32-bit parameter used by specific messages
 */
LRESULT CALLBACK my_window_procedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lParam)
{
    RECT rect;
    PAINTSTRUCT ps = { 0 };
    HDC hdc; // device context, the physical output device and its device driver
    switch (message) {
        case WM_PAINT: // part/all of the window's client area is invalid and msut be updated visually

            // starts a scope (Device context)
            hdc = BeginPaint(window_handle, &ps); // the background of the client area is erased
            GetClientRect(window_handle, &rect); // gets the client area rectangle

            DrawTextW(hdc, L"Hello Windows!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(window_handle, &ps); // ends this scope

            return 0; // successfully handled the message

        case WM_DESTROY:
            PostQuitMessage(0); // the message loop detects this message and breaks
            return 0;
    }

    // the default window procedure to provide default processing
    // for any window messages that an application does not process.
    return DefWindowProcW(window_handle, message, wparam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    static WCHAR appName[] = L"FirstWindow";

    WNDCLASS mywindow_class = { 0 }; // define window class, class as in OOP
    mywindow_class.style = CS_HREDRAW | CS_VREDRAW; // redraw upon resizing h/v
    mywindow_class.hInstance = hInstance;
    mywindow_class.hIcon = LoadIconW(NULL, IDI_APPLICATION); // just use a predefined icon
    mywindow_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
    mywindow_class.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); //
    mywindow_class.lpszMenuName = NULL;
    mywindow_class.lpszClassName = appName;
    mywindow_class.lpfnWndProc = my_window_procedure;

    if (!RegisterClassW(&mywindow_class)) { // a class must be registered before using
        MessageBoxW(NULL, L"This program requires WinNT", appName, MB_ICONERROR);
        return 0;
    }

    HWND window_handle = CreateWindowW(
        appName, L"The Hello Program",
        WS_OVERLAPPEDWINDOW, // a normal overlapped window
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, // parent window handle
        NULL, // window menu handle
        hInstance, // program instance handle
        NULL // additional custom parameters, sent to the window procedure upon WM_CREATE
    );

    ShowWindow(window_handle, nShowCmd); // puts the window on display
    // sends a paint message to the window, bypassing the queue, so that the window draw itself for the first time
    UpdateWindow(window_handle);

    MSG msg; // window message
    while (GetMessageW(&msg, NULL, 0, 0)) { // retrieve messages for all windows without filtering
        TranslateMessage(&msg); // for some keyboard translation
        DispatchMessageW(&msg); // sent to the appropriate window procedure fro processing
    }

    return msg.wParam; // passed with WM_QUIT (PostQuitMessage)
}