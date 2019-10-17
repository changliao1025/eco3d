// 50==================================================
/*!
  \file radiation.h
  \brief

  <long description>

  \author Chang Liao
  \date 2016-09-30
*/
// 50==================================================
#pragma once

#include <array>
#include <cmath>
// 50==================================================
#include "conversion.h"
#include "dimension.h"
#include "global.h"
#include "system.h"

using namespace std;

// 50==================================================
// used global variable
const double dAdjustment_temperature_max = 0.8; // unit fraction
// parameters for 12 months
// extern const int nmonth;

const std::array<double, nmonth> dday_slope = {
    {0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4}};
const std::array<double, nmonth> dday_intcp = {
    {-40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40}};
const std::array<double, 26> solf = {
    {0.20,  0.35,  0.45, 0.51,  0.56,  0.59,  0.62,  0.64,  0.655,
     0.67,  0.682, 0.69, 0.70,  0.71,  0.715, 0.72,  0.722, 0.724,
     0.726, 0.728, 0.73, 0.734, 0.738, 0.742, 0.746, 0.75}};
// 50==================================================
namespace ecohydrology
{
class radiation
{
public:
  radiation();

  ~radiation();

  double dObliquity;
  double dSolar_declination;
  double dSun_hours;
  double dTime_sunrise;
  double dTime_sunset;
  double dShortwave_radiation_potential;
  double dShortwave_radiation_actual;
  double dPar;

  double calculate_actual_shortwave_radiation(int iMonth,
                                              double dSlope_radian_in,
                                              double dTemperature_max_in);

  double calculate_photosyntheis_active_radiation();

  double calculate_potential_shortwave_radiation(int iDay_doy,
                                                 double dAspect_in,
                                                 double dLatitude_in,
                                                 double dSlope_in);

  int run_radiation_model(int iDay_doy, int iMonth_ymd, double dAspect_in,
                          double dLatitude_in, double dSlope_in,
                          double dTemperature_max_in);

private:
  double calculate_potential_solar_radiation_on_surface(
      double dLatitude_radian_in, double dLatitude_difference_radian_in,
      double dRadiation_hourly_in, double dSolar_declination_radian_in,
      double dSunrise_time_in, double dSunset_time_in);

  double calculate_solar_declination(int iDay);

  double calculate_sun_obliquity(int iDay);

  std::array<double, 2>
  calculate_sunrise_sunset_time(double dLatitude_radian_in,
                                double dSolar_declination_radian_in);
};
} // namespace ecohydrology
