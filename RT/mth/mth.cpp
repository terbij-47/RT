#include "mth.h"

#if 0
// шл€па
std::vector<mth::complex<DBL>> mth::Equation3Pow( DBL a, DBL b, DBL c, DBL d )
{
  std::vector<complex<DBL>> Res;

  DBL p = (3 * a * c - b*b) / (3 * a*a);
  DBL q = (2*b*b*b - 9*a*b*c + 27*a*a*d) / (27*a*a*a);
  DBL tmp1 = p / 3;
  DBL tmp2 = q / 2;
  DBL Q = tmp1 * tmp1 * tmp1 + tmp2 * tmp2;
  if (Q > 0)
  {
    DBL alpha = pow(-q / 2 + sqrt(Q) , 1 / 3);
    DBL beta = pow(-q / 2 - sqrt(Q) , 1 / 3);
    complex<DBL> c1(alpha + beta);
    complex<DBL> c2(alpha - beta);

    Res.push_back(c1);
    Res.push_back(c1 * (-0.5) + c2 * complex<DBL>(0, sqrt(3) / 2));
    Res.push_back(c1 * (-0.5) - c2 * complex<DBL>(0, sqrt(3) / 2));
  }
  else if (Q == 0)
  {
    DBL alpha = pow(-q / 2, 1 / 3);
    Res.push_back(complex<DBL>(2 * alpha));
    Res.push_back(complex<DBL>(-alpha));
  }
  else
  {
    std::vector<complex<DBL>> alpha = (complex<DBL>(0, 1) * sqrt(-Q) - q / 2).root(3);
    std::vector<complex<DBL>> beta = (complex<DBL>(0, 1) * (-sqrt(-Q)) - q / 2).root(3);

    complex<DBL> check(-p/3);
    BOOL fl = FALSE;
    for (auto a : alpha)
    {
      for (auto be : beta)
      {
        complex<DBL> v = a * be;
        if (v == check)
        {
          Res.push_back(a + be);
          Res.push_back((a + be) * (-0.5) + (a - be) * complex<DBL>(0, sqrt(3) / 2));
          Res.push_back((a + be) * (-0.5) - (a - be) * complex<DBL>(0, sqrt(3) / 2));
          fl = TRUE;
          break;
        }
      }
      if (fl)
        break;
    }
  }
  for (auto el : Res)
    el -= complex<DBL>(b / (3 * a));
  return Res;
}


std::vector<mth::complex<DBL>> mth::Equation4Pow( DBL a, DBL b, DBL c, DBL d, DBL e )
{
  std::vector<complex<DBL>> Res;
  return Res;
}

#endif 