#include "mthdef.h"
#include "vec3.h"

#ifndef __coord_system_h_
#define __coord_system_h_

class mth::coordSystem
{
public:
  enum class CoordType
  {
    CARTESIAN, CYLIDRICAL, SPHERICAL,
  } CoordSystemType;

  coordSystem( VOID ) : CoordSystemType(CoordType::CARTESIAN)
  {
  }

  vec3<DBL> toCart( vec3<DBL> Vec )
  {
    switch (CoordSystemType)
    {
    case CoordType::CARTESIAN:
      return Vec;
    case CoordType::CYLIDRICAL:
      return vec3<DBL>(Vec[0] * cos(Vec[1]), Vec[0] * sin(Vec[1]), Vec[2]);
    case CoordType::SPHERICAL: // R, phi, theta
      return vec3<DBL>(Vec[0] * sin(Vec[1]) * cos(Vec[2]),
        Vec[0] * sin(Vec[1]) * sin(Vec[2]), Vec[0] * cos(Vec[2]));
    }
  }

};

#endif /* __coord_system_h_ */
