// 50==================================================
/*!
  \file   vegetation.h
  \brief

  <long description>

  \author Chang Liao
  \date   2017-01-18
*/
// 50==================================================
#pragma once

#include "canopy.h"
#include "dimension.h"
#include "global.h"
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>

#include "respiration.h"
#include "root.h"
#include "stem.h"

using namespace std;

// 50==================================================
const std::array<double, nvegetation_type> cnsoil = {
    {16.07, 16.07, 16.07, 16.07, 16.07, 16.07, 16.07, 16.07, 16.07, 16.07,
     16.07, 16.07}};
const std::array<double, nvegetation_type>
    aRatio_carbon_2_nitrogen_equilibrium = {{32.81, 32.81, 32.81, 32.81, 32.81,
                                             32.81, 32.81, 32.81, 32.81, 32.81,
                                             32.81, 32.81}};
const std::array<double, nvegetation_type> cnmin = {
    {32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81,
     32.81, 32.81}};
const std::array<double, nvegetation_type> labncon = {
    {32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81, 32.81,
     32.81, 32.81}};
// 50==================================================
namespace ecohydrology
{
class vegetation
{
public:
  vegetation();

  ~vegetation();

  int iVegetation_type;
  int iVegetation_type_default;
  int iFlag_vegetation_previous;
  double dVegetation_carbon;
  double dVegetation_gpp;
  double dVegetation_npp;
  canopy cCanopy;
  respiration cRespiration;
  root cRoot;
  stem cStem;

  static int convert_land_cover_to_vegetation_type(int iLand_cover_type);

  int calculate_vegetation_npp(double dGPP, double dRespiration_autotrophic);

  int initialize_vegetation(eColumn_type eCU_type);

  int run_vegetation_model(int iMonth, int iPrecipitation_type, double dArea,
                           double dLand_pet, double dLai_in, double dRain_in,
                           double dSnow_in, double dFpar_in, double dPar_in,
                           double dTemperature_mean_in,
                           double dVapor_pressure_deficit_in);

  int update_vegetation_status();

  int update_vegetation_parameter(int iVegetation_type);

  // 50==================================================
  // unused code
  // 50==================================================
  /// double dRespiration_autotrophic;
  //  double dRespitation_maintenance;
  // double dRespitation_growth;
  // double dNitrogen_mobilization;
  // double dNitrogen_structural;
  // double dNitrogen_uptake_structural;
  // double dNitrogen_liable;
  // double dNitrogen_uptake_liable;
  // double dNitrogen_uptake;
  // double dVegetation_nitrogen;
  // double dVegetation_structural_nitrogen; // structural nitrogen
  // double dVegetation_liable_nitrogen;     // liable nitrogen
  // double dRatio_carbon_2_nitrogen; // the ratio between carbon and nitrogen
  // double dNitrogen_resorption;
  // int reset_vegetation_type(int iVegetation_type);
};
} // namespace ecohydrology
