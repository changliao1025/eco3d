// 50==================================================
/*!
  \file   temperature.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-01-20
*/
// 50==================================================
#include "temperature.h"

namespace ecohydrology
{
// 50==================================================
/*!
  <long-description>

  \param void
*/
// 50==================================================
temperature::temperature()
{
  // set up the default values
  iFlag_transpiration = 0;
  dTemperature_cumulative = 0.0;
  dTemperature_max = missing_value;
  dTemperature_mean = missing_value;
  dTemperature_min = missing_value;
  dTemperature_dewpoint = missing_value;
}

temperature::~temperature() {}

// 50==================================================
/*!
  <determine whether the tranpiration starts or not
  temperature of the recent 15 days are used
  >

  \param dTemperature_mean_in  mean temperature, unit: kelvin
  \return <ReturnValue> //return 0(not start yet) or 1(start)
*/
// 50==================================================

int temperature::calculate_transpiration_status(double dTemperature_mean_in)
{
  int error_code = 1;
  // get current size
  auto iDay_count = this->vTemperature_mean.size();
  // unit conversion
  double dTemperature_mean_celsius = dTemperature_mean_in + kelvin_2_celsius;
  // only continue 15 day temperature accumulation exceeds threshold is
  // considered as start of transpiration
  if (iDay_count < 15)
    {
      this->vTemperature_mean.push_back(dTemperature_mean_celsius);
      dTemperature_cumulative = std::accumulate(vTemperature_mean.begin(),
                                                vTemperature_mean.end(), 0.0);
    }
  else
    {
      // remove the first one
      this->vTemperature_mean.erase(vTemperature_mean.begin());
      // add the last one
      this->vTemperature_mean.push_back(dTemperature_mean_celsius);
      dTemperature_cumulative = std::accumulate(vTemperature_mean.begin(),
                                                vTemperature_mean.end(), 0.0);
    }
  if (dTemperature_cumulative >= dTemperature_threshold)
    {
      // both of them are in unit celsius
      iFlag_transpiration = 1;
    }
  else
    {
      iFlag_transpiration = 0;
    }
  return error_code;
}

// 50==================================================
/*!
  <calculate the vapor pressure deficit using dewpoint temperature
  the equation used is based on temperature
  >

  \param dTemperature_dewpoint_in dTemperature_dewpoint_in: unit: kelvin
  \param dTemperature_mean_in dTemperature_mean_in: unit: kelvin
  \return <the vapor pressure deficit>
*/
// 50==================================================

int temperature::calculate_vapor_pressure_deficit(double dTemperature_mean_in)
{
  int error_code = 1;
  double dTemperature_mean_celsius;
  double dTemperature_dewpoint_celsius;
  double dDummy;
  double dSaturated_vapor_pressure;
  double dActual_vapor_pressure;
  // check whether dTemperature_dewpoint is higher than actual temperature

  if (dTemperature_dewpoint >= dTemperature_mean_in)
    {
      // add 1 degree to the dewpoint
      dTemperature_dewpoint = dTemperature_mean_in - 1.0;
    }
  else
    {
      if (dTemperature_dewpoint < (this->dTemperature_min - 4.0))
        {
          dTemperature_dewpoint = this->dTemperature_min - 4.0;
        }
    }
  dTemperature_mean_celsius =
      dTemperature_mean_in + kelvin_2_celsius; // convert unit
  dTemperature_dewpoint_celsius =
      dTemperature_dewpoint + kelvin_2_celsius; // convert unit

  // calcualte saturated pressure
  dDummy =
      (7.5 * dTemperature_mean_celsius) / (273.3 + dTemperature_mean_celsius);
  dSaturated_vapor_pressure = 6.11 * pow(10, dDummy); // unit is hpa

  // do the same for dewpoint temperature and pressure
  dDummy = (7.5 * dTemperature_dewpoint_celsius) /
           (273.3 + dTemperature_dewpoint_celsius);
  dActual_vapor_pressure = 6.11 * pow(10, dDummy);

  // get the difference
  dVapor_pressure_deficit =
      dSaturated_vapor_pressure - dActual_vapor_pressure; // the unit is hPa
  // 50==================================================
  // quality control
  // 50==================================================
  if (abs(dVapor_pressure_deficit) < near_zero)
    {
      // assume they are the same
      dVapor_pressure_deficit = 0.0;
    }
  else
    {
      if (dVapor_pressure_deficit <= -near_zero)
        {
          dVapor_pressure_deficit = 0.0;
        }
    }
  // convert unit to SI unit
  dVapor_pressure_deficit = dVapor_pressure_deficit * hpa_2_pa;
  return error_code;
}
// 50==================================================
// The head quarter
// inputs
// dTemperature_dewpoint_in: unit: kelvin
// dTemperature_max_in: unit: kelvin
// dTemperature_min_in: unit: kelvin
// 50==================================================
// 50==================================================
/*!
  <long-description>

  \param dTemperature_dewpoint_in
  \param dTemperature_max_in
  \param dTemperature_min_in
  \return <ReturnValue>
*/
// 50==================================================

int temperature::run_temperature_model(double dTemperature_dewpoint_in,
                                       double dTemperature_max_in,
                                       double dTemperature_min_in)
{
  int error_code = 1;
  double dTemperature_mean_in;
  this->dTemperature_dewpoint = dTemperature_dewpoint_in;
  this->dTemperature_max = dTemperature_max_in;
  this->dTemperature_min = dTemperature_min_in;
  dTemperature_mean_in = (dTemperature_max_in + dTemperature_min_in) * 0.5;
  this->dTemperature_mean = dTemperature_mean_in;

  error_code = calculate_transpiration_status(dTemperature_mean_in);

  if (1 == error_code)
    {
      error_code = calculate_vapor_pressure_deficit(dTemperature_mean_in);
    }

  return error_code;
}
} // namespace ecohydrology
