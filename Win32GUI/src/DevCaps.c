#include <windows.h>

struct {
	int iIndex;
	TCHAR *szLabel;
	TCHAR *szDesc;
} devcaps[] = {
	HORZSIZE,    TEXT("HORZSIZE"),	  TEXT("Width in millimeters:"), // Computed based on DPI and the entire resolution
	VERTSIZE,    TEXT("VERTSIZE"),	  TEXT("Height in millimeters:"),
	HORZRES,     TEXT("HORZRES"),	  TEXT("Width in pixels:"),
	VERTRES,     TEXT("VERTRES"),	  TEXT("Height in raster lines:"),
	BITSPIXEL,   TEXT("BITSPIXEL"),	  TEXT("Color bits per pixel:"),
	PLANES,	     TEXT("PLANES"),	  TEXT("Number of color planes:"),
	NUMBRUSHES,  TEXT("NUMBRUSHES"),  TEXT("Number of device brushes:"),
	NUMPENS,     TEXT("NUMPENS"),	  TEXT("Number of device pens:"),
	NUMMARKERS,  TEXT("NUMMARKERS"),  TEXT("Number of device markers:"),
	NUMFONTS,    TEXT("NUMFONTS"),	  TEXT("Number of device fonts:"),
	NUMCOLORS,   TEXT("NUMCOLORS"),	  TEXT("Number of device colors:"),
	PDEVICESIZE, TEXT("PDEVICESIZE"), TEXT("Size of device structure:"),
	ASPECTX,     TEXT("ASPECTX"),	  TEXT("Relative width of pixel:"),
	ASPECTY,     TEXT("ASPECTY"),	  TEXT("Relative height of pixel:"),
	ASPECTXY,    TEXT("ASPECTXY"),	  TEXT("Relative diagonal of pixel:"),
	LOGPIXELSX,  TEXT("LOGPIXELSX"),  TEXT("Horizontal dots per inch:"), // DPI
	LOGPIXELSY,  TEXT("LOGPIXELSY"),  TEXT("Vertical dots per inch:"), // DPI
	SIZEPALETTE, TEXT("SIZEPALETTE"), TEXT("Number of palette entries:"),
	NUMRESERVED, TEXT("NUMRESERVED"), TEXT("Reserved palette entries:"),
	COLORRES,    TEXT("COLORRES"),	  TEXT("Actual color resolution:"),
};

#define NUMLINES ((int)(sizeof devcaps / sizeof devcaps[0]))

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR szCmdLine,
		   int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("DevCaps1");
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
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			   szAppName, MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindow(szAppName, TEXT("Device Capabilities"),
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

static int cxChar;
static int cxCaps;
static int cyChar;

static void OnCreate(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	cxChar = tm.tmAveCharWidth;
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
	cyChar = tm.tmHeight + tm.tmExternalLeading;
	ReleaseDC(hwnd, hdc);
}

static void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	for (int i = 0; i < NUMLINES; i++) {
		TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel,
			lstrlen(devcaps[i].szLabel));
		TextOut(hdc, 14 * cxCaps, cyChar * i, devcaps[i].szDesc,
			lstrlen(devcaps[i].szDesc));
		SetTextAlign(hdc, TA_RIGHT | TA_TOP);

		TCHAR szBuffer[10];
		TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer,
			wsprintf(szBuffer, TEXT("%5d"),
				 GetDeviceCaps(hdc, devcaps[i].iIndex)));

		SetTextAlign(hdc, TA_LEFT | TA_TOP);
	}
	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		OnCreate(hwnd);
		return 0;
	case WM_PAINT:
		OnPaint(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}