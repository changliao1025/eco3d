// 50==================================================
/*!
  \file   precipitation.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-01-20
*/
// 50==================================================
#include "precipitation.h"

namespace ecohydrology
{
// 50==================================================
/*!
  <long-description>

  \param void
*/
// 50==================================================
precipitation::precipitation()
{
  iPrecipitation_type = 4;
  // iFlag_new_snow = 0;
  iStorm_type = 0;
  dPrecipitation = 0.0;
  dRain = 0.0;
  dSnow = 0.0;
}

precipitation::~precipitation() {}

// 50==================================================
/*!
  <determine the type of precipitation
  iPrecipitation_type = 0: no precipitation
  1: all rain
  2: all snow
  3: mixture
  >

  \param dPrecipitation_in   the total precipitation including rain and
  snow,unit, meter \param dTemperature_max_in the maximum temperature, unit,
  kelvin \param dTemperature_min_in       the minimum temperature, unit, kelvin
  \return <ReturnValue>
*/
// 50==================================================

int precipitation::calculate_rain_or_snow(double dPrecipitation_in,
                                          double dTemperature_max_in,
                                          double dTemperature_mean_in,
                                          double dTemperature_min_in)
{
  int error_code = 1;
  double dDifference1, dDifference2;
  double dFraction;
  // the flag for precipitation type
  if (dPrecipitation_in <
      tiny_value) // if there is no precipitation, everything is 0.0
    {
      dRain = 0.0;
      dSnow = 0.0;
      // iFlag_new_snow = 0;
      iPrecipitation_type = 4;
    }
  else
    {
      // if the minimum temperature is higher than all snow temperature, then
      // all precipitation is rain see
      // https://plus.google.com/+changliao1025/posts/9wpbnQLvzyS case 2 and 3
      // and 4 , all precipitation is rain
      if (dTemperature_min_in >= dTemperature_all_snow) // case 2 and 3 and 4
        {
          dRain = dPrecipitation_in;
          dSnow = 0.0;
          // the surface snow status remains, however, it may still have snow on
          // surface. iFlag_new_snow = 0;
          iPrecipitation_type = 1;
        }
      else
        {
          // if the highest temperature is lower than all snow temperature, then
          // all precipitation is snow
          if (dTemperature_max_in <= dTemperature_all_snow) // case 1
            {
              dRain = 0.0;
              dSnow = dPrecipitation_in;
              // change the surface snow status
              // iFlag_new_snow = 1;
              iPrecipitation_type = 2;
            }
          // it maybe  a mixture event
          else
            {
              // case 5 and 6
              dDifference1 = dTemperature_max_in - dTemperature_min_in;
              dDifference2 = dTemperature_max_in - dTemperature_all_snow;
              dFraction = dDifference2 / dDifference1;
              if (dFraction <
                  small_value) // dTemperature_max_in is very close to snow
                {
                  dRain = 0.0;
                  dSnow = dPrecipitation_in;
                  // iFlag_new_snow = 1;
                  iPrecipitation_type = 2; // still all snow
                }
              else // dTemperature_max_in is away from snow
                {
                  dRain = dPrecipitation_in * dFraction;
                  if (dRain < tiny_value)
                    {
                      dRain = 0.0;
                      dSnow = dPrecipitation_in;
                      // iFlag_new_snow = 1;
                      iPrecipitation_type = 2; // still all snow
                    }
                  else
                    {
                      dSnow = dPrecipitation_in - dRain;
                      if (dSnow < tiny_value)
                        {
                          dRain = dPrecipitation_in;
                          dSnow = 0.0;
                          // iFlag_new_snow = 0;
                          iPrecipitation_type = 1;
                        }
                      else
                        {
                          // iFlag_new_snow = 1;
                          iPrecipitation_type = 3; // mixture
                        }
                    }
                }
            }
        }
    }

  switch (iPrecipitation_type)
    {
    case 1:
      {
        // if there is only rain
        dTemperature_rain = dTemperature_mean_in; // celsius
        if (dTemperature_rain <= celsius_2_kelvin)
          {
            // if the temperature of rain is lower than 0.0, change it higher
            dTemperature_rain =
                0.5 * (dTemperature_mean_in + dTemperature_all_rain);
            if (dTemperature_rain < celsius_2_kelvin)
              {
                dTemperature_rain = celsius_2_kelvin + 0.01;
              }
          }

        dTemperature_snow = celsius_2_kelvin;
      }
      break;
    case 2:
      {

        // if there is only snow
        dTemperature_snow = dTemperature_mean_in; // mean temperature, kelvin
        if (dTemperature_snow >= celsius_2_kelvin)
          {
            dTemperature_snow =
                0.5 * (dTemperature_min_in + dTemperature_all_snow);
            if (dTemperature_snow >= celsius_2_kelvin)
              {
                dTemperature_snow = celsius_2_kelvin - 0.01;
              }
          }
        dTemperature_rain = celsius_2_kelvin;
      }
      break;
    case 3:
      {
        // if this is a mixture event
        dTemperature_rain =
            0.5 * (dTemperature_mean_in + dTemperature_all_rain); // kelvin
        if (dTemperature_rain < celsius_2_kelvin)
          {
            dTemperature_rain = celsius_2_kelvin + 0.01;
          }
        // there is snowpack already
        dTemperature_snow =
            0.5 * (dTemperature_mean_in + dTemperature_all_snow);
        if (dTemperature_snow >= celsius_2_kelvin)
          {
            dTemperature_snow =
                0.5 * (dTemperature_min_in + dTemperature_all_snow);
            if (dTemperature_snow >= celsius_2_kelvin)
              {
                dTemperature_snow = celsius_2_kelvin - 0.01;
              }
          }
      }
      break;
    default:
      // there is no rain at all
      dTemperature_rain = celsius_2_kelvin;
      dTemperature_snow = celsius_2_kelvin;
      break;
    }

  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iMonth month of the year
  \param dPrecipitation_in unit meter
  \param dTemperature_max_in unit: kelvin
  \param dTemperature_min_in unit: kelvin
  \return <ReturnValue>
*/
// 50==================================================

int precipitation::run_precipitation_model(int iMonth, double dPrecipitation_in,
                                           double dTemperature_max_in,
                                           double dTemperature_mean_in,
                                           double dTemperature_min_in)
{
  int error_code = 1;
  // determine storm type based on month
  // frontal storm = 1; convective storm = 2;
  // we assume the storms in summer are convective storm
  if (iMonth < 6 || iMonth > 8)
    {
      iStorm_type = 1;
    }
  else // only in the summer has convective storms
    {
      iStorm_type = 2;
    }
  this->dPrecipitation = dPrecipitation_in;
  error_code =
      calculate_rain_or_snow(dPrecipitation_in, dTemperature_max_in,
                             dTemperature_mean_in, dTemperature_min_in);
  return error_code;
}
} // namespace ecohydrology