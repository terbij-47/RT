#include "mthdef.h"

#ifndef __compl_h_
#define __compl_h_

template <typename Type = DBL> 
class mth::complex
{
protected:
  Type re, im;
  Type len = 0, phi = 0;

public:
  complex( Type re, Type im) : re(re), im(im)
  {
  }

  complex( Type re ) : re(re), im(0)
  {
  }

  complex( VOID )
  {
  }

  complex<Type> operator+( const complex<Type> z )
  {
    return complex<Type>(re + z.re, im + z.im);
  }

  complex<Type> operator-( const complex<Type> z )
  {
    return complex<Type>(re - z.re, im - z.im);
  }

  complex<Type> & operator+=( complex<Type> z )
  {
    re += z.re;
    im += z.im;
    return *this;
  }

  complex<Type> & operator-=( complex<Type> z )
  {
    re -= z.re;
    im -= z.im;
    return *this;
  }


  complex<Type> operator*( complex<Type> z )
  {
    return complex<Type>(re * z.re - im * z.im, re * z.im + im * z.re);
  }

  complex<Type> operator*( Type N )
  {
    return complex<Type>(re * N, im * N);
  }

  complex<Type> & operator*=( complex<Type> z )
  {
    re = re * z.re - im * z.im;
    im = re * z.im + im * z.re;
    return *this;
  }

  BOOL operator==( complex<Type> z )
  {
    return re == z.re && im == z.im;
  }

  Type operator!( VOID )
  {
    len = sqrt(re * re + im * im);
    phi = atan2(re, im);
    return len;
  }

  complex<Type> pow( INT n )
  {
    Type l = !(*this);
    l = std::pow(l, n);
    return complex<Type>(l * cos(n * phi), l * sin(n * phi));
  }

  std::vector<complex<Type>> root( INT n )
  {
    Type l = !(*this);
    l = std::pow(l, 1 / n);
    std::vector<complex<Type>> Res;
    for (INT i = 0; i < n; i++)
      Res.push_back(complex<Type>(l * cos((phi + 2 * PI * i) / n), 
                                  l * sin((phi + 2 * PI * i) / n)));
    return Res;
  }

};


#endif /* __complex_h_ */
