#include "../def.h"
#include "rt.h"

#ifndef __frame_h_
#define __frame_h_

struct camera
{
  DBL Near = 10;
  DBL SizeW = 1;
  DBL SizeH = 1;

  vec3 Pos = vec3(0, 0.0, 2);
  vec3 At = vec3();
  vec3 Up = vec3(0, 1, 0);
  vec3 Right = vec3(1, 0, 0);
};

class frame
{
public:

  INT FrameW, FrameH;             // Parametrs of screen (width and height)
  const INT Wconst;
  const INT Hconst;
  rt *Scene = nullptr;
  camera Cam;

  // ui * Ui;

  DWORD *Bits;                    // Pointer to bits array of screen
  DWORD *Screen;
  BOOL IsAntia;

  /* Struct constuctor.
   * ARGUMENTS:
   *   - heigth and width of window:
   *       INT NewW (400 if not given), NewH (400 if not given);
   */
  frame( INT W = 500, INT H = 500 ) : FrameW(W), FrameH(H), Wconst(W), Hconst(H),
                                      Bits(new DWORD[W * H]), Screen(new DWORD[H * W])
  {
    memset(Bits, 0x0000FF, sizeof(DWORD) * W * H);
    memset(Screen, 0x00FFFF, sizeof(DWORD) * W * H);
    IsAntia = FALSE;
    SetCamera(5, 1, 1, vec3(2, 0.4, 3.001), vec3(), vec3(0, 1, 0));
  } /* End of 'frame' constuctor */

  /* Struct destructor.
   * ARGUMENTS: None.
   */
  ~frame( VOID )
  {
    delete[] Bits;
    delete[] Screen;
    
  } /* End of '~frame' destructor. */

  /* Create dib section method.
   * ARGUMENTS:
   *   - handle description:
   *       HDC hDC;
   * RETURNS: None.
   */
  VOID DrawToScreen( HDC hDC )
  {
    BITMAPINFOHEADER info = {0};
     
    memset(&info, 0, sizeof(info));
    info.biSize = sizeof(BITMAPINFOHEADER);
    info.biBitCount = 32;
    info.biPlanes = 1;
    info.biCompression = BI_RGB;
    info.biWidth = FrameW;
    info.biHeight = FrameH;
    
    info.biSizeImage = FrameW * FrameH + 4;
    if (IsAntia)
      SetDIBitsToDevice (hDC, 0, 0, FrameW, FrameH, 0, 0, 0, FrameH, Screen, (BITMAPINFO*)&info, DIB_RGB_COLORS);
    else
      SetDIBitsToDevice (hDC, 0, 0, FrameW, FrameH, 0, 0, 0, FrameH, Bits, (BITMAPINFO*)&info, DIB_RGB_COLORS);
  }

  /* Count anti-aliazing function;
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID AntiAliazing( VOID )
  {
  } 

  /* Set pixel color to frame function.
   * ARGUMENTS:
   *   - pixel position:
   *       INT x, y;
   *   - colors (rgb components):
   *       BYTE r, g, b;
   */
  VOID PutPixel( INT x = 0, INT y = 0, BYTE r = 0, BYTE g = 255, BYTE b = 255 )
  {
    if (x < 0 || y < 0 || x > FrameW || y > FrameH)
      return;
    Bits[y * FrameW + x] = min(abs(r), 255) << 16 | 
                           min(abs(g), 255) << 8 | 
                           min(abs(b), 255);
  }

  /* Set pixel color to frame function.
   * ARGUMENTS:
   *   - pixel position:
   *       INT x, y;
   *   - colors (rgb components):
   *       BYTE r, g, b;
   */
  VOID PutPixel( INT x, INT y, vec3 Color )
  {
    if (x < 0 || y < 0 || x > FrameW || y > FrameH)
      return;
    if (Color[0] == 0 && Color[1] == 0 && Color[2] == 0)
    {
      Bits[y * FrameW + x] = 0x00000000;
      return;
    }
  
    Bits[y * FrameW + x] = (INT)min(abs(Color[0] / (1.0 / 255.0)), 255) << 16 | 
                           (INT)min(abs(Color[1] / (1.0 / 255.0)), 255) << 8 | 
                           (INT)min(abs(Color[2] / (1.0 / 255.0)), 255);
  }

  /* Set pixel color to frame function.
   * ARGUMENTS:
   *   - pixel position:
   *       INT x, y;
   * RETURNS: None.
   */
  VOID PutPixel( INT x, INT y )
  {
    if (x < 0 || y < 0 || x > FrameW || y > FrameH)
      return;
    Bits[y * FrameW + x] = 0x00000000;
  }

  VOID SetCamera( DBL Near, DBL W, DBL H, vec3 Pos, vec3 At, vec3 Up )
  {
    Cam.Near = Near;
    Cam.SizeW = W;
    Cam.SizeH = H;
    Cam.Pos = Pos;
    Cam.At = At;
    vec3 Dir = (At - Pos).norm();
    Cam.Right = (Dir % Up).norm();
    Cam.Up = Cam.Right % Dir;
  }

  VOID RenderScene( VOID )
  {
    vec3 Dir = (Cam.At - Cam.Pos).norm();
    // смещение в левый нижний угол 
    vec3 Offset = Cam.Pos - Cam.Right * Cam.SizeW - Cam.Up * Cam.SizeH;
    vec3 dx = Cam.Right * (2 * Cam.SizeW / FrameW);
    vec3 dy = Cam.Up * (2 * Cam.SizeH / FrameH);
    vec3 Start = Cam.Pos - Dir * Cam.Near;

    for (INT x = 0; x < FrameW; x++)
      for (INT y = 0; y < FrameH; y++)
      {
        vec3 End = Offset + dx * x + dy * y;
        ray Ray(Start, End - Start);
        PutPixel(x, y, Scene->Trace(Ray));
      }
  }

};

#endif /* __frame_h_ */
