#include <windows.h>

#include <stdbool.h>

#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		    PWSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Beeper2");
	WNDCLASS wndclass = { 0 };
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			   szAppName, MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindow(szAppName, TEXT("Beeper2 Timer Demo"),
				 WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
				 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				 NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

static bool is_in_default_procedure = false;
static bool before_winproc = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    before_winproc = false;
	switch (message) {
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER, 1000, TimerProc);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}

    is_in_default_procedure = true;
	LRESULT result = DefWindowProc(hwnd, message, wParam, lParam);
    is_in_default_procedure = false;

    before_winproc = false;
    return result;
}

/**
 * @brief called by `DispatchMessage`
 *
 * @param hwnd the window handle
 * @param message the WM_TIMER message ID
 * @param iTimerID the timer ID
 * @param dwTime the number of milliseconds that has elapsed snce Windows was started
 */
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime)
{
	static BOOL fFlipFlop = FALSE;

	MessageBeep(-1);
	fFlipFlop = !fFlipFlop;

	RECT rc;
	GetClientRect(hwnd, &rc);

	HDC hdc = GetDC(hwnd);

	HBRUSH hBrush = CreateSolidBrush(fFlipFlop ? RGB(255, 0, 0) : RGB(0, 0, 255));


	FillRect(hdc, &rc, hBrush);

    TCHAR const *str = (!before_winproc) ? TEXT("winproc") : TEXT("elsewhere");
    TextOut(hdc, 5, 5, str, lstrlen(str));

	ReleaseDC(hwnd, hdc);
	DeleteObject(hBrush);
}