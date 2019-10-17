// 50==================================================
/*!
  \file evapotranspiration.cpp
  \brief

  <long description>

  \author Chang Liao
  \date 2017-01-22
*/
// 50==================================================
#include "evapotranspiration.h"

namespace ecohydrology
{
evapotranspiration::evapotranspiration()
{
  dPet = missing_value;

  dPet_hru = missing_value;
}

evapotranspiration::~evapotranspiration() {}

int evapotranspiration::initialize_evapotranspiration()
{
  int error_code = 1;
  dPet = 0.0;

  dPet_hru = 0.0;
  return error_code;
}

int evapotranspiration::update_evapotranspiration_parameter()
{
  int error_code = 1;
  return error_code;
}
// 50==================================================
/*!
  <calculate the latent heat of evaporation>

  \param dTemperature_mean_in the mean temperature, unit: kelvin
  \return <dLambda, unit: joule per gram>
*/
// 50==================================================

int evapotranspiration::calculate_vaporization_latent_heat(
    double dTemperature_mean_in)
{
  int error_code = 1;
  // convert temperature from K to F
  double dTemperature_fahrenheit =
      convert_from_kelvin_to_fahrenheit(dTemperature_mean_in);
  double dLambda_calorie =
      (597.3 - (0.5653 * dTemperature_fahrenheit)); // units: calories/g
  // convert the unit to SI unit
  dLambda = dLambda_calorie * calorie_2_joule;
  if (dLambda < small_value)
    {
      dLambda = 0.0;
    }
  return error_code;
}

int evapotranspiration::calculate_jh_coef(double dElevation_in,
                                          double dVapor_pressure_deficit_in)
{
  int error_code = 1;

  dCoefficient_jh_fahrenheit =
      27.5 - (dElevation_in / 1000) - 0.25 * dVapor_pressure_deficit_in * 0.01;

  dCoefficient_jh =
      convert_from_fahrenheit_to_kelvin(dCoefficient_jh_fahrenheit);
  return error_code;
}

// 50==================================================
/*!
  <ET using jh methodn>
  \param iMonth month od the year
  \param dShortwave_radiation_in units: joulie per square meter per day
  \param dTemperature_mean_in mean temperature unit kelvin
  \return <ReturnValue>
*/
// 50==================================================

int evapotranspiration::calculate_potential_evapotranspiration_jh(
    int iMonth, double dElevation_in, double dShortwave_radiation_in,
    double dTemperature_mean_in, double dVapor_pressure_deficit_in)
{
  int error_code = 1;
  double dummy0, dummy1;
  double lambda_calorie;
  calculate_jh_coef(dElevation_in, dVapor_pressure_deficit_in);

  double dTemperature_mean_fahrenheit =
      convert_from_kelvin_to_fahrenheit(dTemperature_mean_in);

  if (dTemperature_mean_fahrenheit < dCoefficient_jh_fahrenheit)
    {
      dPet = 0.0;
    }
  else
    {
      // conver SI unit to unit system in PRMS in langley
      dummy0 = dShortwave_radiation_in * joule_2_langley;
      // latent heat of vaporization
      // snow and glacier hydrolgy page 62
      lambda_calorie = dLambda * joule_2_calorie;
      // convert unit from carolies to julies
      dummy1 = aCoefficient_jh[iMonth - 1] *
               (dTemperature_mean_fahrenheit - dCoefficient_jh_fahrenheit) *
               dummy0 / (lambda_calorie * 2.54);
      dPet = dummy1 * inch_2_meter;
    }
  if (dPet < tiny_value) // quality control
    {
      dPet = 0.0;
    }
  dPet_hru = dPet;
  return error_code;
}

// 50==================================================
/*!
  <ET using Priestley-tayor equation>
  \param iMonth month of the year
  \param dElevation_in elevation of the grid, unit meter
  \param dShortwave_radiation_in the daily actual shortwave radiation,
  units: joule per square meter per day
  \param dTemperature_mean_in mean temperature, unit kelvin
  \param dTemperature_mean_yesterday_in mean temperature from yesterday, unit
  kelvin \return <ReturnValue>
*/
// 50==================================================

int evapotranspiration::calculate_potential_evapotranspiration_pt(
    int iMonth, double dElevation_in, double dShortwave_radiation_in,
    double dTemperature_mean_in, double dTemperature_mean_yesterday_in)
{
  int error_code = 1;
  double dAlpha = aAlpha[iMonth - 1];
  double dDEM_feet = dElevation_in * meter_2_foot;
  double dLatent_heat_water_calories =
      (597.3 - (0.5653 * dTemperature_mean_in)); // units: carolies/gm
  double dPsycnst =
      1.6286 * (101.3 - 0.003215 * dDEM_feet) / dLatent_heat_water_calories;
  double dummy0 = 17.26939 * (dTemperature_mean_in + kelvin_2_celsius) /
                  (dTemperature_mean_in);
  double dVp_slope =
      4098.0 * (0.6108 * exp(dummy0)) / (pow(dTemperature_mean_in, 2));
  double dHeat_flux_ground =
      -4.2 * (dTemperature_mean_yesterday_in - dTemperature_mean_in);
  dPet = dAlpha / dLatent_heat_water_calories *
         (dVp_slope / (dVp_slope + dPsycnst)) *
         (dShortwave_radiation_in / 23.88 - dHeat_flux_ground) / 25.4;
  dPet_hru = dPet;
  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iMonth month of the year
  \param dShortwave_in the daily actual shortwave radiation, units: joule per
  square meter per day \param dTemperature_mean_in mean temperature unti kelvin
  \return <ReturnValue>
*/
// 50==================================================

int evapotranspiration::run_evapotranspiration_model(
    int iMonth, double dElevation_in, double dShortwave_in,
    double dTemperature_mean_in, double dVapor_pressure_deficit_in)
{
  int error_code = 1;

  calculate_vaporization_latent_heat(dTemperature_mean_in);
  calculate_potential_evapotranspiration_jh(iMonth, dElevation_in,
                                            dShortwave_in, dTemperature_mean_in,
                                            dVapor_pressure_deficit_in);
  return error_code;
}
} // namespace ecohydrology