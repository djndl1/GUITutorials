/*----------------------------------------------------
   SYSMETS1.C -- System Metrics Display Program No. 1
                 (c) Charles Petzold, 1998
  ----------------------------------------------------*/

#define WINVER 0x0500
#include "sysmets.h"

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		    PWSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

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

	hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 1"),
			    WS_OVERLAPPEDWINDOW | WS_VSCROLL |
				    WS_HSCROLL, // scroll bars
			    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			    CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

static int cxChar; // Character width
static int cxCaps; // capital character width
static int cyChar; // character height

static int iVscrollPos; // vertical text position
static int iHorzPos; // horizontal text position

static int iMaxWidth = 0; // max text width

static int GetHorizontalScrollPosition(HWND hwnd)
{
	SCROLLINFO si = { .cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS };

	GetScrollInfo(hwnd, SB_HORZ, &si);

	return si.nPos;
}

static int GetVerticalScrollPosition(HWND hwnd)
{
	SCROLLINFO si = { .cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS };

	GetScrollInfo(hwnd, SB_VERT, &si);

	return si.nPos;
}

static int OnResize(HWND hwnd, int cxClient, int cyClient)
{
	SCROLLINFO vsi = { 0 };
	vsi.cbSize = sizeof(SCROLLINFO);
	vsi.fMask = SIF_RANGE | SIF_PAGE;
	vsi.nMin = 0;
	vsi.nMax = NUMLINES - 1;
	vsi.nPage = cyClient / cyChar;
	SetScrollInfo(hwnd, SB_VERT, &vsi, TRUE);

	SCROLLINFO hsi = { 0 };
	hsi.cbSize = sizeof(SCROLLINFO);
	hsi.fMask = SIF_RANGE | SIF_PAGE;
	hsi.nMin = 0;
	hsi.nMax = 2 + iMaxWidth / cxChar;
	hsi.nPage = cxClient / cxChar;
	SetScrollInfo(hwnd, SB_HORZ, &hsi, TRUE);
}

static void OnCreate(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	cxChar = tm.tmAveCharWidth;
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2; // upper-case size
	cyChar = tm.tmHeight + tm.tmExternalLeading;

	ReleaseDC(hwnd, hdc);

	iMaxWidth = 40 * cxChar + 22 * cxCaps;
}

static void OnVerticallyScrolled(HWND hwnd, WPARAM wParam)
{
	SCROLLINFO rsi = { 0 };
	rsi.cbSize = sizeof(SCROLLINFO);
	rsi.fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_VERT, &rsi);

	switch (LOWORD(wParam)) {
	case SB_TOP:
		rsi.nPos = rsi.nMin;
		break;
	case SB_BOTTOM:
		rsi.nPos = rsi.nMax;
		break;
	case SB_LINEUP:
		rsi.nPos -= 1;
		break;
	case SB_LINEDOWN:
		rsi.nPos += 1;
		break;
	case SB_PAGEUP:
		rsi.nPos -= rsi.nPage;
		break;
	case SB_PAGEDOWN:
		rsi.nPos += rsi.nPage;
		break;
	case SB_THUMBTRACK: // track directly
		rsi.nPos = rsi.nTrackPos;
		break;
	default:
		break;
	};

	rsi.fMask = SIF_POS;
	SetScrollInfo(hwnd, SB_VERT, &rsi, TRUE);

	GetScrollInfo(hwnd, SB_VERT, &rsi);
	if (iVscrollPos != rsi.nPos) {
		ScrollWindow(hwnd, 0, cyChar * (iVscrollPos - rsi.nPos), NULL,
			     NULL);
		UpdateWindow(hwnd);
	}
}

static void OnHorizontallyScrolled(HWND hwnd, WPARAM wParam)
{
	SCROLLINFO rhsi = { 0 };
	rhsi.cbSize = sizeof(SCROLLINFO);
	rhsi.fMask = SIF_ALL;

	GetScrollInfo(hwnd, SB_HORZ, &rhsi);
	iHorzPos = rhsi.nPos;
	switch (LOWORD(wParam)) {
	case SB_LINELEFT:
		rhsi.nPos -= 1;
		break;
	case SB_LINERIGHT:
		rhsi.nPos += 1;
		break;
	case SB_PAGELEFT:
		rhsi.nPos -= rhsi.nPage;
		break;
	case SB_PAGERIGHT:
		rhsi.nPos += rhsi.nPage;
		break;
	case SB_THUMBPOSITION: // not scrolling until the mouse button is released
		rhsi.nPos = rhsi.nTrackPos;
		break;
	default:
		break;
	}
	// Set the position and then retrieve it. Due to adjustments
	// by Windows it may not be the same as the value set.
	rhsi.fMask = SIF_POS;
	SetScrollInfo(hwnd, SB_HORZ, &rhsi, TRUE);
	GetScrollInfo(hwnd, SB_HORZ, &rhsi);
	// If the position has changed, scroll the window

	if (rhsi.nPos != iHorzPos) {
		ScrollWindow(hwnd, cxChar * (iHorzPos - rhsi.nPos), 0, NULL,
			     NULL); // repaint
	}
}

static void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	iVscrollPos = GetVerticalScrollPosition(hwnd);
	iHorzPos = GetHorizontalScrollPosition(hwnd);

	int iPaintBegin = max(0, iVscrollPos + ps.rcPaint.top / cyChar);
	int iPaintEnd =
		min(NUMLINES - 1, iVscrollPos + ps.rcPaint.bottom / cyChar);

	for (int i = iPaintBegin; i <= iPaintEnd; i++) {
		int x = cxChar * (1 - iHorzPos);
		int y = cyChar * (i - iVscrollPos);
		TextOut(hdc, x, y, sysmetrics[i].szLabel,
			lstrlen(sysmetrics[i].szLabel));

		TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc,
			lstrlen(sysmetrics[i].szDesc));

		SetTextAlign(hdc, TA_RIGHT | TA_TOP);

		TCHAR szBuffer[10];
		TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer,

			wsprintf(szBuffer, TEXT("%5d"),
				 GetSystemMetrics(sysmetrics[i].iIndex)));

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

	case WM_SIZE:
		OnResize(hwnd, LOWORD(lParam), HIWORD(lParam));

		return 0;

	case WM_VSCROLL:
		OnVerticallyScrolled(hwnd, wParam);
		return 0;

	case WM_HSCROLL:
		OnHorizontallyScrolled(hwnd, wParam);

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
