#include "eco3d.h"
namespace ecohydrology
{
// 50==================================================
/*!
      Locate the column index based on the 2D matrix location
      \param i
      \param j
      \return <the actual column index>
*/
// 50==================================================
long eco3d::calculate_index_from_coordinates(long lRow_in, long lColumn_in)
{
  // 50==================================================
  // vColumn_index has stored the indices of all active column starting from 0.
  // remember that even though vColumn index is a matrix, its value is the index
  // of the active hru 50==================================================
  long lIndex_out;

  if (lRow_in < 0 || lRow_in >= nrow || lColumn_in < 0 || lColumn_in >= ncolumn)
    {
      lIndex_out = -1;
    }
  else
    {
      lIndex_out = long(vColumn_index[lRow_in * ncolumn + lColumn_in]);
      if (lIndex_out < 0)
        {
          lIndex_out = -1;
        }
    }
  return lIndex_out;
}
} // namespace ecohydrology