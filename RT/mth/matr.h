#include "mthdef.h"

#ifndef __matr_h_
#define __matr_h_

template <typename Type = DBL>
class mth::matr   /* используетс€ вектор-столбец */ 
{
protected:
  Type m[4][4];
  Type inv[4][4];
  // matr<Type> inv;
  BOOL isInverse = FALSE;

public:
  matr(Type A00, Type A01, Type A02, Type A03,
       Type A10, Type A11, Type A12, Type A13,
       Type A20, Type A21, Type A22, Type A23,
       Type A30, Type A31, Type A32, Type A33) : m{A00, A01, A02, A03, 
                                                   A10, A11, A12, A13, 
                                                   A20, A21, A22, A23,
                                                   A30, A31, A32, A33}, isInverse(FALSE)
  {
  }

  matr( VOID ) : m{1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1}, isInverse(FALSE)
  {
  }

  matr( const Type * A ) : isInverse(FALSE)
  {
    m[0][0] = A[0], m[0][1] = A[1], m[0][2] = A[2], m[0][3] = A[3],
    m[1][0] = A[4], m[1][1] = A[5], m[1][2] = A[6], m[1][3] = A[7],
    m[2][0] = A[8], m[2][1] = A[9], m[2][2] = A[10], m[2][3] = A[11],
    m[3][0] = A[12], m[3][1] = A[13], m[3][2] = A[14], m[3][3] = A[15];
  }

  static matr<Type> scale( const vec3<Type> &s )
  {
    return matr<Type>(s.x,   0,   0, 0,
                        0, s.y,   0, 0,
                        0,   0, s.z, 0,
                        0,   0,   0, 1);
  }

  static matr<Type> rotateZ( const Type angleInDegree )
  {
    DBL a = (DBL)D2R(angleInDegree);
    DBL si = sin(a);
    DBL co = cos(a);

    return matr<Type>(co, -si, 0, 0,
                      si,  co, 0, 0,
                       0,   0, 1, 0,
                       0,   0, 0, 1);
  }

  static matr<Type> rotateX( const Type angleInDegree )
  {
    DBL a = (DBL)D2R(angleInDegree);
    DBL si = sin(a);
    DBL co = cos(a);

    return matr<Type>(1,  0,   0, 0,
                      0, co, -si, 0,  
                      0, si,  co, 0,    
                      0,  0,   0, 1);
  }

  static matr<Type> rotateY( const Type angleInDegree )
  {
    DBL a = (DBL)D2R(angleInDegree);
    DBL si = sin(a);
    DBL co = cos(a); 

    return matr<Type>( co, 0, si, 0,
                        0, 1,  0, 0,
                      -si, 0, co, 0,
                        0, 0,  0, 1);
  }                  

  static matr<Type> translate( const vec3<Type> &t )
  {
    return matr<Type>(1, 0, 0, t.x, 
                      0, 1, 0, t.y,
                      0, 0, 1, t.z,
                      0, 0, 0,   1);
  }

  /* матрица * вектор */
  vec3<Type> operator*( const vec3<Type> & v )
  {
    return vec3<Type>(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
                      m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                      m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
  }

  matr<Type> trans( VOID )
  {
    return matr<Type>(m[0][0], m[1][0], m[2][0], m[3][0],
                      m[0][1], m[1][1], m[2][1], m[3][1],
                      m[0][2], m[1][2], m[2][2], m[3][2],
                      m[0][3], m[1][3], m[2][3], m[3][3]);
  }

  matr operator*(const matr &M2) const
  {

    matr r = {0};

    for (INT i = 0; i < 4; i++)
      for (INT j = 0; j < 4; j++)
        for (INT k = 0; k < 4; k++)
          r.m[i][j] += m[i][k] * M2.m[k][j];
    return r;
  }

  Type determ3x3(const Type A11, const Type A12, const Type A13,
                 const Type A21, const Type A22, const Type A23,
                 const Type A31, const Type A32, const Type A33) const
  {
    return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
      A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
  }

  Type operator!( VOID ) const
  {
    return
      +m[0][0] * determ3x3(m[1][1], m[1][2], m[1][3],
                           m[2][1], m[2][2], m[2][3],
                           m[3][1], m[3][2], m[3][3]) +
      -m[0][1] * determ3x3(m[1][0], m[1][2], m[1][3],
                           m[2][0], m[2][2], m[2][3],
                           m[3][0], m[3][2], m[3][3]) +
      +m[0][2] * determ3x3(m[1][0], m[1][1], m[1][3],
                           m[2][0], m[2][1], m[2][3],
                           m[3][0], m[3][1], m[3][3]) +
      -m[0][3] * determ3x3(m[1][0], m[1][1], m[1][2],
                           m[2][0], m[2][1], m[2][2],
                           m[3][0], m[3][1], m[3][2]);
  }

  matr inverse( VOID )
  {
    if (isInverse)
      return matr((Type *)inv);

    Type det = !(*this);

    if (det == 0)
      return matr();

    // build adjoint matrix 
    inv[0][0] =
      +determ3x3(m[1][1], m[1][2], m[1][3],
                 m[2][1], m[2][2], m[2][3],
                 m[3][1], m[3][2], m[3][3]) / det;
    inv[1][0] =
      -determ3x3(m[1][0], m[1][2], m[1][3],
                 m[2][0], m[2][2], m[2][3],
                 m[3][0], m[3][2], m[3][3]) / det;
    inv[2][0] =
      +determ3x3(m[1][0], m[1][1], m[1][3],
                 m[2][0], m[2][1], m[2][3],
                 m[3][0], m[3][1], m[3][3]) / det;
    inv[3][0] =
      -determ3x3(m[1][0], m[1][1], m[1][2],
                 m[2][0], m[2][1], m[2][2],
                 m[3][0], m[3][1], m[3][2]) / det;

    inv[0][1] =
      -determ3x3(m[0][1], m[0][2], m[0][3],
                 m[2][1], m[2][2], m[2][3],
                 m[3][1], m[3][2], m[3][3]) / det;
    inv[1][1] =
      +determ3x3(m[0][0], m[0][2], m[0][3],
                 m[2][0], m[2][2], m[2][3],
                 m[3][0], m[3][2], m[3][3]) / det;
    inv[2][1] =
      -determ3x3(m[0][0], m[0][1], m[0][3],
                 m[2][0], m[2][1], m[2][3],
                 m[3][0], m[3][1], m[3][3]) / det;
    inv[3][1] =
      +determ3x3(m[0][0], m[0][1], m[0][2],
                 m[2][0], m[2][1], m[2][2],
                 m[3][0], m[3][1], m[3][2]) / det;

    inv[0][2] =
      +determ3x3(m[0][1], m[0][2], m[0][3],
                 m[1][1], m[1][2], m[1][3],
                 m[3][1], m[3][2], m[3][3]) / det;
    inv[1][2] =
      -determ3x3(m[0][0], m[0][2], m[0][3],
                 m[1][0], m[1][2], m[1][3],
                 m[3][0], m[3][2], m[3][3]) / det;
    inv[2][2] =
      +determ3x3(m[0][0], m[0][1], m[0][3],
                 m[1][0], m[1][1], m[1][3],
                 m[3][0], m[3][1], m[3][3]) / det;
    inv[3][2] =
      -determ3x3(m[0][0], m[0][1], m[0][2],
                 m[2][0], m[2][1], m[2][2],
                 m[3][0], m[3][1], m[3][2]) / det;

    inv[0][3] =
      -determ3x3(m[0][1], m[0][2], m[0][3],
                 m[1][1], m[1][2], m[1][3],
                 m[2][1], m[2][2], m[2][3]) / det;
    inv[1][3] =
      +determ3x3(m[0][0], m[0][2], m[0][3],
                 m[1][0], m[1][2], m[1][3],
                 m[2][0], m[2][2], m[2][3]) / det;
    inv[2][3] =
      -determ3x3(m[0][0], m[0][1], m[0][3],
                 m[1][0], m[1][1], m[1][3],
                 m[2][0], m[2][1], m[2][3]) / det;
    inv[3][3] =
      +determ3x3(m[0][0], m[0][1], m[0][2],
                 m[1][0], m[1][1], m[1][2],
                 m[2][0], m[2][1], m[2][2]) / det;

    isInverse = TRUE;
    return matr((Type *)inv);
  }

};

#endif /* __matr_h_ */
