#include "mthdef.h"

#ifndef __mth_vec2_h_
#define __mth_vec2_h

template<typename Type = DBL>
class mth::vec2
{
protected:
  Type x, y;

public:
  vec2(Type x, Type y) : x(x), y(y)
  {
  }

  vec2( VOID ) : x(0), y(0)
  {
  }

  vec2(Type a) : x(a), y(a)
  {
  }

  vec2<Type> operator+(const vec2<Type> &v) const
  {
    return vec2<Type>(x + v.x, y + v.y);
  }

  vec2<Type> operator-(const vec2<Type> &v) const
  {
    return vec2<Type>(x - v.x, y - v.y);
  }

  vec2<Type> & operator+=(const vec2<Type> &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  vec2<Type> & operator-=(const vec2<Type> &v)
  {
    x -= v.x;
    y -= v.x;
    return *this;
  }

  vec2<Type> operator*(const Type N) const
  {
    return vec2<Type>(x * N, y * N);
  }

  vec2<Type> operator/(const Type N) const
  {
    return vec2<Type>(x / N, y / N);
  }

  vec2<Type> & operator*=(const Type N)
  {
    x *= N;
    y *= N;
    return *this;
  }

  vec2<Type> operator*( const vec2<Type> v )
  {
    return vec2<Type>(x * v.x, y * v.y);
  }

  vec2<Type> & operator*=( const vec2<Type> v )
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  vec2<Type> & operator/=(const Type N)
  {
    x /= N;
    y /= N;
    return *this;
  }

  Type len2( VOID ) const
  {
    return x * x + y * y;
  }

  Type operator!( VOID ) const
  {
    return sqrt(x * x + y * y);
  }

  vec2<Type> norm( VOID ) const
  {
    Type l = sqrt(x * x + y * y);
    return vec2<Type>(x / l, y / l);
  }

  vec2<Type> & normSelf( VOID )
  {
    Type l = sqrt(x * x + y * y);
    x /= l;
    y /= l;
    return *this;
  }

  vec2<Type> right( VOID )
  {
    return vec2<Type>(y, -x);
  }

  static vec2<Type> rnd( Type Max = 1 )
  {
    return vec2<Type>(rand() * Max / RAND_MAX, rand() * Max / RAND_MAX);
  }

  Type operator&( const vec2<Type> &v ) const
  {
    return x * v.x + y * v.y;
  }

  Type operator[](const INT i) const
  {
    switch(i)
    {
    case 1:
      return y;
    case 0:
      return x;
    }
    return INFINITY;
  }
};

#endif /* __mth_vec2_h_ */

