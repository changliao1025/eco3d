#include "root.h"

namespace ecohydrology
{
root::root()
{
  dRoot_carbon = missing_value;
  dRoot_carbon_decomposed = missing_value;
  // dRoot_nitrogen = missing_value;
  // dNitrogen_uptake = missing_value;
}

root::~root() {}

///
/// \param eCU_type
/// \param iVegetation_type
/// \return
int root::initialize_root(eColumn_type eCU_type, int iVegetation_type)
{
  int error_code = 1;

  switch (eCU_type)
    {
    case eCU_glacier: // glacier
      {
      }
      break;
    case eCU_lake: // lake
      break;
    case eCU_land: // land
      {

        dRoot_carbon = 0.0;
        dRoot_carbon_decomposed = 0.0;
        // dRoot_nitrogen = 0.0;
        // dNitrogen_uptake = 0.0;
      }
      break;
    case eCU_stream: // stream
      break;
    case eCU_swale:
      break;
    default:
      break;
    }

  return error_code;
}

///
/// \param iMonth
/// \param dTemperature_in
/// \return
int root::root_decomposition(int iMonth, double dTemperature_in)
{

  int error_code = 1;
  double dTemperature_celsius = dTemperature_in + kelvin_2_celsius;

  if (dTemperature_celsius < near_zero)
    {
      dRoot_carbon_decomposed = dRoot_carbon * 0.01;
    }
  else
    {
      dRoot_carbon_decomposed = dRoot_carbon * 0.05;
    }
  return error_code;
}

int root::run_root_model(int iVegetation_type, int iMonth,
                         double dTemperature_in)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dRoot_carbon_decomposed = dRoot_carbon;
    }
  else
    {
      error_code = root_decomposition(iMonth, dTemperature_in);
    }
  return error_code;
}

int root::update_root_parameter(int iVegetation_type)
{
  int error_code = 1;

  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {

      //  dNmax = aNmax[iVegetation_type - 1];
      //  dmaxcut = nmaxcut[iVegetation_type - 1];
      //  dmax1a = nmax1a[iVegetation_type - 1];
      //  dmax1b = nmax1b[iVegetation_type - 1];
      //  dmax2a = nmax2a[iVegetation_type - 1];
      //  dmax2b = nmax2b[iVegetation_type - 1];
      //  dKn1 = aKn1[iVegetation_type - 1];
    }

  return error_code;
}

int root::update_root_status(int iVegetation_type)
{
  int error_code = 1;

  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {

      dRoot_carbon = 0.0;
    }
  else
    {
      dRoot_carbon = dRoot_carbon - dRoot_carbon_decomposed;
      if (dRoot_carbon < tiny_value)
        {
          dRoot_carbon = 0.0;
        }
    }
  return error_code;
}

// 50==================================================
// int root::root_run(double dNitrogen_available_in,
//	double dQ10_in,
//	double dScalar_soil_moisture_nitrogen_uptake_in)
//{
//	int error_code = 1;
//	//calculate_nitrogen_uptake(dNitrogen_available_in,
//	//	dQ10_in,
//	//	dScalar_soil_moisture_nitrogen_uptake_in);
//	return error_code;
//}

// int root::calculate_nitrogen_uptake(double dNitrogen_available_in,
//	double dQ10_in,
//	double dScalar_soil_moisture_nitrogen_uptake_in)
//{
//	int error_code = 1;
//	double dummy0 = dNitrogen_available_in *
//dScalar_soil_moisture_nitrogen_uptake_in; 	double dummy1 = dummy0 / (dummy0 +
//dKn1); 	dNitrogen_uptake = dNmax * dummy1 * dQ10_in; 	if (dNitrogen_uptake <
//near_zero)
//	{
//		dNitrogen_uptake = 0.0;
//	}
//	return error_code;
//}

} // namespace ecohydrology
