#include "../def.h"
#include "shapes/shp.h"
#include "lights/light_base.h"

#ifndef __rt_h_
#define __rt_h_


struct envi
{
  DBL RefractionCoeff = 1;
  vec3 BackgroundColor = vec3(0.01);
  // fog params
  BOOL IsFog = 0;
  DBL FogStart = 0.1;
  DBL FogDensity = 0.05;
  vec3 FogColor = vec3(0.6) + vec3(0, 0, 0.3);

  envi( VOID ){}

  DBL GetFogCoeff( DBL dist )  // сколько света проходит сквозь туман
  {
    if (dist < FogStart || !IsFog)
      return 1;
    return exp(-(dist - FogStart) * FogDensity);
  }
};

class rt
{
  // coord Coords;
  stock<shp *> Shapes;
  stock<light *> Lights;
  envi Environment;
  INT RecLevel = 0;
  const INT MaxRecLevel;

public:

  rt( INT MaxRec = 5 ) : MaxRecLevel(MaxRec)
  {
  }

  rt & operator<<( shp *Shp )
  {
    Shapes << Shp;
    return *this;
  }

  rt & operator<<( light *L )
  {
    Lights << L;
    return *this;
  }

  vec3 Trace( ray Ray, intr *Intr = nullptr )
  {
    intr Closest;
    RecLevel++;
    for (auto sh : Shapes)
    {
      intr Intr = sh->Intersection(Ray);
      if (Intr.Shp && Intr.Param <= Closest.Param)
        Closest = Intr;
    }
    if (Intr)
      *Intr = Closest;
    return Shade(Closest);
  }

protected:
  vec3 Shade( intr &Intr )
  {
    vec3 ResColor = Environment.BackgroundColor;

    if (Intr.Shp)
    {
      // ambient
      ResColor *= Intr.Surf.Ka;
    
      // diffise and specular
      vec3 Diffuse;
      vec3 Specular;
      ray Reflected = Intr.getReflected();
    
      for (auto l : Lights)
      {
        vec3 Dir = l->GetDir(Intr.IntrPoint);
        DBL Attenuation = l->GetAttenuation(Intr, Shapes);
    
        DBL cos1 = Intr.Norm & Dir;
        Diffuse += (l->Color * (cos1 < 0 ? 0 : cos1)) * Attenuation;
    
        // blinn-phong
        DBL cos2 = Intr.Norm & ((l->GetDir(Intr.IntrPoint) - Intr.Ray.Dir).norm());
        Specular += (l->Color * pow((cos2 < 0 ? 0 : cos2), Intr.Surf.Ph)) * Attenuation;
      }
      ResColor += Intr.Surf.Kd * Diffuse;
      ResColor += Intr.Surf.Ks * Specular;
    
      // reflection + refraction + shadows
      if (RecLevel < MaxRecLevel)
      {
        intr IntrTmp;
        vec3 ReflectedColor = Trace(Reflected, &IntrTmp);
        ResColor += ReflectedColor * Intr.Surf.Wl * exp(-IntrTmp.Param * (1 - Intr.Surf.Wl) * 1.5);
    
        IntrTmp = intr();
      
        // get refraction ray
        DBL Proj = Intr.Norm & Intr.Ray.Dir;
        vec3 N = Proj < 0 ? Intr.Norm : -Intr.Norm;
        DBL RefCoeff2 = Intr.Shp->IsInside(Intr.IntrPoint + N * THRESHOLD) ? 
          Intr.Surf.Kr * Intr.Surf.Kr / (Environment.RefractionCoeff * Environment.RefractionCoeff) :
          Environment.RefractionCoeff * Environment.RefractionCoeff  / (Intr.Surf.Kr * Intr.Surf.Kr);
        DBL sin2A1 = ((-Intr.Ray.Dir) % N).len2();
        vec3 Right = Intr.Ray.Dir - Intr.Norm * Proj;
        DBL cos2A2 = (1 - RefCoeff2 * sin2A1);
        vec3 Dir = -N * sqrt(cos2A2) + Right.normSelf() * sqrt(1 - cos2A2);
        ray Refracted(Intr.IntrPoint + Dir * THRESHOLD, Dir);
    
        vec3 RefractedColor = Trace(Refracted, &IntrTmp);
        ResColor += RefractedColor * Intr.Surf.Wr * exp(-IntrTmp.Param * (1 - Intr.Surf.Wr) * 1.5);
      }
    }

    // use fog
    DBL FogCoeff = Environment.GetFogCoeff(Intr.Param);
    ResColor = ResColor * FogCoeff + Environment.FogColor * (1 - FogCoeff);

    RecLevel--;
    return ResColor;
  }

public:
  ~rt()
  {
    for (auto x : Shapes)
      delete x;
    for (auto x : Lights)
      delete x;
  }
};



#endif /* __rt_h_ */

