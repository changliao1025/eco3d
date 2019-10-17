// 50==================================================
/*!
  \file   precipitation.h
  \brief

  <long description>

  \author Chang Liao
  \date   2016-09-30
*/
// 50==================================================
#pragma once
#include "dimension.h"
#include "global.h"
#include <array>
using namespace std;

// 50==================================================
// the array for adjustment
// const std::array<double, nmonth> dAdjmix_rain = {{1.0 ,1.0,1.0, 1.0, 1.0
// ,1.0, 1.0, 1.0,1.0 ,1.0, 1.0,1.0}};
// 50==================================================
namespace ecohydrology
{
class precipitation
{
public:
  precipitation();
  ~precipitation();

  // the index to indicate the type of precipitation
  // 1: rain
  // 2: snow
  // 3: mixture
  int iPrecipitation_type;
  // when dSnow great than near zero, and new snow will form regardless whether
  // there is old snow or not int iFlag_new_snow;
  int iStorm_type;       // the storm type, depends on month in the year
  double dPrecipitation; // the total precipitation, unit: meter
  // double dPrecipitation_cm; //the total precipitation in centimeter
  // double dPrecipitation_mm; //the total precipitation in millimeter
  // the liqid form
  double dRain; // unit: meter
  // double dRain_cm;
  // double dRain_mm;
  // the soild form
  double dSnow; // unit: meter
                // double dSnow_cm;
  // double dSnow_mm;
  double dTemperature_snow;
  double dTemperature_rain;

  int run_precipitation_model(int iMonth, double dPrecipitation_in,
                              double dTemperature_max_in,
                              double dTemperature_mean_in,
                              double dTemperature_min_in);

private:
  int calculate_rain_or_snow(double dPrecipitation_in,
                             double dTemperature_max_in,
                             double dTemperature_mean_in,
                             double dTemperature_min_in);
};
} // namespace ecohydrology
