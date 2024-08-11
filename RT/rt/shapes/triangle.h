#include "shp_base.h"

#ifndef __triangle_h_
#define __triangle_h_

class triangle : public shp
{
public:

  vec3 P0, P1, P2;
  vec3 Norm;
  vec3 ort1, ort2;
  DBL b1, b2;

  triangle( vec3 P0, vec3 P1, vec3 P2, surf Surf, matr Matr = matr() ) : P0(P0), P1(P1), P2(P2)
  {
    this->Matr = Matr;
    this->Surf = Surf;
    b1 = !(P1 - P0);
    b2 = !(P2 - P0);
    ort1 = (P1 - P0) / b1;
    ort2 = (P2 - P0) / b2;
    Norm = (ort2 % ort1).norm();
  }

  BOOL __GetIntersection( ray &Ray, intr &Intr ) override
  {
    if ((Ray.Dir & Norm) == 0)
      return FALSE;
    DBL t = ((P0 - Ray.Start) & Norm) / (Ray.Dir & Norm);
    if (t < 0)
      return FALSE;
    vec3 InterPos = Ray[t];
    vec3 r = InterPos - P0;
    DBL x1 = (r & ort1) / b1;
    DBL x2 = (r & ort2) / b2;
    shp::SetIntr(Intr, Ray, t, this);
    return x1 >= 0 && x2 >= 0 && x1 + x2 <= 1;
  }

  BOOL __GetAllIntersections( ray &Ray, std::vector<intr> &Intrs ) override
  {
    intr Intr;
    BOOL flag = __GetIntersection(Ray, Intr);
    if (flag)
      Intrs.push_back(Intr);
    return flag;
  }

  vec3 __GetNorm( vec3 Point ) override
  {
    return Norm;
  }

  BOOL __IsInside( vec3 Point ) override
  {
    vec3 r = Point - P0;
    DBL x1 = (r & ort1) / b1;
    DBL x2 = (r & ort2) / b2;
    return IS_ZERO(r & Norm) && x1 >= 0 && x2 >= 0 && x1 + x2 <= 1;
  }
};

#endif /* __sphere_h_ */


