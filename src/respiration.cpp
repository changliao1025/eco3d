// 50==================================================
/*!
  \file   respiration.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-02-03
*/
// 50==================================================
#include "respiration.h"

namespace ecohydrology
{
respiration::respiration()
{
  dGrowth_respiration = missing_value;
  dMaintenance_respiration = missing_value;
  dAutotrophic_respiration = missing_value;
}

respiration::~respiration() {}

int respiration::initialize_respiration(eColumn_type eCU_type,
                                        int iVegetation_type)
{

  int error_code = 1;
  this->iVegetation_type = iVegetation_type;
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
        if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
          {
          }
        else
          {

            dQ10a0 = aQ10a0[iVegetation_type - 1];
            dQ10a1 = aQ10a1[iVegetation_type - 1];
            dQ10a2 = aQ10a2[iVegetation_type - 1];
            dQ10a3 = aQ10a3[iVegetation_type - 1];
            dKra = aKra[iVegetation_type - 1];
            dKrb = aKrb[iVegetation_type - 1];
          }
        dGrowth_respiration = 0.0;
        dMaintenance_respiration = 0.0;
        dAutotrophic_respiration = 0.0;
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
 calculate autotrophic respiration, which include both maintenance and growth
 respiration

 \param dVegetation_carbon_in
 \param dVegetation_gpp_in
 \param dVegetation_temperature_in
 \return <ReturnValue>
*/
// 50==================================================
int respiration::calculate_plant_autotrophic_respiration(
    double dVegetation_carbon_in, double dVegetation_gpp_in,
    double dVegetation_temperature_in)
{
  int error_code = 1;

  calculate_plant_maintenance_respiration(dVegetation_carbon_in,
                                          dVegetation_temperature_in);

  calculate_plant_growth_respiration(dVegetation_gpp_in);
  dAutotrophic_respiration = dMaintenance_respiration + dGrowth_respiration;
  return error_code;
}

// 50==================================================
/*!
 <long-description>

 \param dGpp_in
 \return <ReturnValue>
*/
// 50==================================================
int respiration::calculate_plant_growth_respiration(double dGpp_in)
{
  int error_code = 1;
  double dNpp_prime = dGpp_in - dMaintenance_respiration;

  if (abs(dNpp_prime) < near_zero)
    {
      dGrowth_respiration = 0.0;
    }
  else
    {
      if (dNpp_prime <= -near_zero)
        {
          dGrowth_respiration = 0.0;
        }
      else
        {
          dGrowth_respiration = 0.1 * dNpp_prime;
        }
    }

  return error_code;
}

int respiration::calculate_plant_maintenance_respiration(
    double dCarbon_vegetation_in, double dTemperature_in)
{
  int error_code = 1;

  double dQ10 = calculate_plant_respiration_q10(dTemperature_in);

  double dKr = exp(dKra * dCarbon_vegetation_in *
                       kilogram_per_square_meter_2_gram_per_square_meter +
                   dKrb);
  dMaintenance_respiration = dKr * dCarbon_vegetation_in * dQ10;
  return error_code;
}

// 50==================================================
/*!
 effect of temperature on plant respiration

 \param dTemperature_in
 \return <ReturnValue>
*/
// 50==================================================
double respiration::calculate_plant_respiration_q10(double dTemperature_in)
{
  // double check the equation
  double dTemperature_celsius = dTemperature_in + kelvin_2_celsius;
  double dummy1 = (dQ10a1 * dTemperature_celsius);
  double dummy2 = (dQ10a2 * pow(dTemperature_celsius, 2.0));
  double dummy3 = (dQ10a3 * pow(dTemperature_celsius, 3.0));
  double dQ10_out = dQ10a0 + dummy1 + dummy2 + dummy3;
  dQ10_out = pow(dQ10_out, dTemperature_celsius / 10.0);
  if (dQ10_out < 0.0)
    {
      dQ10_out = 2.0;
    }
  if (dQ10_out > 2.0)
    {
      dQ10_out = 2.0;
    }
  return dQ10_out;
}

// 50==================================================
int respiration::update_respiration_parameter(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {

      dQ10a0 = aQ10a0[iVegetation_type - 1];
      dQ10a1 = aQ10a1[iVegetation_type - 1];
      dQ10a2 = aQ10a2[iVegetation_type - 1];
      dQ10a3 = aQ10a3[iVegetation_type - 1];
      dKra = aKra[iVegetation_type - 1];
      dKrb = aKrb[iVegetation_type - 1];
    }
  return error_code;
}

// the head quarter
int respiration::run_respiration_model(int iVegetation_type,
                                       double dVegetation_carbon_in,
                                       double dVegetation_gpp_in,
                                       double dVegetation_temperature_in)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dGrowth_respiration = 0.0;
      dMaintenance_respiration = 0.0;
      dAutotrophic_respiration = 0.0;
    }
  else
    {
      calculate_plant_autotrophic_respiration(dVegetation_carbon_in,
                                              dVegetation_gpp_in,
                                              dVegetation_temperature_in);
    }
  return error_code;
}
} // namespace ecohydrology