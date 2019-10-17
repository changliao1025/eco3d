// 50==================================================
/*!
  \file   reach.cpp
  \brief
  <long description>
  \author Chang Liao
  \date   2017-02-14
*/
// 50==================================================
#include "reach.h"

namespace ecohydrology
{
// 50==================================================
/*!
  Setup all the system variables
  \param void
*/
// 50==================================================
    reach::reach()
    {

      iFlag_just_routed = 0;
      //dReach_travel_time_default = 600; // second
      //dReach_travel_time_minute_default =
      //    dReach_travel_time_default / minute_2_second; // unit minutes
      //iTime_step = int(dTimestep_eco3d_minute / dReach_travel_time_minute_default);
      iTime_step = -9999;
      dReach_travel_time_default = missing_value;

      dReach_travel_time_minute = dReach_travel_time_minute_default;

      // 50==================================================
      dReach_discharge_inflow_daily = missing_value;
      dReach_discharge_inflow_daily_cumulative = missing_value;

      dReach_discharge_inflow_lateral_daily = missing_value;
      dReach_discharge_inflow_upstream_daily = missing_value;
      dReach_discharge_inflow_upstream_daily_cumulative = missing_value;
      dReach_discharge_inflow_external_daily = missing_value;
      dReach_discharge_inflow_groundwater_daily = missing_value;
      dReach_discharge_inflow_precipitation_daily = missing_value;

      dReach_discharge_outflow_daily = missing_value;
      dReach_discharge_outflow_daily_cumulative = missing_value;

      dReach_discharge_outflow_downstream_daily = missing_value;
      dReach_discharge_outflow_downstream_daily_cumulative = missing_value;
      dReach_discharge_outflow_groundwater_daily = missing_value;
      dReach_discharge_outflow_evapotranspiration_daily = missing_value;

      dReach_discharge_storage_daily = missing_value;
      // 50==================================================
      dReach_discharge_inflow_time_step = missing_value;

      dReach_discharge_inflow_upstream_time_step = missing_value;
      dReach_discharge_inflow_lateral_time_step = missing_value;
      dReach_discharge_inflow_external_time_step = missing_value;
      dReach_discharge_inflow_groundwater_time_step = missing_value;
      dReach_discharge_inflow_precipitation_time_step = missing_value;

      dReach_discharge_outflow_time_step = missing_value;
      dReach_discharge_outflow_downstream_time_step = missing_value;
      dReach_discharge_outflow_groundwater_time_step = missing_value;
      dReach_discharge_outflow_evapotranspiration_time_step = missing_value;

      dReach_discharge_storage_time_step = missing_value;

      dReach_discharge_inflow_previous_time_step = missing_value;
      dReach_discharge_inflow_upstream_previous_time_step = missing_value;
      dReach_discharge_outflow_previous_time_step = missing_value;

      // 50==================================================
      dReach_doc_inflow_daily = missing_value;
      dReach_doc_inflow_daily_cumulative = missing_value;

      dReach_doc_inflow_upstream_daily = missing_value;
      dReach_doc_inflow_upstream_daily_cumulative = missing_value;

      dReach_doc_inflow_lateral_daily = missing_value;
       dReach_doc_inflow_lateral_litter_daily= missing_value;
       dReach_doc_inflow_lateral_soil_daily= missing_value;
      dReach_doc_inflow_external_daily = missing_value;
      dReach_doc_inflow_groundwater_daily = missing_value;

      dReach_doc_outflow_daily = missing_value;
      dReach_doc_outflow_daily_cumulative = missing_value;

      dReach_doc_outflow_downstream_daily = missing_value;
      dReach_doc_outflow_downstream_daily_cumulative = missing_value;
      dReach_doc_outflow_groundwater_daily = missing_value;
      dReach_doc_storage_daily = missing_value;

      dReach_doc_inflow_time_step = missing_value;
      dReach_doc_inflow_previous_time_step = missing_value;

      dReach_doc_inflow_upstream_time_step = missing_value;

      dReach_doc_inflow_lateral_time_step = missing_value;

      dReach_doc_inflow_external_time_step = missing_value;
      dReach_doc_inflow_groundwater_time_step = missing_value;

      dReach_doc_outflow_time_step = missing_value;
      dReach_doc_outflow_previous_time_step = missing_value;
      dReach_doc_outflow_groundwater_time_step = missing_value;
      dReach_doc_outflow_downstream_time_step = missing_value;

      dReach_doc_storage_time_step = missing_value;

      dReach_doc_concentration_daily = missing_value;

      dReach_doc_concentration_time_step = missing_value;
      dReach_doc_concentration_previous_time_step = missing_value;
      dReach_carbon = missing_value;
    }

    reach::~reach()
    {
    }

///
/// \param eCU_type
/// \return
    int reach::initialize_reach(eColumn_type eCU_type)
    {
      int error_code = 1;
      this->eCU_type = eCU_type;
      // this->dReach_length = cGeology.dResolution;
      // this->dReach_slope = cGeology.dSlope;
      // double dVelocity; // units: m s(-1)
      if (eCU_type == eCU_stream) // stream
      {

        dReach_discharge_inflow_daily = 0.0;
        dReach_discharge_inflow_daily_cumulative = 0.0;

        dReach_discharge_inflow_lateral_daily = 0.0;

        dReach_discharge_inflow_upstream_daily = 0.0;
        dReach_discharge_inflow_upstream_daily_cumulative = 0.0;
        dReach_discharge_inflow_external_daily = 0.0;
        dReach_discharge_inflow_groundwater_daily = 0.0;
        dReach_discharge_inflow_precipitation_daily = 0.0;

        dReach_discharge_outflow_daily = 0.0;
        dReach_discharge_outflow_daily_cumulative = 0.0;
        dReach_discharge_outflow_downstream_daily = 0.0;
        dReach_discharge_outflow_downstream_daily_cumulative = 0.0;
        dReach_discharge_outflow_groundwater_daily = 0.0;
        dReach_discharge_outflow_evapotranspiration_daily = 0.0;

        dReach_discharge_storage_daily = 0.0;
        // 50==================================================
        dReach_discharge_inflow_time_step = 0.0;
        dReach_discharge_inflow_previous_time_step = 0.0;

        dReach_discharge_inflow_upstream_time_step = 0.0;
        dReach_discharge_inflow_lateral_time_step = 0.0;
        dReach_discharge_inflow_external_time_step = 0.0;
        dReach_discharge_inflow_groundwater_time_step = 0.0;
        dReach_discharge_inflow_precipitation_time_step = 0.0;

        dReach_discharge_outflow_time_step = 0.0;
        dReach_discharge_outflow_previous_time_step = 0.0;

        dReach_discharge_outflow_downstream_time_step = 0.0;
        dReach_discharge_outflow_groundwater_time_step = 0.0;
        dReach_discharge_outflow_evapotranspiration_time_step = 0.0;

        dReach_discharge_storage_time_step = 0.0;
        dReach_discharge_storage_previous_time_step = 0.0;


        dReach_discharge_inflow_upstream_previous_time_step = 0.0;


        // 50==================================================
        dReach_doc_inflow_daily = 0.0;
        dReach_doc_inflow_daily_cumulative = 0.0;
        dReach_doc_inflow_upstream_daily = 0.0;
        dReach_doc_inflow_upstream_daily_cumulative = 0.0;

        dReach_doc_inflow_lateral_daily = 0.0;
        dReach_doc_inflow_lateral_litter_daily= 0.0;
        dReach_doc_inflow_lateral_soil_daily= 0.0;
        dReach_doc_inflow_external_daily = 0.0;
        dReach_doc_inflow_groundwater_daily = 0.0;

        dReach_doc_outflow_daily = 0.0;
        dReach_doc_outflow_daily_cumulative = 0.0;
        dReach_doc_outflow_downstream_daily = 0.0;
        dReach_doc_outflow_downstream_daily_cumulative = 0.0;
        dReach_doc_outflow_groundwater_daily = 0.0;
        dReach_doc_storage_daily = 0.0;

        dReach_doc_inflow_time_step = 0.0;
        dReach_doc_inflow_previous_time_step = 0.0;

        dReach_doc_inflow_upstream_time_step = 0.0;

        dReach_doc_inflow_lateral_time_step = 0.0;

        dReach_doc_inflow_external_time_step = 0.0;
        dReach_doc_inflow_groundwater_time_step = 0.0;

        dReach_doc_outflow_time_step = 0.0;
        dReach_doc_outflow_previous_time_step = 0.0;

        dReach_doc_outflow_groundwater_time_step = 0.0;
        dReach_doc_outflow_downstream_time_step = 0.0;
        dReach_doc_storage_time_step = 0.0;
        dReach_doc_storage_previous_time_step = 0.0;
        dReach_doc_concentration_daily = 0.0;

        dReach_doc_concentration_time_step = 0.0;
        dReach_doc_concentration_previous_time_step = 0.0;

        dReach_travel_time = 600;
        dReach_travel_time_minute = 10;
        iTime_step = int(dTimestep_eco3d_minute / dReach_travel_time_minute);
        dReach_carbon = 0.0;
      }
      else
      {
        // something is wrong.
      }
      return error_code;
    }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int reach::update_reach_parameter()
    {
      int error_code = 1;
      double d;

      // calculate parameters
      k = dReach_travel_time_minute;
      x = x_coef;
      // k must be between 0.01 and 24
      d = k - (k * x) + 0.5 * dReach_travel_time_minute;
      if (d < 1.0E-4)
      {
        d = 0.0001;
      }
      else
      {
      }
      // calculate parameters
      c0 = (-(k * x) + (0.5 * dReach_travel_time_minute)) / d;
      c1 = (k * x + (0.5 * dReach_travel_time_minute)) / d;
      c2 = (k - k * x - 0.5 * dReach_travel_time_minute) / d;
      if (c2 < 0.0)
      {
        c1 = c1 + c2;
        c2 = 0.0;
      }
      if (c0 < 0.0)
      {
        c1 = c1 + c0;
        c0 = 0.0;
      }

      return error_code;
    }

// 50==================================================
/*!
  This is the minute resolution based stream route algorithm
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int reach::calculate_reach_discharge(int iMinute)
    {
      int error_code = 1;
      int iMod;
      double dReach_discharge_storage_minimal = 5.0;
      // convert from daily to minute

      // dReach_discharge_inflow_upstream_time_step MUST be assigned at time step
      // instead of daily step
      dReach_discharge_inflow_lateral_time_step =
              dReach_discharge_inflow_lateral_daily / iTime_step;
      dReach_discharge_inflow_external_time_step =
              dReach_discharge_inflow_external_daily / iTime_step;
      // the upstream is calculated from function

      // total inflow include 3 components
      dReach_discharge_inflow_time_step =
              dReach_discharge_inflow_upstream_time_step +
                      dReach_discharge_inflow_lateral_time_step +
                      dReach_discharge_inflow_external_time_step;

      if (dReach_discharge_inflow_time_step < tiny_value)
      {
        dReach_discharge_inflow_time_step = 0.0;
      }
      // minute flow

      // to describe the system status, the following variables
      // must be updated
      // dStorage_discharge_minute
      // dOutflow_discharge_minute
      // dInflow_discharge_previous_minute
      // time step
      iMod = iMinute % int(dReach_travel_time_minute); // whether it's time to route

      if (iMod == 0) // exactly the time
      {
        // change the flag
        iFlag_just_routed = 1;
        // calculate based the prms equation
        // outflow rate
        dReach_discharge_outflow_time_step =
                dReach_discharge_inflow_time_step * c0 +
                        dReach_discharge_inflow_previous_time_step * c1 +
                        dReach_discharge_outflow_previous_time_step * c2;

        if (dReach_discharge_outflow_time_step < tiny_value)
        {
          dReach_discharge_outflow_time_step = 0.0;
        }

        dReach_discharge_outflow_downstream_time_step
                = dReach_discharge_outflow_time_step;

        // storage
        dReach_discharge_storage_time_step =
                k * (
                        x * dReach_discharge_inflow_time_step
                                + (1.0 - x) * dReach_discharge_outflow_time_step
                );

        if (dReach_discharge_storage_time_step < dReach_discharge_storage_minimal)
        {

          dReach_discharge_storage_time_step = dReach_discharge_storage_minimal;

        }


      }
      else
      {
        iFlag_just_routed = 0;
        // if not the right time, nothing should change.
        // dStorage_discharge_minute mo change
        dReach_discharge_inflow_time_step = 0.0;
        dReach_discharge_outflow_time_step = 0.0;
      }

      return error_code;
    }

// 50==================================================
/*!
  calculate doc concentration
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int reach::calculate_reach_dissolved_organic_carbon_concentration(int iMinute)
    {
      int error_code = 1;
      int iMod;
      double dVolume;
      double dReach_doc_storage_total = 0.0;
      double dReach_doc_concentration_maximum = 0.1;
      double dReach_doc_concentration_minimal = tiny_value;


      iMod = iMinute % int(dReach_travel_time_minute); // whether it's time to route
      // dReach_doc_inflow_upstream_time_step must be at time step
      // convert from daily to minute
      dReach_doc_inflow_lateral_daily = dReach_doc_inflow_lateral_litter_daily
              + dReach_doc_inflow_lateral_soil_daily;

      dReach_doc_inflow_lateral_time_step =
              dReach_doc_inflow_lateral_daily / iTime_step; // kg
      dReach_doc_inflow_external_time_step =
              dReach_doc_inflow_external_daily / iTime_step;
      // the upstream is calculated from function

      // doc include 3 components
      dReach_doc_inflow_time_step = dReach_doc_inflow_upstream_time_step +
              dReach_doc_inflow_lateral_time_step +
              dReach_doc_inflow_external_time_step; // kg
      if (dReach_doc_inflow_time_step < near_zero)
      {
        dReach_doc_inflow_time_step = 0.0;
      }

      dReach_doc_storage_total = dReach_doc_inflow_time_step
              + dReach_doc_storage_previous_time_step;
      // to describe the system status,

      // dStorage_discharge_minute is from discharge function
      // dDoc_concentration_minute
      // dStorage_doc_minute
      // dOutflow_doc_minute


      if (iMod == 0)
      {
        // see illustration for proof
        dVolume = dReach_discharge_storage_time_step +
                dReach_discharge_outflow_time_step;
        //we require the minial storage must be greater than 100cubic meter for the whole reach


        // the new concentration base on total mass and volume
        dReach_doc_concentration_time_step =
                dReach_doc_storage_total / dVolume;

        if (dReach_doc_concentration_time_step < dReach_doc_concentration_minimal)
        {
          dReach_doc_concentration_time_step = dReach_doc_concentration_minimal;
        }
        if (dReach_doc_concentration_time_step > dReach_doc_concentration_maximum)
        {
          dReach_doc_concentration_time_step = dReach_doc_concentration_maximum;
          //cout << "something is wrong!" << endl;
        }
        // convert concentration back to mass to update
        dReach_doc_storage_time_step =
                dReach_doc_concentration_time_step *
                        dReach_discharge_storage_time_step;

        dReach_doc_outflow_time_step =
                dReach_doc_concentration_time_step *
                        dReach_discharge_outflow_time_step;



      }
      else
      {
        // if not the right time, nothing changes
        dReach_doc_inflow_time_step = 0.0;
        dReach_doc_outflow_time_step = 0.0;
      }

      return error_code;
    }

// 50==================================================
/*!
  <long-description>

  \param
  \return <ReturnValue>
*/
// 50==================================================
    int reach::run_reach_model(int iMinute)
    {
      int error_code = 1;
      calculate_reach_discharge(iMinute);
      calculate_reach_dissolved_organic_carbon_concentration(iMinute);
      return error_code;
    }

    int reach::update_reach_time_step_status()
    {
      int error_code = 1;

      dReach_discharge_inflow_previous_time_step =
              dReach_discharge_inflow_time_step;

      dReach_discharge_inflow_upstream_previous_time_step =
              dReach_discharge_inflow_upstream_time_step;

      dReach_discharge_outflow_previous_time_step =
              dReach_discharge_outflow_time_step;

      dReach_discharge_storage_previous_time_step = dReach_discharge_storage_time_step;

      dReach_doc_inflow_previous_time_step = dReach_doc_inflow_time_step;
      dReach_doc_outflow_previous_time_step = dReach_doc_outflow_time_step;



      //dReach_doc_storage_previous_time_step = dReach_doc_storage_time_step;

      dReach_doc_concentration_previous_time_step = dReach_doc_concentration_time_step;
      dReach_doc_storage_previous_time_step = dReach_doc_concentration_previous_time_step
              * dReach_discharge_storage_previous_time_step;

      return error_code;
    }


    int reach::update_reach_status()
    {
      int error_code = 1;
      dReach_discharge_inflow_daily =
              dReach_discharge_inflow_daily_cumulative;

      dReach_discharge_inflow_upstream_daily =
              dReach_discharge_inflow_upstream_daily_cumulative;

      // lateral flow

      dReach_discharge_outflow_daily =
              dReach_discharge_outflow_daily_cumulative;

      dReach_discharge_outflow_downstream_daily =
              dReach_discharge_outflow_downstream_daily_cumulative;

      dReach_discharge_storage_daily = dReach_discharge_storage_time_step;

      //doc
      dReach_doc_inflow_daily = dReach_doc_inflow_daily_cumulative;

      dReach_doc_inflow_upstream_daily =
              dReach_doc_inflow_upstream_daily_cumulative;



      // dReach_doc_inflow_lateral_daily

      dReach_doc_outflow_daily = dReach_doc_outflow_daily_cumulative;

      dReach_doc_outflow_downstream_daily =
              dReach_doc_outflow_downstream_daily_cumulative;

      dReach_doc_storage_daily = dReach_doc_storage_time_step;

      dReach_doc_concentration_daily = dReach_doc_concentration_time_step;
      dReach_carbon =
              dReach_doc_concentration_daily * dReach_discharge_storage_daily;
      return error_code;
    }
// 50==================================================
/*!
  This function is used to calculate the travel time, but it may not be actually
  used for now.

  \param
  \return <ReturnValue>
*/
// 50==================================================
/// int reach::calculate_reach_travel_time()
///{
///	int error_code = 1;
///
///	return error_code;
///}
} // namespace ecohydrology
