#include "mthdef.h"

#ifndef __vec4_h_
#define __vec4_h_

template <typename Type = DBL>
class mth::vec4
{
protected:
  Type x, y, z, w = 1;

public:

  vec4( Type x, Type y, Type z, Type w ) : x(x), y(y), z(z), w(w)
  {
  }

  vec4( vec3<Type> v ) : x(v.x), y(v.y), z(v.z), w(1)
  {
  }

  vec4<Type> operator+( vec4<Type> & v )
  {
    return vec4<Type>(x + v.x, y + v.y, z + v.z, w + v.w);
  }

  vec4<Type> & operator+=( vec4<Type> & v )
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w
    return *this;
  }

  vec4<Type> operator-( vec4<Type> & v )
  {
    return vec4<Type>(x - v.x, y - v.y, z - v.z, w - v.w);
  }

  vec4<Type> & operator-=( vec4<Type> & v )
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w
    return *this;
  }

  Type operator!( VOID )
  {
    return sqrt(x * x + y * y + z * z + w * w);
  }

  Type operator&( vec4<Type> & v )
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

};

#endif /* __vec4_h_ */
