#include "shp_base.h"

#ifndef __csg_h_
#define __csg_h_

class csg : public shp
{
  shp *Shp1;
  shp *Shp2;
  UINT Type;

public:

  enum class type : UINT
  {
    UNION,
    SUB,
    INTER
  };

  csg( shp *Shp1, shp *Shp2, UINT Type, surf Surf, matr Matr = matr() ) : Shp1(Shp1), Shp2(Shp2), Type(Type)
  {
    this->Matr = Matr;
    this->Surf = Surf;
  }

private:
  /* in local coordinate system */
  BOOL __GetIntersection( ray &Ray, intr &Intr ) override
  {
    switch (Type)
    {
    case (UINT)type::INTER:
    {
      Intr.Param = INFINITY;
      std::vector<intr> Intrs1 = Shp1->AllIntersections(Ray);
      std::vector<intr> Intrs2 = Shp2->AllIntersections(Ray);
      if (Intrs1.size() + Intrs2.size() == 0)
        return FALSE;
      
      for (intr i : Intrs1)
        if (Shp2->IsInside(i.IntrPoint))
        {
          Intr = i;
          break;
        }
      for (intr i : Intrs2)
        if (Shp1->IsInside(i.IntrPoint))
        {
          if (i.Param < Intr.Param)
            Intr = i;
          break;
        }
      if (Intr.Param == INFINITY)
        return FALSE;
      Intr.Shp = this;
      return TRUE;
    }

    case (UINT)type::SUB:  // Shp1 - Shp2
    {
      std::vector<intr> Intrs1 = Shp1->AllIntersections(Ray);
      std::vector<intr> Intrs2 = Shp2->AllIntersections(Ray);
      Intr.Param = INFINITY;

      if (Intrs1.size() + Intrs2.size() == 0)
        return FALSE;
      for (auto a : Intrs1)
        if (!Shp2->IsInside(a.IntrPoint))
        {
          Intr = a;
          break;
        }
      for (auto a : Intrs2)
        if (Shp1->IsInside(a.IntrPoint))
        {
          if (a.Param < Intr.Param)
            Intr = a;
          break;
        }
      if (Intr.Param == INFINITY)
        return FALSE;
      Intr.Shp = this;
      return TRUE;
    }

    case (UINT)type::UNION:
      intr Intr1 = Shp1->Intersection(Ray);
      intr Intr2 = Shp2->Intersection(Ray);
      Intr = Intr1.Param < Intr2.Param ? Intr1 : Intr2;
      Intr.Shp = this;
      return Intr1.Shp || Intr2.Shp;

    }
    return FALSE;

  }

  BOOL __GetAllIntersections( ray &Ray, std::vector<intr> &Intrs ) override
  {
    return FALSE;
  }

  /* In local coordinate system */ 
  vec3 __GetNorm( vec3 Point ) override
  {
    switch (Type)
    {
    case (UINT)type::UNION:
      if (Shp1->IsInside(Point))
        return Shp1->GetNorm(Point);
      else if (Shp2->IsInside(Point)) 
        return Shp2->GetNorm(Point);

    case (UINT)type::SUB:
      return vec3();

    case (UINT)type::INTER:
      return vec3();
    }
    return vec3();
  }

  /* In local coordinate system */
  BOOL __IsInside( vec3 Point ) override
  {
    switch (Type)
    {
    case (UINT)type::UNION:
      return Shp1->IsInside(Point) || Shp2->IsInside(Point);

    case (UINT)type::SUB:
      return Shp1->IsInside(Point) && !(Shp2->IsInside(Point));

    case (UINT)type::INTER:
      return Shp1->IsInside(Point) && Shp2->IsInside(Point);
    }
    return FALSE;
  }

  ~csg( VOID ) override
  {
    delete Shp1;
    delete Shp2;
  }

};

#endif
