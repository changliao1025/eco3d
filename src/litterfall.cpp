#include "litterfall.h"

namespace ecohydrology
{
litterfall::litterfall()
{

  dLitterfall_carbon = missing_value;
  // dLitterfall_nitrogen = missing_value;
}

litterfall::~litterfall() {}
///
/// \param eCU_type
/// \param iVegetation_type
/// \return

int litterfall::initialize_litterfall(eColumn_type eCU_type,
                                      int iVegetation_type)
{
  int error_code = 1;
  // this->iVegetation_type = iVegetation_type;
  switch (eCU_type)
    {
    case eCU_glacier:
      {
      }
      break;
    case eCU_lake:
      break;
    case eCU_land:
      {
        dLitterfall_carbon = 0.0;
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
// 50==================================================
/*!
  calculale the carbon content from litterfall

  \param dCarbon, the carbon content in canopy, units: kg per square meter
  \return <dCarbon_litterfall, the litterfall carbon, unit: g per square meter>
*/
// 50==================================================

int litterfall::calculate_litterfall_carbon(double dCarbon_canopy_in)
{
  int error_code = 1;
  dLitterfall_carbon = dCoefficient_litterfall_carbon * dCarbon_canopy_in;
  if (dLitterfall_carbon < near_zero)
    {
      dLitterfall_carbon = 0.0;
    }

  return error_code;
}

// 50==================================================
/*!
  Run the canopy model

  \param dCarbon_canopy_in canopy carbon pool storage  unit, gC m(-2)
  \return <ReturnValue>
*/
// 50==================================================
int litterfall::run_litterfall_model(int iVegetation_type,
                                     double dCarbon_canopy_in)
{
  int error_code;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      // all canopy turn litterfall
      dLitterfall_carbon = dCarbon_canopy_in;
    }
  else
    {
      error_code = calculate_litterfall_carbon(dCarbon_canopy_in);
      // error_code = calculate_litterfall_nitrogen(dNitrogen_canopy_in);
    }
  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
int litterfall::update_litterfall_parameter(int iVegetation_type)
{
  int error_code = 1;

  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {
      dCoefficient_litterfall_carbon =
          aCoefficient_litterfall_carbon[iVegetation_type - 1];
      // dCoefficient_litterfall_nitrogen =
      //	aCoefficient_litterfall_nitrogen[iVegetation_type - 1];
    }

  return error_code;
}

// 50==================================================
/*!
  calcuatele the nitrogen content from litterfall

  \param dNitrogen_structural_in
  \return <ReturnValue>
*/
// 50==================================================

// int litterfall::calculate_litterfall_nitrogen(double dNitrogen_structural_in)
//{
//	int error_code = 1;
//	dLitterfall_nitrogen = dCoefficient_litterfall_nitrogen *
//dNitrogen_structural_in; // unit 	if (dLitterfall_nitrogen < near_zero)
//	{
//		dLitterfall_nitrogen = 0.0;
//	}
//	return error_code;
//}
} // namespace ecohydrology
