#include "mthdef.h"

#ifndef __mth_vec3_h_
#define __mth_vec3_h

template<typename Type = DBL>
class mth::vec3
{
public:
  Type x, y, z;

  vec3(Type x, Type y, Type z) : x(x), y(y), z(z)
  {
  }

  vec3( VOID ) : x(0), y(0), z(0)
  {
  }

  vec3(Type a) : x(a), y(a), z(a)
  {
  }

  vec3<Type> operator+(const vec3<Type> &v) const
  {
    return vec3<Type>(x + v.x, y + v.y, z + v.z);
  }

  vec3<Type> operator-(const vec3<Type> &v) const
  {
    return vec3<Type>(x - v.x, y - v.y, z - v.z);
  }

  vec3<Type> & operator+=(const vec3<Type> &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  vec3<Type> & operator-=(const vec3<Type> &v)
  {
    x -= v.x;
    y -= v.x;
    z -= v.z;
    return *this;
  }

  vec3<Type> operator*(const Type N) const
  {
    return vec3<Type>(x * N, y * N, z * N);
  }

  vec3<Type> operator*(const vec3<Type> &v) const
  {
    return vec3<Type>(x * v.x, y * v.y, z * v.z);
  }

  vec3<Type> & operator*=( const vec3<Type> &v )
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  vec3<Type> operator/(const Type N) const
  {
    return vec3<Type>(x / N, y / N, z / N);
  }

  vec3<Type> & operator*=(const Type N)
  {
    x *= N;
    y *= N;
    z *= N;
    return *this;
  }

  vec3<Type> & operator/=(const Type N)
  {
    x /= N;
    y /= N;
    z /= N;
    return *this;
  }

  vec3<Type> operator-( VOID )
  {
    return vec3<Type>(-x, -y, -z);
  }

  Type len2( VOID ) const
  {
    return x * x + y * y + z * z;
  }

  Type operator!( VOID ) const
  {
    return sqrt(x * x + y * y + z * z);
  }

  vec3<Type> norm( VOID ) const
  {
    Type l = sqrt(x * x + y * y + z * z);
    return vec3<Type>(x / l, y / l, z / l);
  }

  vec3<Type> & normSelf( VOID )
  {
    Type l = sqrt(x * x + y * y + z * z);
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  vec3<Type> operator%( const vec3<Type> &v ) const
  {
    return vec3<Type>(y * v.z - v.y * z, -x * v.z + v.x * z, x * v.y - v.x * y);
  }

  Type operator&( const vec3<Type> &v ) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  Type operator[](const INT i) const
  {
    switch(i)
    {
    case 2:
      return z;
    case 1:
      return y;
    case 0:
      return x;
    }
    return INFINITY;
  }

  static vec3 rnd( Type Max = 1 )
  {
    return vec3(rand() * Max / RAND_MAX, rand() * Max / RAND_MAX, rand() * Max / RAND_MAX);
  }
};

#endif /* __mth_vec2_h_ */

