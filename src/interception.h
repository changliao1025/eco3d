#pragma once

#include <array>
#include <cmath>
#include <iostream>
// 50==================================================
#include "dimension.h"
#include "global.h"
// 50==================================================
using namespace std;

// 50==================================================
// the paramters are obtained from prms user guide

// the maximum depth of water on canopy, unit meter

const std::array<double, nvegetation_type> aCanopy_depth_capacity_rain = {
    {0.00254, 0.00254, 0.00254, 0.00254, 0.00254, 0.00254, 0.000254, 0.000254,
     0.000254, 0.000254, 0.000254, 0.000254}};

const std::array<double, nvegetation_type> aCanopy_depth_capacity_snow = {
    {0.00254, 0.00254, 0.00254, 0.00254, 0.00254, 0.00254, 0.00254, 0.00254,
     0.00254, 0.00254, 0.00254, 0.00254}};

// the parameter for et from canopy
const std::array<double, nmonth> aCoefficient_pan = {
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
// 50==================================================
namespace ecohydrology
{
class interception
{
public:
  interception();

  ~interception();

  int iIntercept_type;

  int iPrecipitation_type;

  double dLai;
  double dLai_previous;
  // 50==================================================
  // since interception only applies to area with canopy coverage
  // in this module, we convert the variable which specify the state of canopy
  // to the grid averaged state. This is done using the lai, and the average
  // state variable name  all have a suffix "_hru"
  // 50==================================================
  double dCanopy_depth;
  double dCanopy_depth_hru;
  double dCanopy_depth_rain;

  double dCanopy_depth_snow;
  // double dCanopy_depth_rain_hru;
  // double dCanopy_depth_snow_hru;

  double dCanopy_storage;
  double dCanopy_storage_rain;

  double dCanopy_storage_snow;
  double dEt_canopy_rain;
  double dEt_canopy_snow;
  double dEt_canopy;
  double dEt_canopy_hru;

  double dPrecipitation_net;
  double dRain_net;
  double dSnow_net;
  // 50==================================================
  // the difference between net precipitation and throughfall is that
  // throughfall only counts for precipitation associated with LAI
  // 50==================================================
  double dThroughfall;
  double dThroughfall_rain;
  double dThroughfall_snow;

  double dCanopy_depth_capacity_rain;
  double dCanopy_depth_capacity_snow;

  double calculate_canopy_depth_capacity(int iFlag_rain_or_snow, int iMonth,
                                         int iVegetation_type);

  double calculate_canopy_evapotranspiration(int iIntercept_type, int iMonth,
                                          double dEt_in);

  std::array<double, 3>
  calculate_canopy_interception(double dArea_in, double dCanopy_depth_in,
                                double dCanopy_depth_capacity_in,
                                double dLai_in, double dPrecipitation_in);

  int initialize_interception(eColumn_type eCU_type, int iVegetation_type);

  int run_interception_model(int iMonth, int iPrecipitation_type,
                             int iVegetation_type, double dArea_in,
                             double dEt_in, double dLai_in, double dRain_in,
                             double dSnow_in, double dTemperature_in);

  int update_interception_parameter(int iVegetation_type);
  int update_interception_status(int iVegetation_type);
};
} // namespace ecohydrology
