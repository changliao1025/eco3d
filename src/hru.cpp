#include "hru.h"
namespace ecohydrology
{
hru::hru()
{
  // land
  iHru_type_default = 3;
}
hru::~hru() {}

int hru::initialize_hru(eColumn_type eCU_type)
{
  int error_code = 1;
  switch (eCU_type)
    {
    case eCU_glacier:
      dHru_surface_depression_percentage = 0.0;
      dHru_impervious_surface_percentage = 0.0;
      dHru_pervious_surface_percentage = 1.0;
      break;
    case eCU_lake:
      dHru_surface_depression_percentage = 0.0;
      dHru_impervious_surface_percentage = 0.0;

      dHru_pervious_surface_percentage = 1.0;
      break;
    case eCU_land:
      dHru_surface_depression_percentage = 0.0;
      dHru_impervious_surface_percentage = 0.0;
      dHru_pervious_surface_percentage = 1.0;
      break;
    case eCU_stream:
      dHru_surface_depression_percentage = 0.0;
      dHru_impervious_surface_percentage = 0.0;
      dHru_pervious_surface_percentage = 1.0;
      break;
    case eCU_swale:
      dHru_surface_depression_percentage = 0.0;
      dHru_impervious_surface_percentage = 0.0;
      dHru_pervious_surface_percentage = 1.0;
      break;
    default:
      dHru_surface_depression_percentage = 0.0;
      dHru_impervious_surface_percentage = 0.0;
      dHru_pervious_surface_percentage = 1.0;
      break;
    }
  return error_code;
}
int hru::update_hru_parameter(double dImpervious_surface_percentage_in,
                              double dPervious_surface_percentage_in,
                              double dSurface_depression_percentage_in)
{
  int error_code = 1;
  this->dHru_surface_depression_percentage = dSurface_depression_percentage_in,
  this->dHru_impervious_surface_percentage = dImpervious_surface_percentage_in,
  this->dHru_pervious_surface_percentage = dPervious_surface_percentage_in;
  return error_code;
}
} // namespace ecohydrology
