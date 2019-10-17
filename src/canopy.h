// 50==================================================
/*!
  \file   canopy.h
  \brief

  The class for vegetation canopy component.

  \author Chang Liao
  \date   2016-09-30
*/
// 50==================================================
#pragma once
#include "interception.h"
#include "litterfall.h"
#include "photosynthesis.h"
#include <array>
#include <iostream>
using namespace std;

// 50==================================================

const std::array<double, nvegetation_type> kleafc = {
    {1.6662, 1.6662, 1.6662, 1.6662, 1.6662, 1.6662, 1.6662, 1.6662, 1.6662,
     1.6662, 1.6662, 1.6662}};
const std::array<double, nvegetation_type> cov = {
    {-0.001187, -0.000452, -0.000664, -0.022846, -0.022846, -0.026209,
     -0.026209, -0.016209, -0.009455, -0.000227, -0.001187, -0.00034}};
// 50==================================================
namespace ecohydrology
{
class canopy
{
public:
  canopy();
  ~canopy();

  double dCanopy_carbon;
  double dCanopy_lai;
  double dCanopy_lai_previous;
  double dScalar_foliage; // the foliage scalar, unit: fracton between 0 and 1
  double dkleafc;         // parameter from tem model
  double dcov;            // parameter from tem model

  interception cInterception;
  litterfall cLitterfall;
  photosynthesis cPhotosynthesis;
  int calculate_foliage_scalar(int iVegetation_type);

  int initialize_canopy(eColumn_type eCU_type, int iVegetation_type);

  int run_canopy_model(int iMonth, int iPrecipitation_type,
                       int iVegetation_type, double dArea_in, double dEt_in,
                       double dFpar_in, double dLai_in, double dRain_in,
                       double dSnow_in, double dPar_in,
                       double dTemperature_mean_in,
                       double dVapor_pressure_deficit_in);

  int update_canopy_parameter(int iVegetation_type);
  int update_canopy_status(int iVegetation_type);

  // double dCanopy_nitrogen; // the nitrogen content in the canopy, units: kg *
  // m(-2)
};
} // namespace ecohydrology
