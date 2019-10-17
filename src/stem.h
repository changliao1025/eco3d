#pragma once

#include "dimension.h"
#include "global.h"

// 50==================================================

// 50==================================================
namespace ecohydrology
{
class stem
{
public:
  stem();

  ~stem();

  double dStem_carbon;
  // double dStem_nitrogen;

  int initialize_stem(eColumn_type eCU_type, int iVegetation_type);

  int update_stem_parameter(int iVegetation_type);

  int update_stem_status(int iVegetation_type);
};
} // namespace ecohydrology
