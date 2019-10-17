#pragma once

#include "dimension.h"

namespace ecohydrology
{
class hru
{
public:
  hru();
  ~hru();
  // 50==================================================
  // physical proprieties
  int iHru_type; // the hru type will not be changed, but other properties could
  int iHru_type_default;
  double dHru_impervious_surface_percentage;

  double dHru_pervious_surface_percentage;
  double dHru_surface_depression_percentage;
  // 50==================================================
  // funtion
  // 50==================================================
  int initialize_hru(eColumn_type eCU_type);
  int update_hru_parameter(double dSurface_depression_percentage_in,
                           double dImpervious_surface_percentage_in,
                           double dPervious_surface_percentage_in);
};
} // namespace ecohydrology
