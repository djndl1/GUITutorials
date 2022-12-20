#include <windows.h>

#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		    PWSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("DigClock");
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

	HWND hwnd = CreateWindow(szAppName, TEXT("Digital Clock"),
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

static BOOL f24Hour, fSuppress;
static HBRUSH hBrushRed;
static int cxClient, cyClient;

void DisplayDigit(HDC hdc, int iNumber)
{
	static BOOL fSevenSegment[10][7] = { { 1, 1, 1, 0, 1, 1, 1, }, // 0
					     { 0, 0, 1, 0, 0, 1, 0, }, // 1
					     { 1, 0, 1, 1, 1, 0, 1, }, // 2
					     { 1, 0, 1, 1, 0, 1, 1, }, // 3
					     { 0, 1, 1, 1, 0, 1, 0, },// 4
					     { 1, 1, 0, 1, 0, 1, 1, },// 5
					     { 1, 1, 0, 1, 1, 1, 1, },// 6
					     { 1, 0, 1, 0, 0, 1, 0, },// 7
					     { 1, 1, 1, 1, 1, 1, 1, },// 8
					     { 1, 1, 1, 1, 0, 1, 1  }, }; // 9
	static POINT ptSegment[7][6] = {
		7,  6,	11, 2,	31, 2,	35, 6,	31, 10, 11, 10, 6,  7,
		10, 11, 10, 31, 6,  35, 2,  31, 2,  11, 36, 7,	40, 11,
		40, 31, 36, 35, 32, 31, 32, 11, 7,  36, 11, 32, 31, 32,
		35, 36, 31, 40, 11, 40, 6,  37, 10, 41, 10, 61, 6,  65,
		2,  61, 2,  41, 36, 37, 40, 41, 40, 61, 36, 65, 32, 61,
		32, 41, 7,  66, 11, 62, 31, 62, 35, 66, 31, 70, 11, 70
	};
	for (int iSeg = 0; iSeg < 7; iSeg++)
		if (fSevenSegment[iNumber][iSeg])
			Polygon(hdc, ptSegment[iSeg], 6);
}

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL suppress)
{
	if (!suppress || (iNumber / 10 != 0)) // two digits or with leading zero
		DisplayDigit(hdc, iNumber / 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
	DisplayDigit(hdc, iNumber % 10); // drawing the lower digit
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayColon(HDC hdc)
{
	POINT ptColon[2][4] = { { { 2, 21 }, { 6, 17 }, { 10, 21 }, { 6, 25 }, },
				            { { 2, 51 }, { 6, 47 }, { 10, 51 }, { 6, 55 } } };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);

	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}

void DisplayTime(HDC hdc, BOOL bf24Hour, BOOL suppress)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	if (bf24Hour)
		DisplayTwoDigits(hdc, st.wHour, suppress);
	else
		DisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12,
				 suppress);

	DisplayColon(hdc);
	DisplayTwoDigits(hdc, st.wMinute, FALSE);
	DisplayColon(hdc);
	DisplayTwoDigits(hdc, st.wSecond, FALSE);
}

/**
 * @brief set the timer; create a red brush
 *
 * @param hwnd
 */
static void OnCreate(HWND hwnd)
{
	hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
	SetTimer(hwnd, ID_TIMER, 1000, NULL);
}

/**
 * @brief in case a setting change, retrieve the new time format
 *
 * @param hwnd
 */
static void OnSettingChange(HWND hwnd)
{
	TCHAR szBuffer[2];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
	f24Hour = (szBuffer[0] == '1');

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
	fSuppress = (szBuffer[0] == '0');

	InvalidateRect(hwnd, NULL, TRUE);
}

static void OnSize(HWND hwnd, LPARAM lParam)
{
	cxClient = LOWORD(lParam);
	cyClient = HIWORD(lParam);
}

static void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	SetMapMode(hdc, MM_ISOTROPIC);

    // set the extents of the window and the device viewport
	SetWindowExtEx(hdc, 276, 72, NULL);
	SetViewportExtEx(hdc, cxClient, cyClient, NULL);

    // set origins of the window and the device viewport and thus their mapping
	SetWindowOrgEx(hdc, 138, 36, NULL);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

	SelectObject(hdc, GetStockObject(NULL_PEN)); // set pen to null, i.e. draws nothing
	SelectObject(hdc, hBrushRed); // set the red brush

	DisplayTime(hdc, f24Hour, fSuppress);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		OnCreate(hwnd);
		// fall through

	case WM_SETTINGCHANGE:
		OnSettingChange(hwnd);
		return 0;

	case WM_SIZE:
		OnSize(hwnd, lParam);
		return 0;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE); // redraw
		return 0;

	case WM_PAINT:
        OnPaint(hwnd);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		DeleteObject(hBrushRed);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}