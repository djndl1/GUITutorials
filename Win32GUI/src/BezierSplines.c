#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		    PWSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Bezier");

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

	HWND hwnd = CreateWindow(szAppName, TEXT("Bezier Splines"),
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

void DrawBezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 4);
	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);

	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
}

static POINT apt[4];
static int cxClient, cyClient;

static void OnSize(HWND hwnd, LPARAM lParam)
{
	cxClient = LOWORD(lParam);
	cyClient = HIWORD(lParam);

	apt[0].x = cxClient / 4;
	apt[0].y = cyClient / 2;

	apt[1].x = cxClient / 2;
	apt[1].y = cyClient / 4;

	apt[2].x = cxClient / 2;
	apt[2].y = 3 * cyClient / 4;

	apt[3].x = 3 * cxClient / 4;
	apt[3].y = cyClient / 2;
}

/**
 * @brief adjust the two control points and redraw the cubic Bezier curve
 *   based on mouse movements
 */
static void OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam & MK_LBUTTON || wParam & MK_RBUTTON) {
		HDC hdc = GetDC(hwnd);

         // erase the old Bezier curve
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		DrawBezier(hdc, apt);

		if (wParam & MK_LBUTTON) {
			apt[1].x = LOWORD(lParam);
			apt[1].y = HIWORD(lParam);
		}

		if (wParam & MK_RBUTTON) {
			apt[2].x = LOWORD(lParam);
			apt[2].y = HIWORD(lParam);
		}

        // draw the new Bezier curve
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		DrawBezier(hdc, apt);
		ReleaseDC(hwnd, hdc);
	}
}

static void OnPaint(HWND hwnd)
{
	InvalidateRect(hwnd, NULL, TRUE);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	DrawBezier(hdc, apt);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_SIZE:
		OnSize(hwnd, lParam);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		OnMouseMove(hwnd, wParam, lParam);
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