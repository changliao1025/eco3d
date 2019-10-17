// 50==================================================
/*!
  \file photosynthesis.h
  \brief

  <long description>

  \author Chang Liao
  \date 2017-01-20
*/
// 50==================================================
#pragma once

#include <array>
#include <iostream>
// 50==================================================
#include "dimension.h"
#include "global.h"

using namespace std;

// 50==================================================
// parameter for vegetation type, these parameters are obtained from the MODIS
// guide
// http://www.ntsg.umt.edu/sites/ntsg.umt.edu/files/modis/MOD17UsersGuide2015_v3.pdf

const std::array<double, nvegetation_type> aEpsilon_max = {{
    0.000962, 0.001268, 0.001086, 0.001165, 0.001051, 0.001281,
    0.000841, // shubland
    0.001239, // savanas
    0.001206, // savanas
    0.000860, // grass
    0.001044, // wetland
    0.001044  // cropland
}};

// parametes used in modis algorithm, unit: celsius
const std::array<double, nvegetation_type> aTmin_max = {
    {8.31, 9.09, 10.44, 9.94, 9.5, 8.61, 8.8, 11.39, 11.39, 12.02, 12.02,
     12.02}};

const std::array<double, nvegetation_type> aTmin_min = {
    {-8.0, -8.0, -8.0, -6.0, -7.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0}};

// parameter used in modis algorithm, unit: pascal
const std::array<double, nvegetation_type> aVpd_max = {
    {4600, 3100, 2300, 1650, 2400, 4700, 4800, 3200, 3100, 5300, 4300, 4300}};

const std::array<double, nvegetation_type> aVpd_min = {
    {650, 800, 650, 650, 650, 650, 650, 650, 650, 650, 650, 650}};

// 50==================================================
namespace ecohydrology
{
class photosynthesis
{
public:
  photosynthesis();

  ~photosynthesis();

  // double iVegetation_type;    // the vegetation type
  // double dFpar;               // fraction o par used unitless
  double dScalar_temperature;
  double dScalar_vpd;

  double dPhotosynthesis_gpp;
  // double dPhotosynthesis_gpp_gram; // units: g per square meter per day

  double dEpsilon_max;
  double dTmin_max; // unit: celsius
  double dTmin_min; // unit: celsius
  double dVpd_max;  // unit: pascal
  double dVpd_min;  // unit: pascal

  int calculate_daily_gpp(double dFpar_in, double dIpar_in,
                          double dTemperature_min_in,
                          double dVapor_pressure_defici_int);

  double calculate_epsilon(double dEpsilon_max_in, double dTemperature_min_in,
                           double dVapor_pressure_deficit_in);

  double calculate_temperature_scalar(double dTemperature_min_in);

  double calculate_vpd_scalar(double dVapor_pressure_deficit_in);

  int initialize_photosynthesis(eColumn_type eCU_type, int iVegetation_type);

  int run_photosynthesis_model(int iVegetation_type, double dFpar_in,
                               double dPar_in, double dTemperature_min_in,
                               double dVapor_pressure_deficit_in);

  int update_photosynthesis_parameter(int iVegetation_type);
};
} // namespace ecohydrology
