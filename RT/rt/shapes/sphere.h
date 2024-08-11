#include "shp_base.h"

#ifndef __sphere_h_
#define __sphere_h_

class sphere : public shp
{
public:

  vec3 Pos;
  DBL R;
  DBL R2;

  sphere( vec3 Pos, DBL R, surf Surf, matr Matr = matr() ) : Pos(Pos), R(R)
  {
    this->Matr = Matr;
    this->Surf = Surf;
    R2 = R * R;
  }

  /* in local coordinate system */
  BOOL __GetIntersection( ray &Ray, intr &Intr ) override
  {
    vec3 Delta = Pos - Ray.Start;
    DBL r = Delta & Ray.Dir;
    DBL h2 = Delta.len2() - r * r;
    if (h2 > R2)
      return FALSE;
    DBL x = sqrt(R2 - h2);
    DBL t2 = r + x;
    if (t2 < 0)
      return FALSE;
    DBL t1 = r - x;
    shp::SetIntr(Intr, Ray, (t1 < 0 ? t2 : t1), this);
    return TRUE;
  }

  BOOL __GetAllIntersections( ray &Ray, std::vector<intr> &Intrs ) override
  {
    vec3 Delta = Pos - Ray.Start;
    DBL r = Delta & Ray.Dir;
    DBL h2 = Delta.len2() - r * r;
    if (h2 > R2)
      return FALSE;
    DBL x = sqrt(R2 - h2);
    DBL t2 = r + x;
    if (t2 < 0)
      return FALSE;
    DBL t1 = r - x;
    if (t1 < 0 || x <= THRESHOLD)
    {
      Intrs.resize(1);
      shp::SetIntr(Intrs[0], Ray, t2, this);
      return TRUE;
    }
    Intrs.resize(2);
    shp::SetIntr(Intrs[0], Ray, t1, this);
    shp::SetIntr(Intrs[1], Ray, t2, this);
    return TRUE;
  }

  /* In local coordinate system */ 
  vec3 __GetNorm( vec3 Point ) override
  {
    return (Point - Pos).norm();
  }

  /* In local coordinate system */
  BOOL __IsInside( vec3 Point ) override
  {
    return (Point - Pos).len2() <= (R2);
  }

};

#endif /* __sphere_h_ */
