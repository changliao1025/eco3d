// 50==================================================
/*!
\file   evapotranspiration.h
\brief

The class for evapotranspiration component.

\author Chang Liao
\date   2016-09-30
*/
// 50==================================================
#pragma once
#include <array>
// 50==================================================
#include "conversion.h"
#include "dimension.h"
#include "global.h"
#include "mathematics.h"
#include "system.h"
using namespace std;

// 50==================================================

const std::array<double, nmonth> aCoefficient_jh = {{0.014, 0.014, 0.014, 0.02,
                                                     0.03, 0.04, 0.05, 0.04,
                                                     0.03, 0.02, 0.014, 0.014}};
const std::array<double, nmonth> aAlpha = {
    {1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26}};

// 50==================================================
namespace ecohydrology
{
class evapotranspiration
{
public:
  evapotranspiration();
  ~evapotranspiration();
  // kelvin
  double dCoefficient_jh;
  double dCoefficient_jh_fahrenheit;
  // the total potential et for each hru, unit, meter
  double dPet;
  // double dPet_cm;
  double dPet_hru;

  double dLambda;

  int calculate_vaporization_latent_heat(double dTemperature_mean_in);

  int calculate_jh_coef(double dElevation_in,
                        double dVapor_pressure_deficit_in);
  // several algrithm will be supported later
  int calculate_potential_evapotranspiration_jh(
      int iMonth, double dElevation_in, double dShortwave_in,
      double dTemperature_mean_in, double dVapor_pressure_deficit_in);
  int calculate_potential_evapotranspiration_pt(
      int iMonth, double dElevation_in, double dShortwave_radiation_in,
      double dTemperature_mean_in, double dTemperature_mean_yesterday_in);
  int initialize_evapotranspiration();
  int run_evapotranspiration_model(int iMonth, double dElevation_in,
                                   double dShortwave_radiation_actual_in,
                                   double dTemperature_mean_in,
                                   double dVapor_pressure_deficit_in);

  int update_evapotranspiration_parameter();
};
} // namespace ecohydrology