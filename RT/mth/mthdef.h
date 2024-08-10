#ifndef __mthdef_h_
#define __mthdef_h_

#include <Windows.h>
#include <cmath>
#include <vector>
#include <algorithm>

#define PI 3.14159265358979323846
#define Ee  2.71828182845904523536
#define D2R(A) (A * PI / 180.0)
#define R2D(A) (A * 180.0 / PI)
#define THRESHOLD 0.00001
#define IS_ZERO(A) (abs(A) <= THRESHOLD)

typedef double DBL;
typedef float FLT;


namespace mth
{
  template<typename Type> class vec2;
  template<typename Type> class vec3;
  template<typename Type> class complex;
  template<typename Type> class vec4;
  template<typename Type> class matr;
  template<typename Type> class ray;
  // class camera;
  // class coordSystem;

  // std::vector<complex<DBL>> Equation4Pow( DBL a, DBL b, DBL c, DBL d, DBL e );
  // std::vector<complex<DBL>> Equation3Pow( DBL a, DBL b, DBL c, DBL d );

}

typedef mth::vec2<DBL> vec2;
typedef mth::vec3<DBL> vec3;
typedef mth::complex<DBL> complex;
typedef mth::vec4<DBL> vec4;
typedef mth::matr<DBL> matr;
typedef mth::ray<mth::vec3<DBL>> ray;

// typedef mth::coordSystem coord;
#endif /* __mthdef_h_ */

