// 50==================================================
/*!
  \file   temperature.h
  \brief

  <long description>

  \author Chang Liao
  \date   2016-09-30
*/
// 50==================================================
#pragma once

#include "global.h"
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

// 50==================================================
// the threhold to determine whether the transpiration starts or not, unit:
// Celsius
const double dTemperature_threshold = 100.0;
// 50==================================================
namespace ecohydrology
{
class temperature
{
public:
  temperature();
  ~temperature();
  int iFlag_transpiration; // the flag whether transpiration is on(1) or off(0)
  double dTemperature_cumulative; // the cumulative temperature for 15 days,
                                  // used for transpiration, unit: celsius
  double dTemperature_max;
  double dTemperature_mean;
  double dTemperature_min;

  double dTemperature_dewpoint;
  double dVapor_pressure_deficit; // unit: Pascal
  std::vector<double> vTemperature_mean;
  int calculate_transpiration_status(double dTemperature_mean_in);
  int calculate_vapor_pressure_deficit(double dTemperature_mean_in);

  int run_temperature_model(double dTemperature_dewpoint_in,
                            double dTemperature_max_in,
                            double dTemperature_min_in);
};
} // namespace ecohydrology