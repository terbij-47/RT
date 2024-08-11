#include "../def.h"
#include "shapes/shp.h"
#include "lights/light_base.h"

#ifndef __rt_h_
#define __rt_h_

class rt
{
  // coord Coords;
  stock<shp *> Shapes;
  stock<light *> Lights;
  vec3 BackgroundColor = vec3(0.3);
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

  vec3 Trace( ray & Ray, intr *Intr = nullptr )
  {
    intr Closest;
    RecLevel++;
    for (auto sh : Shapes)
    {
      intr Intr = sh->Intersection(Ray);
      if (Intr.Shp && Intr.Param <= Closest.Param)
        Closest = Intr;
    }
    if (Closest.Shp)
    {
      if (Intr)
        *Intr = Closest;
      return Shade(Closest);
    }
    RecLevel--;
    return vec3();
  }

protected:
  vec3 Shade( intr &Intr )
  {
    vec3 ResColor(0);
    ray Reflected = Intr.getReflected();

    // ambient
    ResColor += Intr.Surf.Ka * BackgroundColor;

    // diffise and specular
    vec3 Diffuse(0);
    vec3 Specular(0);

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

    if (RecLevel < MaxRecLevel)
    {
      intr IntrTmp;
      vec3 ReflectedColor = Trace(Reflected, &IntrTmp);
      ResColor += ReflectedColor * Intr.Surf.Wl * exp(-IntrTmp.Param * (1 - Intr.Surf.Wl) * 1.5);

      IntrTmp = intr();
      ray Refracted = Intr.getRefracted();
      vec3 RefractedColor = Trace(Refracted, &IntrTmp);
      ResColor += RefractedColor * Intr.Surf.Wr * exp(-IntrTmp.Param * (1 - Intr.Surf.Wr) * 1.5);
    }
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

