#include "../../def.h"

#ifndef __light_base_h_
#define __light_base_h_


class light
{
public:
  DBL Cl, Cc, Cq;
  vec3 Color;

  light( VOID )
  {
  }

  virtual vec3 GetDir( vec3 Point )
  {
    return vec3();
  }

  virtual DBL GetAttenuation( intr &Intr, stock<shp*> & Shapes )
  {
    return GetShadowCoeff(Intr, Shapes);
  }

protected:
  DBL GetShadowCoeff( intr &Intr, stock<shp*> & Shapes ) // сколько прошло дальше
  {
    DBL Coeff = 1;
    for (auto sh : Shapes)
    {
      vec3 Start = Intr.IntrPoint + Intr.Norm * THRESHOLD;
      ray Ray(Start, GetDir(Start));
      intr Intr = sh->Intersection(Ray);
      if (Intr.Shp)
        Coeff *= Intr.Surf.Wr;
    }
    return Coeff;
  }
};

class dirLight : public light
{
protected:
  vec3 Dir;

public:

  dirLight( vec3 Dir, vec3 Color = vec3(1), DBL Cl = 0, DBL Cc = 0, DBL Cq = 0 ) :
    Dir(Dir.norm())
  {
    this->Color = Color;
    this->Cl = Cl;
    this->Cc = Cc;
    this->Cq = Cq;
  }

  vec3 GetDir( vec3 Point ) override
  {
    return -Dir;
  }

};

class pointLight : public light
{
protected:
  vec3 Pos;

public:
  pointLight( vec3 Pos, vec3 Color = vec3(1), DBL Cl = 0, DBL Cc = 0, DBL Cq = 0 ) :
    Pos(Pos)
  {
    this->Color = Color;
    this->Cl = Cl;
    this->Cc = Cc;
    this->Cq = Cq;
  }

  vec3 GetDir( vec3 Point ) override
  {
    return (Pos - Point).norm();
  }

  DBL GetAttenuation( intr &Intr, stock<shp*> & Shapes ) override
  {
    DBL dist2 = (Intr.IntrPoint - Pos).len2();
    return min(1 / (Cc + Cl * sqrt(dist2) + Cq * dist2), 1) * GetShadowCoeff(Intr, Shapes);
  }

};

#endif
