// 50==================================================
/*!
  \file   atmosphere.cpp
  \brief
  <long description>
  \author Chang Liao
  \date   2017-01-20
*/
// 50==================================================
#include "atmosphere.h"

namespace ecohydrology
{
atmosphere::atmosphere() {}

atmosphere::~atmosphere() {}
// 50==================================================
/*!
  <atmospheric components>
  \param iDay_doy day of the year
  \param iMonth_ymd month of the year
  \param dAspect_in degree
  \param dLatitude_in degree
  \param dPrecipitation_in meter
  \param dSlope_in degree
  \param dTemperature_basin_max_in kelvin
  \param dTemperature_dewpoint_in kelvin
  \param dTemperature_max_in kelvin
  \param dTemperature_min_in kelvin
  \return <ReturnValue>
*/
// 50==================================================
int atmosphere::run_atmosphere_model(int iDay_doy, int iMonth_ymd,
                                     double dAspect_in, double dLatitude_in,
                                     double dPrecipitation_in, double dSlope_in,
                                     double dTemperature_basin_max_in,
                                     double dTemperature_dewpoint_in,
                                     double dTemperature_max_in,
                                     double dTemperature_min_in)
{
  int error_code;
  double dTemperature_mean;
  if (dTemperature_max_in <= dTemperature_min_in)
    {
      dTemperature_max_in = dTemperature_min_in + 1.0;
    }
  error_code = cTemperature.run_temperature_model(
      dTemperature_dewpoint_in, dTemperature_max_in, dTemperature_min_in);
  if (1 == error_code)
    {
      dTemperature_mean = cTemperature.dTemperature_mean;
      error_code = cPrecipitation.run_precipitation_model(
          iMonth_ymd, dPrecipitation_in, dTemperature_max_in, dTemperature_mean,
          dTemperature_min_in);
      if (1 == error_code)
        {

          error_code = cRadiation.run_radiation_model(
              iDay_doy, iMonth_ymd, dAspect_in, dLatitude_in, dSlope_in,
              dTemperature_basin_max_in);
        }
    }

  return error_code;
}
} // namespace ecohydrology
