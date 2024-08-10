#include "shp_base.h"

#ifndef __quadric_h_
#define __quadric_h_

class quadric : public shp
{
public:

  // matrix
  DBL A, B, C;
  DBL D, E, F;
  DBL G, H, I, J;

  // a b c d
  // b e f g
  // c f h i
  // d g i j
  quadric( DBL A, DBL B, DBL C,
           DBL D, DBL E, DBL F,
           DBL G, DBL H, DBL I, DBL J,  matr Matr = matr() ) : A(A), B(B), C(C),
                                              D(D), E(E), F(F),
                                              G(G), H(H), I(I), J(J)
  {
    this->Matr = Matr;
  }

  BOOL __GetIntersection( ray &Ray, intr &Intr ) override
  {
    DBL Dx = Ray.Dir[0];
    DBL Dy = Ray.Dir[1];
    DBL Dz = Ray.Dir[2];
    DBL Px = Ray.Start[0];
    DBL Py = Ray.Start[1];
    DBL Pz = Ray.Start[2];

    DBL a = A*Dx*Dx + 2*B*Dy*Dx + 2*C*Dz*Dx + E*Dy*Dy + 2*F*Dy*Dz + H*Dz*Dz;
    DBL b = 2*(A*Px*Dx + B*(Px*Dy + Dx*Py) + C*(Px*Dz + Dx*Pz) + D*Dx + E*Py*Dy +
      + F*(Py*Dz + Dy*Pz) + G*Dy + H*Pz*Dz + I*Dz);
    
    DBL c = A*Px*Px + 2*B*Py*Px + 2*C*Pz*Px + 2*D*Px + E*Py*Py + 2*F*Py*Pz + 
      + 2*G*Py + H*Pz*Pz + 2*I*Pz + J;

    if (a == 0 && b != 0)
    {
      DBL t = -c / b;
      shp::SetIntr(Intr, Ray, t, this);
      return TRUE;
    }
    DBL determ = b * b - 4 * a * c;
    if (determ < 0)
      return FALSE;
    DBL root = sqrt(determ);
    DBL t1 = (-b - root) / 2 / a;
    DBL t2 = (-b + root) / 2 / a;
    if (t2 < 0)
      return FALSE;
    shp::SetIntr(Intr, Ray, t1 < 0 ? t2 : t1, this);
    return TRUE;
  }

  BOOL __GetAllIntersections( ray &Ray, std::vector<intr> &Intrs ) override
  {
    DBL Dx = Ray.Dir[0];
    DBL Dy = Ray.Dir[1];
    DBL Dz = Ray.Dir[2];
    DBL Px = Ray.Start[0];
    DBL Py = Ray.Start[1];
    DBL Pz = Ray.Start[2];

    DBL a = A*Dx*Dx + 2*B*Dy*Dx + 2*C*Dz*Dx + E*Dy*Dy + 2*F*Dy*Dz + H*Dz*Dz;
    DBL b = 2*(A*Px*Dx + B*(Px*Dy + Dx*Py) + C*(Px*Dz + Dx*Pz) + D*Dx + E*Py*Dy +
      + F*(Py*Dz + Dy*Pz) + G*Dy + H*Pz*Dz + I*Dz);
    
    DBL c = A*Px*Px + 2*B*Py*Px + 2*C*Pz*Px + 2*D*Px + E*Py*Py + 2*F*Py*Pz + 
      + 2*G*Py + H*Pz*Pz + 2*I*Pz + J;

    if (a == 0 && b != 0)
    {
      Intrs.resize(1);
      DBL t = -c / b;
      shp::SetIntr(Intrs[0], Ray, t, this);
      return TRUE;
    }
    DBL determ = b * b - 4 * a * c;
    if (determ < 0)
      return FALSE;
    DBL root = sqrt(determ);
    DBL t1 = (-b - root) / 2 / a;
    DBL t2 = (-b + root) / 2 / a;
    if (t2 < 0)
      return FALSE;
    if (t1 < 0 || (t2 - t1) < THRESHOLD)
    {
      Intrs.resize(1);
      shp::SetIntr(Intrs[0], Ray, t2, this);
      return TRUE;
    }
    Intrs.resize(2);
    shp::SetIntr(Intrs[0], Ray, t1, this);
    shp::SetIntr(Intrs[1], Ray, t2, this);
    return TRUE;
  }

  vec3 __GetNorm( vec3 Point ) override
  {
    DBL x = Point[0];
    DBL y = Point[1];
    DBL z = Point[2];
    return vec3(2*x*A + 2*B*y + 2*C*z + 2*D,
                2*B*x + 2*E*y + 2*F*z + 2*G,
                2*C*x + 2*F*y + 2*z*H + 2*I).norm();
  }

  BOOL __IsInside( vec3 Point ) override
  {
    DBL x = Point[0];
    DBL y = Point[1];
    DBL z = Point[2];
    return IS_ZERO(A*x*x + 2*B*x*y + 2*C*x*z + 2*D*x + E*y*y + 
      2*F*y*z + 2*G*y + H*z*z + 2*I*z + J);
  }
};

#endif /* __sphere_h_ */


