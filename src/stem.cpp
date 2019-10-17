#include "stem.h"

namespace ecohydrology
{
stem::stem()
{
  dStem_carbon = missing_value;
  // dStem_nitrogen = missing_value;
}

stem::~stem() {}

///
/// \param eCU_type
/// \param iVegetation_type
/// \return
int stem::initialize_stem(eColumn_type eCU_type, int iVegetation_type)
{
  int error_code = 1;

  switch (eCU_type)
    {
    case eCU_glacier:
      {
      }
      break;
    case eCU_lake:
      break;
    case eCU_land: // land
      {

        dStem_carbon = 0.0;
      }
      break;
    case eCU_stream:
      break;
    case eCU_swale:
      break;
    default:
      break;
    }

  return error_code;
}

int stem::update_stem_parameter(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {
    }
  return error_code;
}

int stem::update_stem_status(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dStem_carbon = 0.0;
    }
  else
    {
    }
  return error_code;
}
} // namespace ecohydrology
