#include "../../def.h"

#ifndef __shp_base_h_
#define __shp_base_h_

struct surf
{
  vec3 Ka, Kd, Ks;
  DBL Ph = 10;
  DBL Wl = 0.78;  // коэфф затухания (какая часть света проходит дальше)
  DBL Kr = 0.15, Wr = 0.15;  // коэфф преломления, коэфф затухания (какая часть света проходит дальше)

  surf( vec3 Ka, vec3 Kd, vec3 Ks, DBL Ph, DBL Wl = 0, DBL Kr = 1, DBL Wr = 0 ) : 
    Ka(Ka), Kd(Kd), Ks(Ks), Ph(Ph), Wl(Wl), Kr(Kr), Wr(Wr)
  {
  }

  surf( VOID )
  {
  }

};

class intr
{
  friend class shp;
public:

  ray Ray;
  DBL Param = INFINITY;
  vec3 IntrPoint;
  vec3 Norm;
  shp *Shp = nullptr;
  surf Surf;

  intr( VOID )
  {
  }

  ray getReflected( VOID )
  {
    DBL Oy = Norm & Ray.Dir;
    vec3 Dir = Ray.Dir - Norm * Oy * 2;
    return ray(IntrPoint + Dir * THRESHOLD, Dir);
  }

  ray getRefracted( VOID )
  {
    DBL Proj = Norm & Ray.Dir;
    vec3 N = Proj < 0 ? Norm : -Norm;
    DBL sin2A1 = ((-Ray.Dir) % N).len2();
    vec3 Right = Ray.Dir - Norm * Proj;
    DBL cos2A2 = (1 - Surf.Kr * Surf.Kr * sin2A1);
    vec3 Dir = -N * sqrt(cos2A2) + Right.normSelf() * sqrt(1 - cos2A2);
    return ray(IntrPoint + Dir * THRESHOLD, Dir);
  }

};

class shp
{       
  friend class intr;
public:

  matr Matr;
  surf Surf;

  shp( VOID )
  {
  }

  intr Intersection( ray &Ray )
  {
    intr Intr;

    ray Local(Matr.inverse() * Ray.Start, Matr.inverse() * Ray.Dir);
    Intr.Ray = Ray;
    Intr.IntrPoint = Matr * Intr.IntrPoint;
    Intr.Norm = (Matr.inverse() * Intr.Norm).norm();
    Intr.Param = !(Intr.IntrPoint - Ray.Start);
    if (!__GetIntersection(Local, Intr))
    {
      Intr.Shp = nullptr;
      Intr.Param = INFINITY;
    }
    return Intr;
  }

  vec3 GetNorm( vec3 Point )
  {
    return (Matr.inverse() * __GetNorm(Matr.inverse() * Point)).norm();
  }

  BOOL IsInside( vec3 Point )
  {
    return __IsInside(Matr.inverse() * Point);
  }

  std::vector<intr> AllIntersections( ray &Ray )
  {
    std::vector<intr> Intrs;

    ray Local(Matr.inverse() * Ray.Start, Matr.inverse() * Ray.Dir);
    __GetAllIntersections(Local, Intrs);
    for (intr i : Intrs)
    {
      i.Ray = Ray;
      i.IntrPoint = Matr * i.IntrPoint;
      i.Norm = (Matr.inverse() * i.Norm).norm();
      i.Param = !(i.IntrPoint - Ray.Start);
    }
    return Intrs;
  }

  static VOID SetIntr( intr &Intr, ray &Ray, DBL Param, shp *Shp )
  {
    Intr.Shp = Shp;
    Intr.Param = Param;
    Intr.IntrPoint = Ray[Param];
    Intr.Norm = Shp->__GetNorm(Intr.IntrPoint);
    Intr.Ray = Ray;
    Intr.Surf = Shp->Surf;
  }

private:

  /* local coordinate system */ 
  virtual BOOL __GetIntersection( ray &Ray, intr &Intr )
  {
    return FALSE;
  }

  /* local coordinate system */ 
  virtual BOOL __GetAllIntersections( ray &Ray, std::vector<intr> &Intrs )
  {
    return FALSE;
  }

  /* local coordinate system */ 
  virtual vec3 __GetNorm( vec3 Point )
  {
    return vec3();
  }

  /* local coordinate system */ 
  virtual BOOL __IsInside( vec3 Point )
  {
    return FALSE;
  }

public:
  virtual ~shp( VOID )
  {
  }

};


#endif /* __shp_base_h_ */
