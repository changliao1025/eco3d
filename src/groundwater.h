#pragma once
#include <iostream>
// 50==================================================
#include "dimension.h"
#include "global.h"
// 50==================================================
using namespace std;

// 50==================================================
const double gwsink_coef = 0.01;
const double groundwater_coef = 0.01;
// 50==================================================
namespace ecohydrology
{
class groundwater
{
public:
  groundwater();
  ~groundwater();
  // 50==================================================
  // groundwater system variables
  double dGroundwater_inflow;          // total inflow, unit: cm
  double dGroundwater_outflow;         // outflow unit cm
  double dGroundwater_storage;         // total storage, unit, cm
  double dGroundwater_inflow_upslope;  // cascade groundwater flow, unit, cm
  double dGroundwater_inflow_soil;     // cascade groundwater flow, unit, cm
  double dGroundwater_inflow_external; // cascade groundwater flow, unit, cm
  double dGroundwater_inflow_gravity_volume; // recharge from gravity drainage,
                                             // unit: cm
  double
      dGroundwater_inflow_soil_volume; // recharge from soil capillary, unit, cm

  double dGroundwater_outflow_downslope; // outflow unit cm
  double dGroundwater_outflow_soil;      // outflow unit cm
  double dGroundwater_outflow_sink;      // outflow to sink unit cm

  double
      dGroundwater_storage_minimum; // the minimum storage, unit cm
                                    // 50==================================================
  // function
  // 50==================================================

  int calculate_groundwater_flow(double dSoil_to_groundwater_hru_in,
                                 double dGravity_drainage_hru_in);
  int initialize_groundwater();
  int run_groundwater_model(double dSoil_to_groundwater_hru_in,
                            double dGravity_drainage_hru_in);
  int update_groundwater_parameter();
};
} // namespace ecohydrology
