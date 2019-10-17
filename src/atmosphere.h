// 50==================================================
/*!
  \file   atmosphere.h
  \brief

  The class for atmospheric component

  \author Chang Liao
  \date   2016-09-30
*/
// 50==================================================
#pragma once
// 50==================================================
#include "precipitation.h"
#include "radiation.h"
#include "temperature.h"
using namespace std;

// 50==================================================
namespace ecohydrology
{
class atmosphere
{
public:
  atmosphere();
  ~atmosphere();

  precipitation cPrecipitation;
  radiation cRadiation;
  temperature cTemperature;
  // 50==================================================
  // function
  // 50==================================================
  int run_atmosphere_model(
      int iDay_doy, int iMonth_ymd,
      double dAspect_in,   // unit: degree
      double dLatitude_in, // unit: degree
      double
          dPrecipitation_in, // the daily precipitation, unit: centermeter, cm
      double dSlope_in,      // the slope the surface elevation, unit: degree
      double dTemperature_basin_max_in, // the maxmium the precipitation across
                                        // the basin
      double dTemperature_dewpoint_in, // the dewpoint temperature, unit: degree
      double dTemperature_max_in,      // the maximum temperature. unit: celsius
      double dTemperature_min_in       // the minimum temperature, unit; celsius
  );
};
} // namespace ecohydrology
