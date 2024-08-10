#include <thread>

#include "win.h"
#include "../rt/frame.h"

#ifndef __rt_win_h_
#define __rt_win_h_


// #include "def.h"
// #include "scene.h"

  class rt_win : public win
  {
  private:
      CHAR *WindowClassName;            // Window class name and name of window
    
  public:

    frame WinFrame;
    /* Change window size function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
     VOID Resize( VOID )
     {
       if (WinFrame.FrameH != win::H && WinFrame.FrameW != win::W)
       {
         INT Size = WinFrame.Hconst * WinFrame.Wconst;
         DWORD *Tmp = new DWORD[Size];
         memset(Tmp, 0, sizeof(DWORD) * Size);
         memcpy(Tmp, WinFrame.Bits, sizeof(DWORD) * Size);
         delete[] WinFrame.Bits;
         WinFrame.Bits = Tmp;
         WinFrame.FrameH = WinFrame.Hconst;
         WinFrame.FrameW = WinFrame.Wconst;
       }
     }

    /* Class constuctor.
     * ARGUMENTS:
     *   - application handle instanced:
     *       HINSTANCE hInst;
     */
     rt_win( HINSTANCE hInst = GetModuleHandle(nullptr), CHAR* NewClass = (CHAR *)"NoData" ) : WindowClassName(NewClass)
     {
       WNDCLASS wc = { 0 };
       HWND hWnd;
     
       wc.style = CS_VREDRAW | CS_HREDRAW;
       wc.cbClsExtra = 0;
       wc.cbWndExtra = sizeof(rt_win *);
       wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
       wc.hCursor = LoadCursor(NULL, IDC_ARROW);
       wc.hIcon = LoadIcon(NULL, IDI_ERROR);
       wc.hInstance = hInstance;
       wc.lpfnWndProc = win::WinFunc;
       wc.lpszClassName = WindowClassName;
       
       if (!RegisterClass(&wc))
       {
         MessageBox(NULL, "Error registre windows class", "ERROR", MB_OK);
         return;
       }
     
       hWnd = CreateWindow(WindowClassName, WindowClassName,
         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
         CW_USEDEFAULT, CW_USEDEFAULT, 
         WinFrame.Wconst, WinFrame.Hconst,
         NULL, NULL, hInstance, reinterpret_cast<VOID *>(this));
       ShowWindow(hWnd, SW_SHOWNORMAL);
       UpdateWindow(hWnd);
       
       //HWND hButton1 = CreateWindow("button", "Button 1",
       //    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       //    20, 20,
       //    90, 30,
       //    hWnd,
       //    (HMENU) 1,
       //    hInst, NULL);


     }
    
    /* Class destructor.
     * ARGUMENTS: None.
     */
    ~rt_win( VOID ) override
    {
    }

    /* Main loop function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Run( VOID ) override
    {
      MSG msg;
    
      while (TRUE)
      {
        /* Check message at window message queue */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
          if (msg.message == WM_QUIT)
            break;
          else
          {
            /* Displatch message to window */
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
        else
        {
          /* Idle... */
        }
      }
    }

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    BOOL OnCreate( CREATESTRUCT *CS ) override
    {
      return TRUE;
    }
    
    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnDestroy( VOID ) override
    {
    }
    
    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID OnSize( UINT State, INT W, INT H ) override
    {
      HDC hDC = GetDC(hWnd);
      HDC hMemDc = CreateCompatibleDC(hDC);
      HBITMAP hBm = CreateCompatibleBitmap(hDC, WinFrame.Wconst, WinFrame.Hconst);
      ReleaseDC(hWnd, hDC);
      SelectObject(hMemDc, hBm);
      Resize();
    }
    
    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    BOOL OnEraseBkgnd( HDC hDC ) override
    {
      hDC = GetDC(hWnd);
      SetBkColor(hDC, RGB(100, 0, 0));
      ReleaseDC(hWnd, hDC);
      return TRUE;
    }
    
    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    VOID OnPaint( HDC hDC, PAINTSTRUCT *PS ) override
    {
      WinFrame.DrawToScreen(hDC);
    }
    
    /* WM_ACTIVATE window message handle function.
     * ARGUMENTS:
     *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
     *       UINT Reason;
     *   - handle of active window:
     *       HWND hWndActDeact;
     *   - minimized flag:
     *       BOOL IsMinimized;
     * RETURNS: None.
     */
    VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized ) override 
    {
      // WinFrame.DrawToScreen(GetDC(hWndActDeact));
    }
    
    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    VOID OnTimer( INT Id ) override
    {
      InvalidateRect(hWnd, NULL, FALSE);
    }

    /* Window message universal handle function.
     * Should be returned 'DefWindowProc' call result.
     * ARGUMENTS:
     *   - message type (see WM_***):
     *      UINT Msg;
     *   - message 'word' parameter:
     *      WPARAM wParam;
     *   - message 'long' parameter:
     *      LPARAM lParam;
     * RETURNS:
     *   (LRESULT) message depende return value.
     */
    LRESULT OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam ) override
    {
      return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
  
  }; /* End of 'rt_win' class */

#endif  /* __rt_win_h_ */

/* END OF 'rt_win.h' FILE */