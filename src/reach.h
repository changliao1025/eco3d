// 50==================================================
/*!
  The stream segment reach class
  \brief
  This class mainly simulate process within a segement reach.
  \author Chang Liao
  \date 2017-02-14
*/
// 50==================================================
#pragma once

#include <array>
#include <cmath>
// 50==================================================
#include "dimension.h"
#include "geology.h"
#include "global.h"
#include "groundwater.h"
#include "hru.h"
#include "landcover.h"
#include "system.h"

// 50==================================================
// parameter from prms model
const double k_coef = 1.0; // travel time, unit minute
const double x_coef = 0.3;
// using namespace std;
// 50==================================================
namespace ecohydrology
  {
      class reach
      {
      public:
          reach();

          ~reach();

          eColumn_type eCU_type;
          int iTime_step; // all reaches are simulated at daily outter time step 24* 60
          // mintes

          int iFlag_just_routed;
          double dReach_travel_time;
          double dReach_travel_time_default; // unit: second
          double dReach_travel_time_minute;
          double dReach_travel_time_minute_default;
          double dReach_discharge_inflow_daily;
          double dReach_discharge_inflow_daily_cumulative;

          double dReach_discharge_inflow_lateral_daily;
          double dReach_discharge_inflow_upstream_daily;
          double dReach_discharge_inflow_upstream_daily_cumulative;

          double dReach_discharge_inflow_external_daily;
          double dReach_discharge_inflow_groundwater_daily;
          double dReach_discharge_inflow_precipitation_daily;
          double dReach_discharge_outflow_daily;
          double dReach_discharge_outflow_daily_cumulative;

          double dReach_discharge_outflow_downstream_daily;

          double dReach_discharge_outflow_downstream_daily_cumulative;
          double dReach_discharge_outflow_groundwater_daily;
          double dReach_discharge_outflow_evapotranspiration_daily;
          double dReach_discharge_storage_daily;
          double dReach_discharge_inflow_time_step;
          double dReach_discharge_inflow_lateral_time_step;
          double dReach_discharge_inflow_upstream_time_step;
          double dReach_discharge_inflow_external_time_step;
          double dReach_discharge_inflow_groundwater_time_step;
          double dReach_discharge_inflow_precipitation_time_step;
          double dReach_discharge_outflow_time_step;
          double dReach_discharge_outflow_downstream_time_step;
          double dReach_discharge_outflow_groundwater_time_step;
          double dReach_discharge_outflow_evapotranspiration_time_step;
          double dReach_discharge_storage_time_step;

          double dReach_discharge_inflow_previous_time_step;
          double dReach_discharge_inflow_upstream_previous_time_step;
          double dReach_discharge_storage_previous_time_step;


          double dReach_discharge_outflow_previous_time_step;


          double dReach_doc_inflow_daily;
          double dReach_doc_inflow_daily_cumulative;
          double dReach_doc_inflow_upstream_daily;
          double dReach_doc_inflow_upstream_daily_cumulative;
          double dReach_doc_inflow_lateral_daily;
          double dReach_doc_inflow_lateral_litter_daily;
          double dReach_doc_inflow_lateral_soil_daily;

          double dReach_doc_inflow_external_daily;
          double dReach_doc_inflow_groundwater_daily;
          double dReach_doc_outflow_daily;
          double dReach_doc_outflow_daily_cumulative;
          double dReach_doc_outflow_downstream_daily;
          double dReach_doc_outflow_downstream_daily_cumulative;
          double dReach_doc_outflow_groundwater_daily;
          double dReach_doc_storage_daily;
          double dReach_doc_inflow_time_step;
          double dReach_doc_inflow_previous_time_step;

          double dReach_doc_inflow_upstream_time_step;
          double dReach_doc_inflow_lateral_time_step;
          double dReach_doc_inflow_external_time_step;
          double dReach_doc_inflow_groundwater_time_step;
          double dReach_doc_outflow_time_step;
          double dReach_doc_outflow_previous_time_step;
          double dReach_doc_outflow_groundwater_time_step;
          double dReach_doc_outflow_downstream_time_step;
          double dReach_doc_storage_time_step;
          double dReach_doc_storage_previous_time_step;

          double dReach_doc_concentration_daily;
          double dReach_doc_concentration_time_step;
          double dReach_doc_concentration_previous_time_step;
          double dReach_carbon;
          // 50==================================================
          double k;          // travel time
          double x;          // mannning ?
          double c0, c1, c2; // parameter for prms
          geology cGeology;
          hru cHru;
          groundwater cGroundwater;
          landcover cLandcover;

          int calculate_reach_discharge(int iMinute);

          int calculate_reach_dissolved_organic_carbon_concentration(int iMinute);

          int initialize_reach(eColumn_type eCU_type);

          int run_reach_model(int iMinute);

          int update_reach_parameter();

          int update_reach_time_step_status();

          int update_reach_status();
          // int calculate_reach_travel_time();
          // double dReach_length; // length of reach, 500 meter currently
          // double dReach_slope; //the slope the reach bed
          // double dReach_inflow_doc_concentration_time_step; // doc concentration, kg
          // m(-3) double dReach_inflow_upstream_doc_concentration_time_step; double
          // dReach_inflow_lateral_doc_concentration_time_step; double
          // dReach_inflow_external_doc_concentration_time_step;
          // double dReach_doc_outflow_downstream_time_step;
          // double dReach_doc_outflow_groundwater_time_step;
          // double dReach_doc_inflow_groundwater_time_step;
          // double dReach_inflow_doc_concentration_daily; // doc concentration, kg
          // m(-3) double dReach_inflow_upstream_doc_concentration_daily; double
          // dReach_inflow_lateral_doc_concentration_daily; double
          // dReach_inflow_external_doc_concentration_daily;
          //
          // double dReach_discharge_inflow_groundwater_time_step;
          // double dReach_discharge_inflow_precipitation_time_step;
          // double dReach_discharge_outflow_downstream_time_step;
          // double dReach_discharge_outflow_groundwater_time_step;
          // double dReach_discharge_outflow_evapotranspiration_time_step;
      };
  } // namespace ecohydrology
