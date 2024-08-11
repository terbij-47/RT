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

  surf Surf1(vec3(0.1), vec3(0, 0, 0), vec3(.5), 50, 0, 0.99, 0.9);
  surf Surf2(vec3(0.2), vec3(1, 0, 1), vec3(.5), 50);

  // Scene << new dirLight(vec3(-1, -1, 1));
  Scene << new pointLight(vec3(0, 2, 0.3), vec3(1), 0., 0, 0.0);
  shp* Sphere = new sphere(vec3(0, 0, 0), 0.7, Surf1);
  shp* Box = new box(vec3(), vec3(0, 1, 0.2), vec3(1, 0, 0), 1, 1, 1, Surf2);
  // Scene << new sphere(vec3(0, 0, 0), 1, matr::scale(vec3(1, 1, 1)));
  // Scene << new triangle(vec3(), vec3(0.5, 0, 0), vec3(0, 0, 0.5));
  // Scene << new box(vec3(), vec3(0, 1, 0), vec3(1, 0, 0), 1, 1, 1, Surf1);
  Scene << new sphere(vec3(0, 0.4, 0), 0.5, Surf1);
  Scene << new sphere(vec3(-1.5, 0.2, -1.5), 0.3, Surf2);
  // Scene << new quadric(-1, 0, 0, 0, 1, 0, 0, 1, 0, -1, Surf1);
  Scene << new plane(vec3(), vec3(0, 1, 0), Surf2);
  Scene << new csg(Box, Sphere, (UINT)csg::type::SUB, Surf1);
  W.WinFrame.Scene = &Scene;

  W.WinFrame.RenderScene();

  W.Run();


}