#include <afxwin.h>
#include "Hello.hpp"

CMyApp myApp; // must be defined globally to be found by MFC in AfxWinMain in winmain.cpp

BOOL CMyApp::InitInstance()
{
    m_pMainWnd = new CMainWindow;

    m_pMainWnd->ShowWindow(m_nCmdShow); // WS_VISIBLE
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

// message map implementation
BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
ON_WM_PAINT()
END_MESSAGE_MAP()

CMainWindow::CMainWindow()
{
    Create(nullptr, TEXT("The Hello Application")); // CreateWindow
}

void CMainWindow::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    dc.DrawText(TEXT("Hello, MFC"), -1, &rect,
                DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}
