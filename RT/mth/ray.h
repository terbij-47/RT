#include "mthdef.h"

#ifndef __ray_h_
#define __ray_h_

template<typename Type = vec3>
class mth::ray
{
public:

  Type Start;
  Type Dir;

  ray( Type Start, Type Dir ) : Start(Start)
  {
    this->Dir = Dir.norm();
  }

  ray( VOID )
  {
  }

  Type operator[]( DBL Param )
  {
    return Start + Dir * Param;
  }
};

#endif /* __ray_h_ */
