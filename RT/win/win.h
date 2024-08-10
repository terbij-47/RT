#include <windows.h>
#include "../def.h"

#ifndef __win_h_
#define __win_h_


  class win
  {
   protected:
     HWND hWnd;           // window handle
     INT W, H;            // window size
     HINSTANCE hInstance; // application handle
 
    
      static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                       WPARAM wParam, LPARAM lParam );
    private:
      BOOL IsFullScreen;                  // Full screen mode flag
      RECT FullScreenSaveRect;            // Full screen mode save rectangle 
      INT MouseWheel;                     // Relative mouse wheel offset 
    
    public:

      BOOL IsActive;         // Active window flag
    
      /* Class constuctor.
       * ARGUMENTS:
       *   - application handle instanced:
       *       HINSTANCE hInst;
       */
      win( HINSTANCE hInst = GetModuleHandle(nullptr) ) : hInstance(hInst), hWnd(nullptr), H(0), W(0), IsFullScreen(0), FullScreenSaveRect({}), MouseWheel(0), IsActive(0)
      {
      }

      /* Class destructor.
       * ARGUMENTS: None.
       */
      virtual ~win( VOID )
      {
      }

      /* Main loop function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      virtual VOID Run( VOID )
      {
      }

      /* Change full screen mode function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID FlipFullScreen( VOID )
      {
      }
   

      /* WM_CREATE window message handle function.
       * ARGUMENTS:
       *   - structure with creation data:
       *       CREATESTRUCT *CS;
       * RETURNS:
       *   (BOOL) TRUE to continue creation window, FALSE to terminate.
       */
      virtual BOOL OnCreate( CREATESTRUCT *CS )
      {
        return TRUE;
      }
      
      /* WM_DESTROY window message handle function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      virtual VOID OnDestroy( VOID )
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
      virtual VOID OnSize( UINT State, INT W, INT H )
      {
        HDC hDC = GetDC(hWnd);
        HDC hMemDc = CreateCompatibleDC(hDC);
        HBITMAP hBm = CreateCompatibleBitmap(hDC, W, H);
        ReleaseDC(hWnd, hDC);
        SelectObject(hMemDc, hBm);
      }
      
      /* WM_ERASEBKGND window message handle function.
       * ARGUMENTS:
       *   - device context of client area:
       *       HDC hDC;
       * RETURNS:
       *   (BOOL) TRUE if background is erased, FALSE otherwise.
       */
      virtual BOOL OnEraseBkgnd( HDC hDC )
      {
        hDC = GetDC(hWnd);
        SetBkColor(hDC, RGB(0, 0, 0));
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
      virtual VOID OnPaint( HDC hDC, PAINTSTRUCT *PS )
      {
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
      virtual VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
      {
      }
      
      /* WM_TIMER window message handle function.
       * ARGUMENTS:
       *   - specified the timer identifier.
       *       INT Id;
       * RETURNS: None.
       */
      virtual VOID OnTimer( INT Id )
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
      virtual LRESULT OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
      {
        return DefWindowProc(hWnd, Msg, wParam, lParam);
      }
    }; /* End of 'win' class */

#endif /* __win_h_  */