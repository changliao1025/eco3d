// 50==================================================
/*!
  \file surface_runoff.h
  \brief
  The surface runoff model
  \author Chang Liao
  \date 2017-01-30
*/
// 50==================================================
#pragma once
#include <array>
#include <iostream>
// 50==================================================
#include "dimension.h"
#include "global.h"
#include "infiltration.h"
using namespace std;
// 50==================================================
const double dStorage_impervious_max = 0.05 * inch_2_meter;

const std::array<double, nlandcover_type> aContribution_area_max = {
    {0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6,
     0.6}};

const std::array<double, nlandcover_type> aContribution_area_min = {
    {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.2, 0.2, 0.2, 0.2, 0.2,
     0.2}};
// 50==================================================
namespace ecohydrology
{
class surface_runoff
{
public:
  surface_runoff();
  ~surface_runoff();
  // 50==================================================
  // system state variables
  // 50==================================================
  // in this model, the impervious surface is supposed to extend to the deeper
  // bottom at current setup_eco3d,
  // which means there will no water and soil under the impervious surface as
  // well
  // as a result, many variable have two forms, one is the intensity on pervious
  // surface and the other one is the average
  // for example, if the infiltration in the pervious zone is 0.005 meter/day,
  // and dFraction_impervious is 0.4.
  // the average infiltration, called as dInfiltration_hru is 0.005 * (1.0 -
  // 0.4) = 0.003 meter /day we still use the "_hru" to denote the hru average
  // when applicable
  int iFlag_infiltration;

  double dImpervious_surface_et;
  double dImpervious_surface_et_hru;
  double dImpervious_surface_percentage;

  double dSurface_runoff_impervious;
  double dSurface_runoff_impervious_hru;

  double dSurface_storage_impervious;
  double dSurface_storage_impervious_hru;
  // 50==================================================
  // pervious area
  // 50==================================================
  // the fraction of pervious surface, usually equals to 1.0 -
  // dFraction_impervious,

  double dPervious_surface_percentage;

  double dSurface_runoff_pervious;
  double dSurface_runoff_pervious_hru;

  double dSurface_runoff_inflow;
  double dSurface_runoff_outflow;

  double dSurface_runoff_storage;
  double dSurface_runoff_inflow_external;

  // Hortonian runoff process

  double dSurface_runoff_inflow_upslope;
  // double dSurface_runoff_outflow_dunnian;

  double dSurface_runoff_outflow_infiltration;

  double dSurface_runoff_outflow_downslope;
  double dSurface_runoff_hru;

  double dSurface_et_hru;

  double dContribution_area;

  double dContribution_area_max;

  double dContribution_area_min;

  infiltration cInfiltration;

  int calculate_impervious_surface_runoff(double dEt_available_in,
                                          double dSca_in,
                                          double dWater_available_in);
  int calculate_impervious_surface_evapotranspiration(
      double dEt_available_in, double dSca_in, double dWater_available_in);
  int calculate_impervious_surface_storage(double dWater_available_in);
  int calculate_pervious_surface_runoff(
      int iFlag_old_snow, int iPrecipitation_type, double dSoil_moisture_in,
      double dSoil_moisture_max_in, double dStorage_recharge_zone_in,
      double dStorage_recharge_zone_max_in, double dTemperature_mean_in,
      double dWater_available_pervious_in);
  double calculate_variable_source_area(double dSoil_recharge_in,
                                        double dSoil_recharge_max_in);
  int initialize_surface_runoff(eColumn_type eCU_type);
  int run_surface_runoff_model(
      int iFlag_old_snow, int iFlag_has_snow, eColumn_type eCU_type,
      int iLand_cover_type, int iPrecipitation_type,
      double dET_available_hru_in, double dRain_net_in, double dSnow_net_in,
      double dSCA_in, double dSnowmelt_in, double dSoil_moisture_in,
      double dSoil_moisture_max_in, double dSoil_temperature_in,
      double dStorage_recharge_zone_in, double dStorage_recharge_zone_max);

  int update_surface_runoff_parameter(int iLand_cover_type, int iSoil_type);
};
} // namespace ecohydrology
