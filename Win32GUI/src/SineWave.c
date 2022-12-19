#include <windows.h>
#include <math.h>

#define NUM 1000
#define TWOPI (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		    PWSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SineWave");

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

	HWND hwnd = CreateWindow(szAppName, TEXT("Sine Wave Using Polyline"),
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

static int cxClient, cyClient;

/**
 * draw a sine curve as a series of lines
 */
static void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	MoveToEx(hdc, 0, cyClient / 2, NULL); // move to (0, Y/2)
	LineTo(hdc, cxClient, cyClient / 2); // draw the x axis

	POINT apt[NUM]; // compute every point
	for (int i = 0; i < NUM; i++) {
		apt[i].x = i * cxClient / NUM;
		apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / NUM))); // Y/2 * (1 - sin(x))
	}
	Polyline(hdc, apt, NUM);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
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