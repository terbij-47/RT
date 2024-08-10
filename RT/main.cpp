#include "def.h"
#include "win/rt_win.h"

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  vec2 a(1, 2);
  vec2 b(4, 5);
  vec2 c = a + b;

  complex z(1, 2);

  // auto res = mth::Equation3Pow(1, -3, -1, 3);

  auto w = complex(0, 1) * complex(1, 0);

  matr m;

  rt_win W(hInstance, (CHAR*)("first win"));

  rt Scene;

  // shp* Sphere = new sphere(vec3(0, 0, 0), 0.7, matr());
  // shp* Box = new box(vec3(), vec3(0, 1, 0.2), vec3(1, 0, 0), 1, 1, 1, matr());
  // Scene << new sphere(vec3(0, 0, 0), 1, matr::scale(vec3(1, 1, 1)));
  // Scene << new triangle(vec3(), vec3(0.5, 0, 0), vec3(0, 0, 0.5));
  Scene << new box(vec3(), vec3(0, 1, 0.2), vec3(1, 0, 0), 1, 1, 1, matr());
  Scene << new sphere(vec3(0, 0, 0), 0.5);
  // Scene << new quadric(-1, 0, 0, 0, 1, 0, 0, 1, 0, -1);
  Scene << new plane(vec3(), vec3(0, 1, 0), matr());
  // Scene << new csg(Box, Sphere, (UINT)csg::type::SUB, matr());
  W.WinFrame.Scene = &Scene;

  W.WinFrame.RenderScene();

  W.Run();


}