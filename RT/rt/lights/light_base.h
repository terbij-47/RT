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
    vec3 Start = Intr.IntrPoint + Intr.Norm * THRESHOLD;
    ray Ray(Start, GetDir(Start));
    for (auto sh : Shapes)
    {
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

class projLight : public light
{
protected:
  vec3 Pos;
  vec3 Dir;
  DBL Alpha, CosAlpha;

public:
  projLight( vec3 Pos, vec3 Dir, DBL AlphaInDegree, vec3 Color = vec3(1), DBL Cl = 0, DBL Cc = 0, DBL Cq = 0 ) :
    Pos(Pos), Dir(Dir), Alpha(D2R(AlphaInDegree))
  {
    this->Color = Color;
    this->Cl = Cl;
    this->Cc = Cc;
    this->Cq = Cq;
    CosAlpha = cos(Alpha);
  }

  vec3 GetDir( vec3 Point ) override
  {
    return (Pos - Point).norm();
  }

  DBL GetAttenuation( intr &Intr, stock<shp*> & Shapes ) override
  {
    INT Fl = ((Intr.IntrPoint - Pos).norm() & Dir) > CosAlpha ? 1 : 0;
    DBL dist2 = (Intr.IntrPoint - Pos).len2();
    return min(1 / (Cc + Cl * sqrt(dist2) + Cq * dist2), 1) * GetShadowCoeff(Intr, Shapes) * Fl;
  }
};

class softLight : public light
{
  BOOL IsRand = 0;

protected:
  vec3 Pos;
  DBL R;

public:
  softLight( vec3 Pos, DBL R = 1, vec3 Color = vec3(1), DBL Cl = 0, DBL Cc = 0, DBL Cq = 0 ) :
    Pos(Pos), R(R)
  {
    this->Color = Color;
    this->Cl = Cl;
    this->Cc = Cc;
    this->Cq = Cq;
  }

  vec3 GetDir( vec3 Point ) override
  {
    vec3 Dir = (Pos - Point).norm();
    if (!IsRand)
      return Dir;
    vec3 Right = Dir % vec3(1, 0, 0);
    if (IS_ZERO(Right.len2()))
      Right = Dir % vec3(0, 1, 0);
    Right.normSelf();
    vec3 Up = Dir % Right;
    vec2 RndParam = vec2::rnd() * vec2(2 * PI, R);
    vec3 Offset = Right * cos(RndParam[0]) * RndParam[1] + 
      Up * sin(RndParam[0]) * RndParam[1];
    return (Pos + Offset - Point).norm();
  }

  DBL GetAttenuation( intr &Intr, stock<shp*> & Shapes ) override
  {
    DBL dist2 = (Intr.IntrPoint - Pos).len2();
    IsRand = 1;
    DBL Shd = 0;
    INT Times = 100;
    for (INT i = 0; i < Times; i++)
      Shd += GetShadowCoeff(Intr, Shapes);
    IsRand = 0;

    return min(1 / (Cc + Cl * sqrt(dist2) + Cq * dist2), 1) * Shd / Times;

  }
};


class reflectorLight : public light
{
protected:
  vec3 Pos;
  vec3 Dir;

public:
  reflectorLight( vec3 Pos, vec3 Dir, vec3 Color = vec3(1), DBL Cl = 0, DBL Cc = 0, DBL Cq = 0 ) :
    Pos(Pos), Dir(Dir)
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
    DBL Fl = max((Intr.IntrPoint - Pos).norm() & Dir, 0);
    DBL dist2 = (Intr.IntrPoint - Pos).len2();
    return min(1 / (Cc + Cl * sqrt(dist2) + Cq * dist2), 1) * GetShadowCoeff(Intr, Shapes) * Fl;
  }
};
 

#endif
