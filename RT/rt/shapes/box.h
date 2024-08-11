#include "shp_base.h"

#ifndef __box_h_
#define __box_h_

class box : public shp
{
public:
  vec3 Pos;
  vec3 Up;
  vec3 Right;
  vec3 Fwd;
  DBL H;
  DBL W;
  DBL D;
protected:
  vec3 P[6];
  vec3 N[6];
  DBL t[6];
  DBL Sizes[6];

public:
  box( vec3 Pos, vec3 Up, vec3 Right, DBL H, DBL W, DBL D, surf Surf, matr Matr = matr() ) : 
    Pos(Pos), Up(Up.norm()), H(H), W(W), D(D)
  {
    this->Matr = Matr;
    this->Surf = Surf;
    Fwd = (Right % this->Up).norm();
    this->Right = this->Up % Fwd;
    // up, -up, right, -right, fwd, -fwd
    P[0] = Pos + this->Up * H / 2;
    N[0] = this->Up;
    Sizes[0] = H / 2;

    P[1] = Pos - this->Up * H / 2;
    N[1] = -this->Up;
    Sizes[1] = H / 2;

    P[2] = Pos + this->Right * W / 2;
    N[2] = this->Right;
    Sizes[2] = W / 2;

    P[3] = Pos - this->Right * W / 2;
    N[3] = -this->Right;
    Sizes[3] = W / 2;

    P[4] = Pos + this->Fwd * D / 2;
    N[4] = this->Fwd;
    Sizes[4] = D / 2;

    P[5] = Pos - this->Fwd * D / 2;
    N[5] = -this->Fwd;
    Sizes[5] = D / 2;
  }

  BOOL __GetIntersection( ray &Ray, intr &Intr ) override
  {
    INT tmp = 0;
    for (INT i = 0; i < 6; i++)
    {
      t[i] = (Ray.Dir & N[i]) == 0 ? -1 : ((P[i] - Ray.Start) & N[i]) / (Ray.Dir & N[i]);
      if (t[i] < 0)
        continue;
      vec3 Delta = Ray[t[i]] - P[i];
      DBL dist1 = abs(Delta & N[(i + 2) % 6]);
      DBL dist2 = abs(Delta & N[(i + 4) % 6]);
      if (dist1 > Sizes[(i + 2) % 6] || dist2 > Sizes[(i + 4) % 6])
        t[i] = -1;
      if ((t[i] > 0 && t[tmp] < 0) || (t[i] > 0 && t[tmp] > t[i]))
        tmp = i;
    }
    if (t[tmp] < 0)
      return FALSE;
    shp::SetIntr(Intr, Ray, t[tmp], this);
    return TRUE;
  }

  BOOL __GetAllIntersections( ray &Ray, std::vector<intr> &Intrs ) override
  {
    for (INT i = 0; i < 6; i++)
    {
      t[i] = (Ray.Dir & N[i]) == 0 ? -1 : ((P[i] - Ray.Start) & N[i]) / (Ray.Dir & N[i]);
      if (t[i] < 0)
        continue;
      vec3 Delta = Ray[t[i]] - P[i];
      DBL dist1 = abs(Delta & N[(i + 2) % 6]);
      DBL dist2 = abs(Delta & N[(i + 4) % 6]);
      if (!(dist1 > Sizes[(i + 2) % 6] || dist2 > Sizes[(i + 4) % 6]))
      {
        Intrs.push_back(intr());
        shp::SetIntr(Intrs[Intrs.size() - 1], Ray, t[i], this);
      }
    }
    if (!Intrs.size())
      return FALSE;
    std::sort(Intrs.begin(), Intrs.end(), [](intr a, intr b){
      return a.Param < b.Param;
      });
    return TRUE;
  }

  vec3 __GetNorm( vec3 Point ) override
  {
    for (INT i = 0; i < 6; i++)
      if (IS_ZERO((Point - P[i]) & N[i]))
        return N[i];
    return vec3();
  }

  BOOL __IsInside( vec3 Point ) override
  {
    vec3 Delta = Point - Pos;
    DBL dUp = abs(Delta & Up);
    DBL dRight = abs(Delta & Right);
    DBL dFwd = abs(Delta & Fwd);
    return dUp <= H / 2 && dRight <= W / 2 && dFwd <= D / 2;
  }
};

#endif /* __sphere_h_ */


