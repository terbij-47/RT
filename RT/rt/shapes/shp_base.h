#include "../../def.h"

#ifndef __shp_base_h_
#define __shp_base_h_

struct surf
{
  vec3 Ka, Kd, Ks;
  DBL Ph = 10;

  surf( vec3 Ka, vec3 Kd, vec3 Ks, DBL Ph ) : Ka(Ka), Kd(Kd), Ks(Ks), Ph(Ph)
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

  vec3 getReflected( VOID )
  {
  }

  vec3 getRefracted( VOID )
  {
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
    Intr.Norm = Shp->GetNorm(Intr.IntrPoint);
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
