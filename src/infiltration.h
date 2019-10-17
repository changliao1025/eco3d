// 50==================================================
/*!
  \file infiltration.h
  \brief
  <long description>
  \author Chang Liao
  \date 2017-01-30
*/
// 50==================================================
#pragma once
#include <array>
#include <cmath>
#include <iostream>
// 50==================================================
#include "dimension.h"
#include "global.h"
using namespace std;
// maximum infiltration rate under snow, unit: meter
const std::array<double, nsoil_type> aSnow_infiltration_max_base = {
    {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2}};
// 50==================================================
namespace ecohydrology
{
class infiltration
{
public:
  infiltration();
  ~infiltration();
  // 50==================================================
  // the infiltration rate, units: meter * day (-1)
  double dInfiltration;
  // the hru avarage infiltration rate, units: meter * day(-1)

  double dInfiltration_hru;
  // the excessive surface runoff rate, units: meter * day(-1)
  double dSurface_runoff_excess;
  // parameters based on soil type
  double dSnow_infiltration_max_base;
  double dSnow_infiltration_max;

  // 50==================================================
  // function
  // 50==================================================
  int calculate_excess_snowmelt_infiltration(double dSoil_moisture_in,
                                             double dSoil_moisture_max_in);

  int calculate_infiltration(int iFlag_old_snow,
                             //	int iHru_type,
                             int iPrecipitation_type,
                             double dFraction_pervious_in,
                             double dRunoff_pervious_in,
                             double dSoil_moisture_in,
                             double dSoil_moisture_max_in,
                             double dWater_available_pervious_in);

  int initialize_infiltration(eColumn_type eCU_type);

  int run_infiltration_model(int iFlag_old_snow,
                             // int iHru_type,
                             int iPrecipitation_type,
                             double dPervious_surface_percentage_in,
                             double dRunoff_pervious_in,
                             double dSoil_moisture_in,
                             double dSoil_moisture_max_in,
                             double dSoil_temperature_in,
                             double dWater_available_pervious_in);
  int update_infiltration_parameter(int iSoil_type);
};
} // namespace ecohydrology
