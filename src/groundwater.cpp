#include "groundwater.h"
namespace ecohydrology
{
groundwater::groundwater()
{
  dGroundwater_inflow = missing_value;
  dGroundwater_inflow_gravity_volume = missing_value;
  dGroundwater_inflow_soil_volume = missing_value;
  dGroundwater_outflow = missing_value;
  dGroundwater_outflow_sink = missing_value;
  dGroundwater_inflow_upslope = missing_value;
  dGroundwater_storage = missing_value;
}
groundwater::~groundwater() {}

int groundwater::initialize_groundwater()
{
  int error_code = 1;

  dGroundwater_inflow = 0.0;
  dGroundwater_inflow_gravity_volume = 0.0;
  dGroundwater_inflow_soil_volume = 0.0;
  dGroundwater_outflow = 0.0;
  dGroundwater_outflow_sink = 0.0;
  dGroundwater_inflow_upslope = 0.0;
  dGroundwater_storage = 0.0;
  dGroundwater_storage_minimum = 1.0;
  return error_code;
}
int groundwater::update_groundwater_parameter()
{
  int error_code = 1;
  return error_code;
}
// calculate groundwater flow=========================
// input
// dSoil_to_groundwater_hru_in, unit cm
// dGravity_drainage_hru_in, unit, cm
//
int groundwater::calculate_groundwater_flow(double dSoil_to_groundwater_in,
                                            double dGravity_drainage_in)
{
  int error_code = 1;
  // temporal variables
  double dGroundwater_inflow_volume;
  double dGroundwater_outflow_volume;
  double dGroundwater_upslope_volume;
  double dGroundwater_sink_volume;
  double dStorage_groundwater_volume;
  double dStorage_minimum_volume;
  // inflow from soil capillary
  dGroundwater_inflow_soil_volume =
      dSoil_to_groundwater_in * dArea; // unit, cubic meter
  // inflow from gravity drainage
  dGroundwater_inflow_gravity_volume =
      dGravity_drainage_in * dArea; // unit, cubic meter;
  // current storage
  dStorage_groundwater_volume =
      dGroundwater_storage * dArea; // unit, cubic meter;
  // minimum storage
  dStorage_minimum_volume =
      dGroundwater_storage_minimum * dArea; // unit, cubic meter
  if (dStorage_groundwater_volume < dStorage_minimum_volume)
    {
      dStorage_groundwater_volume = dStorage_minimum_volume;
    }
  else
    {
      // nothing here
    }
  // from groundwater cascade
  dGroundwater_upslope_volume = dGroundwater_inflow_upslope * dArea;
  // sum up to the total inflow
  dGroundwater_inflow_volume = dGroundwater_inflow_soil_volume +
                               dGroundwater_inflow_gravity_volume +
                               dGroundwater_upslope_volume;
  // add inflow to current storage
  dStorage_groundwater_volume =
      dStorage_groundwater_volume + dGroundwater_inflow_volume;
  // calculate outflow
  dGroundwater_outflow_volume = dStorage_groundwater_volume * groundwater_coef;
  // remove outflow
  dStorage_groundwater_volume =
      dStorage_groundwater_volume - dGroundwater_outflow_volume;
  // check?

  // calculate sink
  dGroundwater_sink_volume = dStorage_groundwater_volume * gwsink_coef;
  // remove sink
  dStorage_groundwater_volume =
      dStorage_groundwater_volume - dGroundwater_sink_volume;

  if (dStorage_groundwater_volume < dStorage_minimum_volume)
    {
      dStorage_groundwater_volume = dStorage_minimum_volume;
    }
  else
    {
      // nothing here
    }

  dGroundwater_storage = dStorage_groundwater_volume / dArea;
  dGroundwater_outflow = dGroundwater_outflow_volume / dArea;
  return error_code;
}
// 50==================================================
int groundwater::run_groundwater_model(double dSoil_to_groundwater_hru_in,
                                       double dGravity_drainage_hru_in)
{
  int error_code = 1;
  calculate_groundwater_flow(dSoil_to_groundwater_hru_in,
                             dGravity_drainage_hru_in);
  return error_code;
}
} // namespace ecohydrology
