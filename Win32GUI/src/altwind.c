#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		    PWSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("AltWind");

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

	HWND hwnd = CreateWindow(szAppName,
				 TEXT("Alternate and Winding Fill Modes"),
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

static POINT aptFigure[10] = { 10, 70, 50, 70, 50, 10, 90, 10, 90, 50,
			       30, 50, 30, 90, 70, 90, 70, 30, 10, 30 };
static void OnPaint(HWND hwnd)
{
	POINT apt[10];
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	SelectObject(hdc, GetStockObject(GRAY_BRUSH));

	for (int i = 0; i < 10; i++) {
		apt[i].x = cxClient * aptFigure[i].x / 200;
		apt[i].y = cyClient * aptFigure[i].y / 100;
	}

	SetPolyFillMode(hdc, ALTERNATE);
	Polygon(hdc, apt, 10);

	for (int i = 0; i < 10; i++) {
		apt[i].x += cxClient / 2;
	}

	SetPolyFillMode(hdc, WINDING);
	Polygon(hdc, apt, 10);

	EndPaint(hwnd, &ps);
}

static void OnSize(HWND hwnd, LPARAM lParam)
{
	cxClient = LOWORD(lParam);
	cyClient = HIWORD(lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) {
	case WM_SIZE:
		OnSize(hwnd, lParam);
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