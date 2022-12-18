#include <afxwin.h> // MFC

/**
 * @brief providers the main message loop
 * An MFC app can have one and only one `CWinApp`
 */
class CMyApp : public CWinApp
{
public:
    /**
     * @brief called right after the app starts.
     * This function should starts the window
     * 
     * @return `TRUE` if initialization succeeds and the app starts,
     *          `FALSE` then the app quits
     */
    virtual BOOL InitInstance();
};

/**
 * a message map is needed for proper message handling
 * 1. DECLARE_MESSAGE_MAP()
 * 2. implement the map with `BEGIN_MESSAGE_MAP()` and `END_MESSAGE_MAP()`
 */
class CMainWindow : public CFrameWnd
{
public:
    CMainWindow();

protected:
    /**
     * @brief preconfigured by MFC; the signature should be looked up in the documentation
     *  `afx_msg` is a visual reminder that this function is message handlere
     */
    afx_msg void OnPaint();

    /**
     * @brief should go to the last member
     */
    DECLARE_MESSAGE_MAP()
};