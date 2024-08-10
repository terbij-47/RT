#include "../def.h"
#include "shapes/shp.h"
// #include "lights/light_base.h"

#ifndef __rt_h_
#define __rt_h_

class rt
{
public:

  // coord Coords;
  stock<shp *> Shapes;
//  stock<light *> Lights;

  vec3 Trace( ray & Ray )
  {
    intr Closest;

    for (auto sh : Shapes)
    {
      intr Intr = sh->Intersection(Ray);
      if (Intr.Shp && Intr.Param <= Closest.Param)
        Closest = Intr;
    }
    return Shade(Closest);
  }

  vec3 Shade( intr &Intr )
  {
    return Intr.Norm;
  }

  rt & operator<<( shp *Shp )
  {
    Shapes << Shp;
    return *this;
  }

  ~rt()
  {
    for (auto x : Shapes)
      delete x;
  }
};



#endif /* __rt_h_ */

