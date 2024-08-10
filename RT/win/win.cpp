#include "win.h"

class rt_win : public win
{
};

LRESULT CALLBACK win::WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam )
{
  win *Win;
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.y =
      GetSystemMetrics(SM_CYCAPTION) * 8 +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.x += 300;
    return 0;
  case WM_CREATE:
    /* Attach 'this' pointer to window class to window */
    SetWindowLongPtr(hWnd, 0, (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
  default:
    Win = reinterpret_cast<rt_win *>(GetWindowLongPtr(hWnd, 0));
    if (Win != NULL)
      switch (Msg)
      {
      case WM_CREATE:
        Win->hWnd = hWnd;
        
        return Win->OnCreate((CREATESTRUCT *)lParam) ? 0 : -1;
      case WM_SIZE:
        Win->W = (INT)(SHORT)LOWORD(lParam);
        Win->H = (INT)(SHORT)HIWORD(lParam);
        Win->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
        return 0;
      case WM_ACTIVATE:
        Win->IsActive = LOWORD(wParam) != WA_INACTIVE;
        Win->OnActivate((UINT)LOWORD(wParam),(HWND)(lParam),(BOOL)HIWORD(wParam));
        return 0;
      case WM_ERASEBKGND:
        return (LRESULT)Win->OnEraseBkgnd((HDC)wParam);
      case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        Win->OnPaint(hDC, &ps);
        EndPaint(hWnd, &ps);
        return 0;
      case WM_DRAWITEM:
         // Win->OnDrawItem((INT)wParam, (DRAWITEMSTRUCT *)lParam);
        return 0;
      case WM_TIMER:
        Win->OnTimer((UINT)wParam);
        return 0;
      case WM_MOUSEWHEEL:
        // Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam),
        //                   (INT)(SHORT)HIWORD(lParam),
        //                   (INT)(SHORT)HIWORD(wParam),
        //                   (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDBLCLK:
        // Win->OnButtonDown(TRUE, (INT)(SHORT)LOWORD(lParam),
        //                   (INT)(SHORT)HIWORD(lParam),(UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        // Win->OnLButtonDown(FALSE, (INT)(SHORT)LOWORD(lParam),
        //                  (INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        // Win->OnLButtonUp((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), 
        //                  (UINT)(SHORT)LOWORD(wParam));
      case WM_DROPFILES:
        // Win->OnDropFiles((HDROP)wParam);
        return 0;
      case WM_NOTIFY:
        // return Win->OnNotify((INT)wParam, (NMHDR *)lParam);
      case WM_COMMAND:
        // Win->OnCommand((INT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
        return 0;
      case WM_MENUSELECT:
        // Win->OnMenuSelect((HMENU)lParam,
        //   (HIWORD(wParam) & MF_POPUP) ? 0L : (INT)LOWORD(wParam),
        //   (HIWORD(wParam) & MF_POPUP) ?
        //     GetSubMenu((HMENU)lParam, LOWORD(wParam)) : 0L,
        //   (UINT)(((SHORT)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam)));
        return 0;

      case WM_INITMENUPOPUP:
        // Win->OnInitMenuPopup((HMENU)wParam, (UINT)
        //   LOWORD(lParam), (BOOL)HIWORD(lParam));
        return 0;
      case WM_HSCROLL:
        // Win->OnHScroll((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
        return 0;
      case WM_VSCROLL:
        // Win->OnVScroll((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
        return 0;
      case WM_ENTERSIZEMOVE:                                                
        // Win->OnEnterSizeMove();
        return 0;
      case WM_EXITSIZEMOVE:
        //Win->OnExitSizeMove();
        return 0;
      case WM_DESTROY:
        Win->OnDestroy();
        PostQuitMessage(30); /// ?
        return 0;
      default:
        return Win->OnMessage(Msg, wParam, lParam);
      }
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'win::WinFunc' function */
