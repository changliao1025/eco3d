#include "mathematics.h"
// 50==================================================
// calcualte the mean value of an array
// 50==================================================
double calculate_mean(std::vector<double> vData_in)
{
  double dMean = 0.0;
  double dTotal = 0.0;
  double dDummy = 0.0;

  long lCount = 0;

  std::vector<double>::iterator iIterator;

  for (iIterator = vData_in.begin(); iIterator != vData_in.end(); iIterator++)
    {
      dDummy = *iIterator;

      dTotal = dTotal + dDummy;
      lCount = lCount + 1;
    }

  // get mean
  if (lCount > 0)
    {
      dMean = dTotal / lCount;
    }
  else
    {
      dMean = missing_value;
    }
  return dMean;
}

// 50==================================================
// calcualte the mean value of an array, excluding the missing value
// 50==================================================
double calculate_mean(std::vector<double> vData_in, double missing_value)
{
  double dMean = 0.0;
  double dTotal = 0.0;
  double dDummy = 0.0;
  long lCount = 0;

  std::vector<double>::iterator iIterator;

  for (iIterator = vData_in.begin(); iIterator != vData_in.end(); iIterator++)
    {
      dDummy = *iIterator;
      if (dDummy != missing_value)
        {
          dTotal = dTotal + dDummy;
          lCount = lCount + 1;
        }
    }
  // get mean
  if (lCount > 0)
    {
      dMean = dTotal / lCount;
    }
  else
    {
      dMean = missing_value;
    }
  return dMean;
}
// 50==================================================
// calcualte the mean value of an array in range min and max
// 50==================================================
double calculate_mean(std::vector<double> vData_in, double max_value,
                      double min_value)
{
  double dMean = 0.0;
  double dTotal = 0.0;
  double dDummy;
  long lCount = 0;

  std::vector<double>::iterator iIterator;
  for (iIterator = vData_in.begin(); iIterator != vData_in.end(); iIterator++)
    {
      dDummy = *iIterator;
      if (dDummy >= min_value && dDummy <= max_value)
        {
          dTotal = dTotal + dDummy;
          lCount = lCount + 1;
        }
    }

  // get mean
  if (lCount > 0)
    {
      dMean = dTotal / lCount;
    }
  else
    {
      dMean = missing_value;
    }
  return dMean;
}

void tridia(int n, std::vector<double> &a, std::vector<double> &b,
            std::vector<double> &c, std::vector<double> &r,
            std::vector<double> &u)
// ------------------------ notes ----------------------------------
// solve for u given the set of equations f * u = r, where u is a
// vector of length n, r is a vector of length n, and f is an n x n
// tridiagonal matrix defined by the vectors a, b, c [each of length n].
// a(1) and c(n) are undefined and are not referenced by the subroutine.

//    |b(1) c(1)   0  ...                     |   |u(1  )|   |r(1  )|
//    |a(2) b(2) c(2) ...                     |   |u(2  )|   |r(2  )|
//    |               ...                     | * | ...  | = | ...  |
//    |               ... a(n-1) b(n-1) c(n-1)|   |u(n-1)|   |r(n-1)|
//    |               ...   0    a(n  ) b(n  )|   |u(n  )|   |r(n  )|

{
  std::vector<double> gam(n);
  double bet;
  int j;

  bet = b[0];
  u[0] = r[0] / bet;

  for (j = 1; j < n; j++)
    {
      gam[j] = c[j - 1] / bet;
      bet = b[j] - a[j] * gam[j];
      u[j] = (r[j] - a[j] * u[j - 1]) / bet;
    }

  for (j = n - 2; j >= 0; j--)
    {
      u[j] = u[j] - gam[j + 1] * u[j + 1];
    }

  return;
}
