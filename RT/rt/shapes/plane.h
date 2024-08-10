#include "shp_base.h"

#ifndef __plane_h_
#define __plane_h_

class plane : public shp
{
public:

  vec3 Pos;
  vec3 Norm;

  plane( vec3 Pos, vec3 Norm, matr Matr = matr() ) : Pos(Pos), Norm(Norm.norm())
  {
    this->Matr = Matr;
  }

  BOOL __GetIntersection( ray &Ray, intr &Intr ) override
  {
    if (IS_ZERO(Ray.Dir & Norm))
      return FALSE;
    DBL t = ((Pos - Ray.Start) & Norm) / (Ray.Dir & Norm);
    if (t < 0)
      return FALSE;
    shp::SetIntr(Intr, Ray, t, this);
    return TRUE;
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
    return IS_ZERO((Point - Pos) & Norm);
  }
};

#endif /* __sphere_h_ */

