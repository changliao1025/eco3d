// 50==================================================
/*!
  \File soil.cpp
  \brief
  <long description>
  \author
  \date 2016-10-03
*/
// 50==================================================
#include "soil.h"

namespace ecohydrology
{
// 50==================================================
// this is the main soil source file, which calls a list of soil sub-models
// including soil water model, soil carbon model and soil thermal model.
// 50==================================================
// 50==================================================
/*!
  <long-description>
  \param void
*/
// 50==================================================
    soil::soil()
    {
      // does soil type change with time? but vegetaion will change
      // initialize all the necessary system variables
      iSoil_type_default = 1;
      dPreferential_inflow = missing_value;

      // dDunnian_runoff_outflow = missing_value;

      dCapillary_inflow = missing_value;
      // 50==================================================
      dSoil_water_storage_preferential = missing_value;
      dSoil_water_storage_gravity = missing_value;
      dSoil_water_storage_moisture = missing_value;
      dSoil_water_storage_recharge_zone = missing_value;
      dStorage_soil_moisture_lower = missing_value;

      // 50==================================================
      dSoil_moisture_volume = missing_value;

      dSoil_moisture_to_groundwater = missing_value;
      dGravity_inflow = missing_value;

      dGravity_to_preferential = missing_value;

      dSoil_water_interflow = missing_value;
      dSoil_interflow_inflow = missing_value;
      dSoil_water_interflow_outflow = missing_value;
      dSoil_water_interflow_storage = missing_value;

      // dSoil_interflow_inflow_upslope= missing_value;
      dSoil_water_interflow_inflow_external = missing_value;
      // dSoil_interflow_inflow_groundwater= missing_value;
      dSoil_water_outflow_downslope = missing_value;
      dSoil_water_outflow_groundwater = missing_value;

      dSoil_interflow_slow = missing_value;
      dSoil_interflow_fast = missing_value;
      dSoil_gravity_drainage = missing_value;
      dET_pervious = missing_value;
      dThreshold_preferential = missing_value;
      dThreshold_saturation = missing_value;

      dSoil_water_dunnian_runoff_gravity = missing_value;
      // dDunnian_runoff_inflow_upslope = missing_value;
      dSoil_water_dunnian_runoff_preferential = missing_value;
      // 50==================================================
      // DOC
      // 50==================================================
      dSoil_carbon = missing_value;
      dSoil_soc = missing_value;
      dSoil_doc_concentration = missing_value;
      dSoil_spsoc_mass = missing_value;
      dSoil_doc_storage = missing_value;        //
      dSoil_doc_production = missing_value;     // kg
      dSoil_doc_mineralization = missing_value; // kg
      dSoil_doc_adsorption = missing_value;
      dSoil_doc_desorption = missing_value;
      dSoil_doc_inflow_upslope = missing_value;
      dSoil_doc_outflow_downslope = missing_value;
      dSoil_spsoc_mineralization = missing_value; // kg
      dSoil_spsoc_adsorption = missing_value;
      dSoil_spsoc_desorption = missing_value;

      dSoil_interflow_fast_downslope = missing_value;
      dSoil_interflow_fast_upslope = missing_value;
      dSoil_interflow_slow_downslope = missing_value;
      dSoil_interflow_slow_upslope = missing_value;

      dSoil_heterotrophic_respiration = missing_value;;
    }

    soil::~soil()
    {
    }

// 50==================================================
// step 1
/*!
  Calculate the inflow into the preferential zone
  \param dInfiltration_hru_in: the infiltration from surface
  \param : the fraction of preferential pore in soil
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_preferential_inflow(double dInfiltration_hru_in,
                                                 double dPreferential_percentage_in)
    {
      int error_code = 1;
      double dummy0;
      double dummy1;
      double dInfiltration;
      if (dFraction_pervious < small_value)
      {
        dInfiltration = 0.0;
      }
      else
      {
        dInfiltration = dInfiltration_hru_in;
      }
      dummy0 = dSoil_interflow_fast_upslope;
      // calculate the preferential flow based on preferential fraction

      dummy1 = dInfiltration * dPreferential_percentage_in;
      dPreferential_inflow = dummy0 + dummy1;
      if (dPreferential_inflow < tiny_value)
      {
        dPreferential_inflow = 0.0;
      }
      return error_code;
    }
// 50==================================================
// step 2
/*!
  Calculate the storage in the preferential reservoir and any excessive dunnian
  runoff \param \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_preferential_storage()
    {
      int error_code = 1;
      double dStorage_available = dSoil_storage_preferential_max - dSoil_water_storage_preferential;
      double dDifference_flow;

      if (abs(dStorage_available) < tiny_value)
      {

        // all inflow goes out to dunnian flow
        dSoil_water_storage_preferential = dSoil_storage_preferential_max;
        dSoil_water_dunnian_runoff_preferential = dPreferential_inflow;
      }
      else
      {
        if (dStorage_available <= -tiny_value)
        {
          // impossible
        }
        else
        {
          dDifference_flow = dStorage_available - dPreferential_inflow;
          if (abs(dDifference_flow) < tiny_value)
          {
            dSoil_water_storage_preferential = dSoil_storage_preferential_max;
            dSoil_water_dunnian_runoff_preferential = 0.0;
          }
          else
          {
            if (dDifference_flow <= -near_zero)
            {
              dSoil_water_storage_preferential = dSoil_storage_preferential_max;
              dSoil_water_dunnian_runoff_preferential = -dDifference_flow;
            }
            else
            {
              dSoil_water_storage_preferential =
                      dSoil_water_storage_preferential + dPreferential_inflow;
              // no dunnian flow
              dSoil_water_dunnian_runoff_preferential = 0.0;
            }
          }
        }
      }



      // quality control
      if (dSoil_water_storage_preferential < tiny_value)
      {
        dSoil_water_storage_preferential = 0.0;
      }
      if (dSoil_water_dunnian_runoff_preferential < tiny_value)
      {
        dSoil_water_dunnian_runoff_preferential = 0.0;
      }
      return error_code;
    }
// 50==================================================
/*!//step 3
  Route the rest infiltration into the capillary reservoir
  This is where surface cascade comes into play
  \param dInfiltration_hru_in: unit: meter
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_capillary_inflow(double dInfiltration_hru_in)
    {
      int error_code = 1;
      double dummy0 = 0.0;
      double dummy1 = 0.0;
      if (dFraction_pervious < small_value)
      {
        dCapillary_inflow = 0.0;
      }
      else
      {
        // there is pervious area
        // cascade effects from outside the grid
        dummy0 = dSoil_interflow_slow_upslope;
        // from a fraction of infiltration
        dummy1 = dInfiltration_hru_in * (1.0 - dFraction_preferential);
        // sum up
        dCapillary_inflow = dummy0 + dummy1;

        if (dCapillary_inflow < tiny_value)
        {
          dCapillary_inflow = 0.0;
        }
      }

      return error_code;
    }
// 50==================================================
/*!//step 4
  Calculate the capillary reservoir recharge zone storage
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_recharge_zone()
    {
      int error_code = 1;
      double dStorage_available = 0.0;
      double dDifference_storage = 0.0;
      // calculate the availabel space
      dStorage_available = dSoil_storage_recharge_zone_max - dSoil_water_storage_recharge_zone;
      // check whether there is still space
      if (abs(dStorage_available) < tiny_value)
      {
        // there is no more space to add water
        // dStorage_recharge_zone is still at maximum
        dSoil_water_storage_recharge_zone = dSoil_storage_recharge_zone_max;
      }
      else
      {
        if (dStorage_available <= -tiny_value)
        {
          // this is impossible
        }
        else
        {
          dDifference_storage = dStorage_available - dCapillary_inflow;
          if (abs(dDifference_storage) < tiny_value)
          {
            // inflow is just enough
            dSoil_water_storage_recharge_zone = dSoil_storage_recharge_zone_max;
          }
          else
          {
            if (dDifference_storage <= -tiny_value)
            {
              // too much inflow
              dSoil_water_storage_recharge_zone = dSoil_storage_recharge_zone_max;
            }
            else
            {
              dSoil_water_storage_recharge_zone =
                      dSoil_water_storage_recharge_zone + dCapillary_inflow;
            }
            if (dSoil_water_storage_recharge_zone < tiny_value)
            {
              dSoil_water_storage_recharge_zone = 0.0;
            }
          }
        }
      }

      return error_code;
    }
// 50==================================================
/*!//step 5 and 6
  still in the capillary zone, but to the max soil moisture
  the max moisture is larger than the recharge zone capacity
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_moisture()
    {
      int error_code = 1;
      double dStorage_available = 0.0;
      double dDifference_flow = 0.0;

      // calculate the remaining soil moisture available
      dStorage_available = dSoil_storage_moisture_max - dSoil_water_storage_moisture;
      // check whether there is space or not
      if (abs(dStorage_available) < tiny_value)
      {

        // moisture is at maximum
        // everything must go to groundwater first
        dDifference_flow = dSoil_moisture_to_groundwater_max - dCapillary_inflow;
        if (abs(dDifference_flow) < tiny_value)
        {

          // max groundwater recharge rate, the remaining is 0.0
          dSoil_moisture_to_groundwater = dSoil_moisture_to_groundwater_max;
          dGravity_inflow = 0.0;
        }
        else
        {
          if (dDifference_flow <= -tiny_value)
          {
            dSoil_moisture_to_groundwater = dSoil_moisture_to_groundwater_max;
            dGravity_inflow = -dDifference_flow;
          }
          else
          {
            // excess is not too much
            // everything goes to groundwater first
            dSoil_moisture_to_groundwater = dCapillary_inflow;
            dGravity_inflow = 0.0;
          }
        }

        // reset real inflow to capillary
        dCapillary_inflow = 0.0;
      }
      else
      {
        if (dStorage_available <= -tiny_value)
        {
          // impossible
        }
        else
        {
          // there is still space
          // get the difference between inflow and available
          dDifference_flow = dStorage_available - dCapillary_inflow;
          if (abs(dDifference_flow) < tiny_value)
          {
            // they are the same
            dSoil_water_storage_moisture = dSoil_storage_moisture_max;
            dSoil_moisture_to_groundwater = 0.0;
            dGravity_inflow = 0.0;
          }
          else
          {
            if (dDifference_flow <= -tiny_value)
            {
              // inflow exceeds available by -dDifference_flow
              // moisture reaches maximum
              dSoil_water_storage_moisture = dSoil_storage_moisture_max;
              // the real inflow be adjusted
              dCapillary_inflow = dStorage_available;
              // route the rest inflow to groundwater
              // any excess is first applied to grondwater reservoir
              dDifference_flow =
                      dSoil_moisture_to_groundwater_max - (-dDifference_flow);

              if (abs(dDifference_flow) < tiny_value)
              {
                // they are the same
                dSoil_moisture_to_groundwater = dSoil_moisture_to_groundwater_max;
                dGravity_inflow = 0.0;
              }
              else
              {
                if (dDifference_flow <= -tiny_value)
                {
                  // excess(dDifference_flow) exceeds maximum
                  // groundwater recharge rate, the remaining goes to
                  // gravity
                  dSoil_moisture_to_groundwater = dSoil_moisture_to_groundwater_max;
                  dGravity_inflow = -dDifference_flow;
                }
                else
                {
                  // excess is not too much for groundwater recharge
                  dSoil_moisture_to_groundwater = dDifference_flow;
                  // no excess go to gravity
                  dGravity_inflow = 0.0;
                }
              }
            }
            else
            {
              // inflow is too less
              // add inflow to moisture
              dSoil_water_storage_moisture =
                      dSoil_water_storage_moisture + dCapillary_inflow;
              if (dSoil_water_storage_moisture < tiny_value)
              {
                dSoil_water_storage_moisture = 0.0;
              }
              // others are also 0.0
              dSoil_moisture_to_groundwater = 0.0;
              dGravity_inflow = 0.0;
            }
          }
        }
      }

      // set soil lower zone moisture
      dStorage_soil_moisture_lower =
              dSoil_water_storage_moisture - dSoil_water_storage_recharge_zone;
      if (abs(dStorage_soil_moisture_lower) < tiny_value)
      {
        dStorage_soil_moisture_lower = 0.0;
      }
      else
      {
        if (dStorage_soil_moisture_lower <= -tiny_value)
        {
          dStorage_soil_moisture_lower = 0.0;
        }
      }

      return error_code;
    }
// 50==================================================
// step 7
/*!
  Calculate the gravity reservoir
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_gravity_reservoir()
    {
      int error_code = 1;
      double dStorage_available = dSoil_storage_gravity_max - dSoil_water_storage_gravity;
      double dDifference_storage = 0.0;

      // check whether there is space or not

      if (abs(dStorage_available) < tiny_value)
      {
        // they are close
        // if there is no space left
        // gravity storage reaches maximum
        dSoil_water_storage_gravity = dSoil_storage_gravity_max;
        // all inflow to gravity is routed to preferential
        dGravity_to_preferential = dGravity_inflow;
        // reset gravity inflow to 0.0
        dGravity_inflow = 0.0;
      }
      else
      {
        if (dStorage_available <= -tiny_value)
        {
          // impossible
        }
        else
        {
          // there is still space
          // dGravity_inflow is the soil_2_ssr or gvr_maxin in prms
          // check whether inflow exceeds or not
          dDifference_storage = dStorage_available - dGravity_inflow;
          if (abs(dDifference_storage) < tiny_value)
          {
            dGravity_inflow = dStorage_available;
            // storage reaches maximum
            dSoil_water_storage_gravity = dSoil_storage_gravity_max;
            // excess inflow goes to preferential
            dGravity_to_preferential = 0.0;
          }
          else
          {
            if (dDifference_storage <= -tiny_value)
            {
              // inflow indeed exceeds available
              // inflow equals to effective available space
              dGravity_inflow = dStorage_available;
              // storage reaches maximum
              dSoil_water_storage_gravity = dSoil_storage_gravity_max;
              // excess inflow goes to preferential
              dGravity_to_preferential = -dDifference_storage;
            }
            else
            {
              // inflow is too less
              // add inflow to gravity storage
              dSoil_water_storage_gravity = dSoil_water_storage_gravity + dGravity_inflow;
              // no excess flow to preferential
              dGravity_to_preferential = 0.0;
            }
          }
        }
      }

      // quality control
      if (dSoil_water_storage_gravity < near_zero)
      {
        dSoil_water_storage_gravity = 0.0;
      }
      if (dGravity_to_preferential < tiny_value)
      {
        dGravity_to_preferential = 0.0;
      }
      if (dGravity_inflow < tiny_value)
      {
        dGravity_inflow = 0.0;
      }
      return error_code;
    }
// 50==================================================
// step 8
/*!
  Because the gravity reservoir may recharge to the preferential reservois,
  the preferential reservoir storage needs to be re-calculated.
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_preferential_reservoir()
    {
      int error_code = 1;
      double dStorage_available = 0.0;
      double dDifference_flow = 0.0;
      // calcuate the available storage
      dStorage_available = dSoil_storage_preferential_max - dSoil_water_storage_preferential;
      // check available
      if (abs(dStorage_available) < tiny_value)
      {

        // no space left
        // add inflow from gravity to runoff from step 7
        dSoil_water_storage_preferential = dSoil_storage_preferential_max;
        dGravity_to_preferential = 0.0;
        dSoil_water_dunnian_runoff_gravity = dGravity_to_preferential;
      }
      else
      {
        if (dStorage_available <= -tiny_value)
        {
          // impossible
          dSoil_water_storage_preferential = dSoil_storage_preferential_max;
          dGravity_to_preferential = 0.0;
          dSoil_water_dunnian_runoff_gravity = dGravity_to_preferential;
        }
        else
        {
          // if there is still space
          dDifference_flow = dStorage_available - dGravity_to_preferential;
          // whether inflow exceeds available,dGravity_to_preferential is from
          // step 7
          if (abs(dDifference_flow) < tiny_value)
          {
            // inflow is too less
            dSoil_water_storage_preferential = dSoil_storage_preferential_max;
            // no excess runoff
            dSoil_water_dunnian_runoff_gravity = 0.0;
          }
          else
          {
            if (dDifference_flow <= -tiny_value)
            {
              // inflow exceed available
              dSoil_water_storage_preferential = dSoil_storage_preferential_max;
              dGravity_to_preferential = dStorage_available;
              dSoil_water_dunnian_runoff_gravity = -dDifference_flow;
            }
            else
            {
              // inflow is too less
              dSoil_water_storage_preferential =
                      dSoil_water_storage_preferential + dGravity_to_preferential;
              // no excess runoff
              dSoil_water_dunnian_runoff_gravity = 0.0;
            }
          }
        }
      }
      // quality control
      if (dSoil_water_storage_preferential < near_zero)
      {
        dSoil_water_storage_preferential = 0.0;
      }
      if (dGravity_to_preferential < tiny_value)
      {
        dGravity_to_preferential = 0.0;
      }
      if (dSoil_water_dunnian_runoff_gravity < near_zero)
      {
        dSoil_water_dunnian_runoff_gravity = 0.0;
      }
      return error_code;
    }
// 50==================================================
/*!//step 9
  Calculate the slow interflow in the gravity reservoir
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_slow_interflow()
    {
      int error_code = 1;
      dSoil_interflow_slow =
              dCoefficient_slow_linear * dSoil_water_storage_gravity +
                      dCoefficient_slow_square * (dSoil_water_storage_gravity * dSoil_water_storage_gravity);
      if (dSoil_interflow_slow < near_zero)
      {
        dSoil_interflow_slow = 0.0;
      }
      dSoil_water_storage_gravity = dSoil_water_storage_gravity - dSoil_interflow_slow
              - dSoil_gravity_drainage;
      if (dSoil_water_storage_gravity < tiny_value)
      {
        dSoil_water_storage_gravity = 0.0;
      }
      dSoil_interflow_slow_downslope = dSoil_interflow_slow;

      return error_code;
    }
// 50==================================================
/*!//step 10
  Calcualate the gravity drainage to groundwater reservoir
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_gravity_drainage()
    {
      int error_code = 1;
      dSoil_gravity_drainage = dRate_gravity_to_groundwater_base *
              pow(dSoil_water_storage_gravity, dGravity_to_groundwater_coef);
      if (dSoil_gravity_drainage < near_zero)
      {
        dSoil_gravity_drainage = 0.0;
      }

      return error_code;
    }
// 50==================================================
/*!//step 11
  Calculate the fast flow in the preferential reservoir
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_fast_interflow()
    {
      int error_code = 1;
      dSoil_interflow_fast = (dCoefficient_fast_linear * dSoil_water_storage_preferential) +
              dCoefficient_fast_square *
                      (dSoil_water_storage_preferential * dSoil_water_storage_preferential);
      if (dSoil_interflow_fast < near_zero)
      {
        dSoil_interflow_fast = 0.0;
      }
      dSoil_water_storage_preferential = dSoil_water_storage_preferential - dSoil_interflow_fast;
      if (dSoil_water_storage_preferential < tiny_value)
      {
        dSoil_water_storage_preferential = 0.0;
      }

      dSoil_interflow_fast_downslope = dSoil_interflow_fast;
      return error_code;
    }
// 50==================================================
/*!//step 12
  Summarize the dunnian flow from gravity and preferential reservoirs
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_dunnian_flow()
    {
      int error_code = 1;
      dSoil_water_outflow_dunnian_runoff = dSoil_water_dunnian_runoff_preferential
              + dSoil_water_dunnian_runoff_gravity;
      if (dSoil_water_outflow_dunnian_runoff < tiny_value)
      {
        dSoil_water_outflow_dunnian_runoff = 0.0;
      }

      // dDunnian_runoff_storage = 0.0;//care
      // dDunnian_runoff = dDunnian_runoff;
      return error_code;
    }
// 50==================================================
/*!//step 13
  Calculate interflow
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_interflow()
    {
      int error_code = 1;
      dSoil_water_interflow = dSoil_interflow_slow + dSoil_interflow_fast;
      if (dSoil_water_interflow < tiny_value)
      {
        dSoil_water_interflow = 0.0;
      }
      dSoil_interflow_inflow = dSoil_water_interflow + dSoil_water_interflow_inflow_external;

      dSoil_water_interflow_outflow = dSoil_interflow_inflow;

      dSoil_water_outflow_downslope = dSoil_interflow_inflow;

      return error_code;
    }

    int soil::calculate_soil_to_groundwater()
    {
      int error_code = 1;

      dSoil_water_outflow_groundwater = dSoil_moisture_to_groundwater + dSoil_gravity_drainage;
      if (dSoil_water_outflow_groundwater < near_zero)
      {
        dSoil_water_outflow_groundwater = 0.0;
      }

      return error_code;
    }
// 50==================================================
/*!step 14
  Evapotransportation
  cascade is handled by the master program since interactions cannot be directed
  within individual cell \param iFlag_transpiration \param iLand_cover_type
  \param dEt_available_hru_in
  \param dSca_in
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_evapotranspiration(int iFlag_transpiration,
                                                int iLand_cover_type,
                                                double dEt_available_hru_in,
                                                double dSca_in)
    {
      int error_code = 1;
      int iET_type;
      double dRatio_recharge_zone =
              dSoil_water_storage_recharge_zone / dSoil_storage_recharge_zone_max;
      double dRatio_lower_zone =
              dSoil_water_storage_moisture / dSoil_storage_moisture_max;
      double dET_recharge_potential = 0.0;
      double dET_lower_potential = 0.0;
      if (dEt_available_hru_in < near_zero)
      {
        dET_pervious = 0.0;
        dET_soil = dET_pervious;
        return error_code;
      }

      switch (iSoil_type)
      {
        case 1:
        {
          if (dRatio_recharge_zone < 0.25)
          {
            dET_recharge_potential =
                    0.5 * dRatio_recharge_zone * dEt_available_hru_in;
            dET_lower_potential =
                    0.5 * dRatio_lower_zone * dEt_available_hru_in;
          }
          else
          {
            dET_recharge_potential = dEt_available_hru_in;
            dET_lower_potential = dEt_available_hru_in;
          }
        }
          break;
        case 2:
        {
          if (dRatio_recharge_zone < 0.5)
          {
            dET_recharge_potential =
                    dRatio_recharge_zone * dEt_available_hru_in;
            dET_lower_potential = dRatio_lower_zone * dEt_available_hru_in;
          }
          else
          {
            dET_recharge_potential = dEt_available_hru_in;
            dET_lower_potential = dEt_available_hru_in;
          }
        }
          break;
        case 3:
        {
          if (dRatio_recharge_zone <= 0.33)
          {
            dET_recharge_potential =
                    0.5 * dRatio_recharge_zone * dEt_available_hru_in;
            dET_lower_potential =
                    0.5 * dRatio_lower_zone * dEt_available_hru_in;
          }
          else
          {
            if (dRatio_recharge_zone >= 0.66)
            {
              dET_recharge_potential = dEt_available_hru_in;
              dET_lower_potential = dEt_available_hru_in;
            }
            else
            {
              dET_recharge_potential =
                      dRatio_recharge_zone * dEt_available_hru_in;
              dET_lower_potential = dRatio_lower_zone * dEt_available_hru_in;
            }
          }
        }
          break;
        case 5:
        {
          if (dRatio_recharge_zone < 0.25)
          {
            dET_recharge_potential =
                    0.5 * dRatio_recharge_zone * dEt_available_hru_in;
            dET_lower_potential =
                    0.5 * dRatio_lower_zone * dEt_available_hru_in;
          }
          else
          {
            dET_recharge_potential = dEt_available_hru_in;
            dET_lower_potential = dEt_available_hru_in;
          }
        }
          break;
        case 13: // predominantly sand
        {
          if (dRatio_recharge_zone < 0.25)
          {
            dET_recharge_potential =
                    0.5 * dRatio_recharge_zone * dEt_available_hru_in;
            dET_lower_potential =
                    0.5 * dRatio_lower_zone * dEt_available_hru_in;
          }
          else
          {
            dET_recharge_potential = dEt_available_hru_in;
            dET_lower_potential = dEt_available_hru_in;
          }
        }
          break;
        default:
        {
          if (dRatio_recharge_zone < 0.25)
          {
            dET_recharge_potential =
                    0.5 * dRatio_recharge_zone * dEt_available_hru_in;
            dET_lower_potential =
                    0.5 * dRatio_lower_zone * dEt_available_hru_in;
          }
          else
          {
            dET_recharge_potential = dEt_available_hru_in;
            dET_lower_potential = dEt_available_hru_in;
          }
        }
          break;
      }
      // at this stage, potential may be even large than available

      if (iFlag_transpiration == 0) // transpiration is off
      {
        if (dSca_in > 0.99) // completely coved by snow. et_type = 1
        {
          iET_type = 1;
          dET_pervious = 0.0;
          dET_soil = dET_pervious;
          return error_code;
        }
        else // et type = 2 there is fractional snow
        {
          iET_type = 2;
          dET_recharge_potential = dET_recharge_potential * (1.0 - dSca_in);
        }
      }
      else // transpiration is on
      {
        if (iLand_cover_type > 1 && iLand_cover_type <= 5) // forest et type = 3
        {
          iET_type = 3;
        }
        else // not tree
        {
          if (dSca_in >= 0.99) // snow et type = 1
          {
            iET_type = 1;
            dET_pervious = 0.0;
            dET_soil = dET_pervious;
            return error_code;
          }
          else // et type = 2, there is fractional snow
          {
            iET_type = 2;
            dET_recharge_potential = dET_recharge_potential * (1.0 - dSca_in);
          }
        }
      }

      //
      if (dET_recharge_potential > dSoil_water_storage_recharge_zone)
      {
        dET_recharge_potential = dSoil_water_storage_recharge_zone;
        dSoil_water_storage_recharge_zone = 0.0;
      }
      else
      {
        dSoil_water_storage_recharge_zone = dSoil_water_storage_recharge_zone - dET_recharge_potential;
      }

      if (iET_type == 2 || dET_recharge_potential > dET_lower_potential)
      {
        if (dET_recharge_potential > dSoil_water_storage_moisture)
        {
          // is this even possible?
          dET_recharge_potential = dSoil_water_storage_moisture;
          dSoil_water_storage_moisture = 0.0;
        }
        else
        {
          dET_pervious = dET_recharge_potential;
          dSoil_water_storage_moisture =
                  dSoil_water_storage_moisture - dET_recharge_potential;
        }
        dET_pervious = dET_recharge_potential;
      }
      else
      {
        if (dET_lower_potential > dSoil_water_storage_moisture)
        {
          dET_pervious = dSoil_water_storage_moisture;
          dSoil_water_storage_moisture = 0.0;
        }
        else
        {
          dSoil_water_storage_moisture = dSoil_water_storage_moisture - dET_lower_potential;
          dET_pervious = dET_lower_potential;
        }
      }
      if (dET_pervious > dEt_available_hru_in)
      {
        dET_pervious = dEt_available_hru_in;
      }
      if (dSoil_water_storage_recharge_zone > dSoil_water_storage_moisture)
      {
        dSoil_water_storage_recharge_zone = dSoil_water_storage_moisture;
      }

      dET_soil = dET_pervious;

      return error_code;
    }

// 50==================================================
/*!
  calculate the soil moisture limitation scalar
  based on tem rhmoist: effect of moisture on decomposition
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_decomposition_moisture_scalar()
    {
      int error_code = 1;
      double dummy = (dSoil_moisture_volume - dMoisture_min) *
              (dSoil_moisture_volume - dMoisture_max);
      dScalar_soil_moisture_decomposition =
              dummy /
                      (dummy - pow((dSoil_moisture_volume - dMoisture_optimization), 2.0));
      if (dScalar_soil_moisture_decomposition < 0.01)
      {
        dScalar_soil_moisture_decomposition = 0.0;
      }
      return error_code;
    }

// 50==================================================
/*!
  calculate adsorption
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_dissolved_organic_carbon_adsorption()
    {
      int error_code = 1;

      double dVolume;
      double dDoc_adsorption_mass;

      dVolume = dSoil_thickness * dArea;

      // 50==================================================
      //
      // 50==================================================
      dDoc_adsorption_mass =
              dCoefficient_desorption * dSoil_density *
                      kilogram_per_cubic_meter_2_gram_per_cubic_centimeter * dKd // l per g
                      * dSoil_doc_concentration *
                      kilogram_per_cubic_meter_2_milligram_per_liter * dVolume *
                      cubic_meter_2_cubic_centimeter;
      dDoc_adsorption_mass = dDoc_adsorption_mass * milligram_2_kilogram;
      // 50==================================================

      dSoil_doc_adsorption = dDoc_adsorption_mass / dArea; // kg/m2
      if (dSoil_doc_adsorption < near_zero)
      {
        dSoil_doc_adsorption = 0.0;
      }

      return error_code;
    }

    int soil::calculate_soil_dissolved_organic_carbon_desorption()
    {
      int error_code = 1;

      double dVolume;
      double dDoc_desorption_mass;

      dVolume = dSoil_thickness * dArea;
      // 50==================================================
      // units: kg * kg(-1) - km (3) * kg(-1) * kg * m(-3) = unitless
      // 50==================================================
      dDoc_desorption_mass =
              dCoefficient_desorption // unitless
                      * dSoil_density * kilogram_per_cubic_meter_2_gram_per_cubic_centimeter
                      * dSoil_spsoc // unitless
                      * dVolume * cubic_meter_2_cubic_centimeter;
      dDoc_desorption_mass = dDoc_desorption_mass * gram_2_kilogram;

      dSoil_doc_desorption = dDoc_desorption_mass / dArea;
      if (dSoil_doc_desorption < tiny_value)
      {
        dSoil_doc_desorption = 0.0;
      }

      return error_code;
    }

    int soil::calculate_soil_dissolved_organic_carbon_production(
            double dSoil_temperature_in)
    {
      int error_code = 1;
      double dummy1, dummy2;

      double dDoc_production_mass;
      double dVolume;

      dVolume = dSoil_thickness * dArea;
      // 50==================================================
      // step 1 production
      // 50==================================================
      // units: mg/g //P
      dummy1 = cMicrobe.calculate_soil_dissolved_organic_carbon_production_rate(
              iFlag_anoxic, dSoil_temperature_in);

      // density to mass
      // units: gram
      dummy2 = dSoil_density *
              kilogram_per_cubic_meter_2_gram_per_cubic_centimeter * dVolume *
              cubic_meter_2_cubic_centimeter;

      // milligram
      dDoc_production_mass = dummy1 * dummy2; // Pp

      dDoc_production_mass =
              dDoc_production_mass * milligram_2_kilogram;     // kilogram
      dSoil_doc_production = dDoc_production_mass / dArea; // kg/m2
      if (dSoil_doc_production < tiny_value)
      {
        dSoil_doc_production = 0.0;
      }

      return error_code;
    }

    int soil::calculate_soil_dissolved_organic_carbon_mineralization(
            double dSoil_temperature_in)
    {
      int error_code = 1;
      // 50==================================================
      // step 2 mineralization
      // 50==================================================
      double dummy1, dummy2;
      double dVolume;

      double dDoc_mineralization_mass;

      if (dSoil_water_storage > tiny_value)
      {
        dVolume = dSoil_water_storage * dArea;
        dummy1 = cMicrobe.calculate_soil_dissolved_organic_carbon_mineralization_rate(
                iFlag_anoxic, dSoil_temperature_in);
        // concentration to mass
        // * unitless * kg * m(-3) * m3 = kg
        /* dummy2 = dSoil_moisture_volume * dSoil_doc_concentration *
                  kilogram_per_cubic_meter_2_milligram_per_liter * dVolume *
                  cubic_meter_2_cubic_liter;         // Q * C
      */
        dummy2 = dSoil_doc_concentration *
                kilogram_per_cubic_meter_2_milligram_per_liter * dVolume *
                cubic_meter_2_cubic_liter;

        dDoc_mineralization_mass = dummy1 * dummy2; // u * Q * C milligram
        if (dDoc_mineralization_mass < near_zero)
        {
          dDoc_mineralization_mass = 0.0;
        }
        dDoc_mineralization_mass = dDoc_mineralization_mass * milligram_2_kilogram;
        dSoil_doc_mineralization = dDoc_mineralization_mass / dArea;
        if (dSoil_doc_mineralization < near_zero)
        {
          dSoil_doc_mineralization = 0.0;
        }
      }
      else
      {
        dSoil_doc_mineralization = 0.0;

      }

      return error_code;
    }


// 50==================================================
/*!
  the mass balance based doc concentration
  \param dDoc_upslope_in
  \param dTemperature_soil_in
  \return <ReturnValue>
*/
// 50==================================================
    int soil::calculate_soil_dissolved_organic_carbon_concentration()
    {
      int error_code = 1;
      double dSoil_doc_concentration_maximal = 0.1;
      double dVolume;
      double dMass;


      //calculate the total doc storage


      dSoil_doc_inflow = dSoil_doc_inflow_upslope
              + dSoil_doc_inflow_external
              + dSoil_doc_inflow_groundwater;

      dSoil_doc_total = dSoil_doc_storage
              + dSoil_doc_inflow
              + dSoil_doc_production
              + dSoil_doc_desorption
              - dSoil_doc_mineralization
              - dSoil_doc_adsorption;

      if (dSoil_doc_total < tiny_value)
      {
        dSoil_doc_total = tiny_value;
        dSoil_doc_concentration = small_value;
      }
      else
      {
        dMass = dSoil_doc_total * dArea;

        dSoil_water_flow_total = dSoil_water_storage
                + dSoil_water_outflow_downslope
                + dSoil_water_outflow_groundwater
                + dSoil_water_outflow_dunnian_runoff;

        if (dSoil_water_flow_total < small_value)
        {
          dSoil_water_flow_total = small_value;
          dSoil_doc_concentration = small_value;
        }
        else
        {
          dVolume = dSoil_water_flow_total * dArea;

          //calculate the doc fluxes

          dSoil_doc_concentration = dMass / dVolume;

          if (dSoil_doc_concentration > dSoil_doc_concentration_maximal)
          {
            dSoil_doc_concentration = dSoil_doc_concentration_maximal;
          }
        }

      }

      //calculate the total doc volume
      return error_code;
    }

// 50==================================================
    int soil::calculate_soil_potential_dissolved_organic_carbon_mineralization(
            double dSoil_temperature_in)
    {
      int error_code = 1;
      double dummy0;
      double dummy1;
      double dummy2;
      double dVolume = dSoil_thickness * dArea;
      double dSpsoc_mineralization_mass;
      // 50==================================================
      // step 1, spsoc mineralization
      // 50==================================================
      // calculate u2, unitless
      dummy0 =
              cMicrobe
                      .calculate_soil_sorbed_potentially_soluble_organic_carbon_mineralization_rate();

      dummy1 = dSoil_density; //[iFlag_anoxic];//units: kg * m(-3)
      //
      dummy2 = dSoil_spsoc; // unitless
      dSpsoc_mineralization_mass =
              dummy0 // mu2  unitless
                      * dummy1 * kilogram_per_cubic_meter_2_gram_per_cubic_centimeter // p,
                      * dummy2 // unitless
                      * dVolume * cubic_meter_2_cubic_centimeter;

      dSpsoc_mineralization_mass = dSpsoc_mineralization_mass * gram_2_kilogram;

      dSoil_spsoc_mineralization = dSpsoc_mineralization_mass / dArea;

      if (dSoil_spsoc_mineralization < near_zero)
      {
        dSoil_spsoc_mineralization = 0.0;
      }
      return error_code;
    }

    int soil::calculate_soil_heterotrophic_respiration(double dSoil_temperature_in)
    {
      int error_code = 1;
      double dSoil_heterotrophic_respiration_rate;
      calculate_soil_decomposition_moisture_scalar();
      dSoil_heterotrophic_respiration_rate =
              cMicrobe.calculate_soil_heterotrophic_respiration_rate(
                      dSoil_clay_percentage, dSoil_silt_percentage, dSoil_temperature_in);
      dSoil_heterotrophic_respiration = dSoil_carbon *
              dScalar_soil_moisture_decomposition *
              dSoil_heterotrophic_respiration_rate / 30.0;

      if (dSoil_heterotrophic_respiration < near_zero)
      {
        dSoil_heterotrophic_respiration = 0.0;
      }
      return error_code;
    }

///
/// \param eCU_type
/// \return
    int soil::initialize_soil(eColumn_type eCU_type)
    {
      int error_code = 1;
      iSoil_type = iSoil_type_default;
      switch (eCU_type)
      {
        case eCU_glacier: // glacier
        {
          // there is no soil in glacier column
        }
          break;
        case eCU_lake: // lake
        {
          // soil is omitted here
        }
          break;
        case eCU_land: // land
        {
          if (iSoil_type < 1 || iSoil_type > nsoil_type)
          {

          }
          else // typical soil
          {
            dSoil_water_storage = 0.0;
            dSoil_water_storage_preferential = 0.0;
            dSoil_water_storage_gravity = 0.0;
            dSoil_water_storage_recharge_zone = 0.0;
            dSoil_water_storage_moisture = 0.0;

            dStorage_soil_moisture_lower = 0.0;

            //dFraction_preferential = 0.25;
            dPreferential_inflow = 0.0;

            dCapillary_inflow = 0.0;

            dSoil_moisture_volume = 0.0;
            dSoil_moisture_to_groundwater = 0.0;
            dGravity_inflow = 0.0;

            dSoil_water_interflow = 0.0;
            dSoil_water_interflow_outflow = 0.0;
            dSoil_water_interflow_storage = 0.0;

            dSoil_water_interflow_inflow_external = 0.0;

            dSoil_water_outflow_downslope = 0.0;
            dSoil_water_outflow_groundwater = 0.0;

            dGravity_to_preferential = 0.0;

            dSoil_interflow_slow = 0.0;
            dSoil_interflow_fast = 0.0;
            dSoil_gravity_drainage = 0.0;
            dET_pervious = 0.0;
            dThreshold_preferential = 0.0;
            dThreshold_saturation = 0.0;

            dSoil_water_dunnian_runoff_gravity = 0.0;

            dSoil_water_dunnian_runoff_preferential = 0.0;

            dSoil_interflow_fast_downslope = 0.0;
            dSoil_interflow_fast_upslope = 0.0;
            dSoil_interflow_slow_downslope = 0.0;
            dSoil_interflow_slow_upslope = 0.0;

            // 50==================================================
            // DOC
            // 50==================================================



            //dSoil_density = 100.0;
            dSoil_carbon = 0.0;
            dSoil_soc = 0.0;
            dSoil_doc_concentration = 0.0;
            dSoil_spsoc = 0.001;
            dSoil_spsoc_mass = 0.0;
            dSoil_doc_storage = 0.0;        //
            dSoil_doc_production = 0.0;     // kg
            dSoil_doc_mineralization = 0.0; // kg
            dSoil_doc_adsorption = 0.0;
            dSoil_doc_desorption = 0.0;
            dSoil_doc_inflow_upslope = 0.0;
            dSoil_doc_inflow_external = 0.0;
            dSoil_doc_inflow_groundwater = 0.0;

            dSoil_doc_outflow_downslope = 0.0;
            dSoil_spsoc_mineralization = 0.0; // kg
            dSoil_spsoc_adsorption = 0.0;
            dSoil_spsoc_desorption = 0.0;

            dSoil_heterotrophic_respiration = 0.0;

          }
        }
          break;
        case eCU_stream: // stream
          break;
        case eCU_swale: // swale
          break;
        default:
          break;
      }

      cMicrobe.initialize_microbe();

      return error_code;
    }

// 50==================================================
/*!
  Run soil module
  \param iFlag_transpiration: the flag of transpiration
  \param iLand_cover_type: land cover tpye
  \param dEt_available_hru_in: avaiable ET
  \param dFraction_pervious_in: the fraction of pervious area
  \param dInfiltration_hru_in: infiltration from surface runoff module, unit:
  meter \param dSca_in: snow cover area, unit: fraction \param
  dTemperature_soil_in: the soil temperature, unit: kelvin \return <ReturnValue>
*/
// 50==================================================
    int soil::run_soil_model(int iFlag_transpiration, int iLand_cover_type,
                             double dDoc_leaching_in, double dEt_available_hru_in,
                             double dFraction_pervious_in,
                             double dInfiltration_hru_in, double dSca_in,
                             double dTemperature_soil_in)
    {
      int error_code = 1;
      // 50==================================================
      // run soil water model first
      // 50==================================================
      double dTemperature_soil_celsius = dTemperature_soil_in + kelvin_2_celsius;
      double dummy0, dummy1;
      if (dTemperature_soil_celsius < near_zero)
      {
        dCoefficient_fast_linear = dCoefficient_fast_linear_base;

        dCoefficient_fast_square = dCoefficient_fast_square_base;

        dCoefficient_slow_linear = dCoefficient_slow_linear_base;

        dCoefficient_slow_square = dCoefficient_slow_square_base;

        dRate_gravity_to_groundwater = dRate_gravity_to_groundwater_base;

        // dCoefficient_fast_linear = dCoefficient_fast_linear_base *
        //                           pow(3, (dTemperature_soil_celsius) / 10);

        // dCoefficient_fast_square = dCoefficient_fast_square_base *
        //                           pow(3, (dTemperature_soil_celsius) / 10);

        // dCoefficient_slow_linear = dCoefficient_slow_linear_base *
        //                           pow(2, (dTemperature_soil_celsius) / 10);

        // dCoefficient_slow_square = dCoefficient_slow_square_base *
        //                           pow(2, (dTemperature_soil_celsius) / 10);

        // dRate_gravity_to_groundwater = dRate_gravity_to_groundwater_base *
        //                               pow(2, (dTemperature_soil_celsius) / 10);
      }
      else
      {
        dCoefficient_fast_linear = dCoefficient_fast_linear_base;

        dCoefficient_fast_square = dCoefficient_fast_square_base;

        dCoefficient_slow_linear = dCoefficient_slow_linear_base;

        dCoefficient_slow_square = dCoefficient_slow_square_base;

        dRate_gravity_to_groundwater = dRate_gravity_to_groundwater_base;
      }

      run_soil_water_model(iFlag_transpiration, iLand_cover_type,
                           dEt_available_hru_in, dFraction_pervious_in,
                           dInfiltration_hru_in, dSca_in);

      // anoxic
      dummy0 = dThreshold_preferential - dThreshold_wilting_point;
      dummy1 = dSoil_water_storage_gravity - dummy0;
      if (abs(dummy1) < tiny_value)
      {
        // just close
        iFlag_anoxic = 1;
      }
      else
      {
        if (dummy1 < -tiny_value)
        {
          // less
          iFlag_anoxic = 0;
        }
        else
        {
          // more than
          iFlag_anoxic = 1;
        }
      }
      dSoil_water_storage =
              dSoil_water_storage_moisture
                      + dSoil_water_storage_gravity
                      + dSoil_water_storage_preferential;

      run_soil_carbon_model(dDoc_leaching_in, dTemperature_soil_in);
      return error_code;
    }
// 50==================================================
/*!
  sub head quarter of soil water model<long-description>
  \param iFlag_transpiration
  \param iLand_cover_type
  \param dEt_available_hru_in
  \param dFraction_pervious_in
  \param dInfiltration_hru_in
  \param dSca_in
  \return <ReturnValue>
*/
// 50==================================================
    int soil::run_soil_water_model(int iFlag_transpiration, int iLand_cover_type,
                                   double dEt_available_hru_in,
                                   double dFraction_pervious_in,
                                   double dInfiltration_hru_in, double dSca_in)
    {
      int error_code = 1;

      this->dFraction_pervious = dFraction_pervious_in;

      calculate_soil_preferential_inflow(dInfiltration_hru_in,
                                         dFraction_preferential);
      calculate_preferential_storage();
      calculate_capillary_inflow(dInfiltration_hru_in);
      calculate_soil_recharge_zone();
      calculate_soil_moisture();
      calculate_gravity_reservoir();
      calculate_preferential_reservoir();
      // step 9
      calculate_slow_interflow();
      // step 10
      calculate_gravity_drainage();
      // step 11
      calculate_fast_interflow();
      // step 12
      calculate_dunnian_flow();
      calculate_interflow();

      calculate_soil_to_groundwater();

      calculate_soil_evapotranspiration(iFlag_transpiration, iLand_cover_type,
                                        dEt_available_hru_in, dSca_in);

      dSoil_moisture_volume = dSoil_water_storage_moisture / dSoil_storage_moisture_max;//dSoil_thickness; //
      if (dSoil_moisture_volume < small_value)
      {
        dSoil_moisture_volume = small_value;
      }

      return error_code;
    }
// 50==================================================
/*!
  Soil thermal model
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::run_soil_thermal_model()
    {
      int error_code = 1;
      return error_code;
    }


// 50==================================================
/*!
  calculate the doc aborption and desorption
  be careful with the sign and units
  \param
  \return <ReturnValue>
*/
// 50==================================================
    int soil::run_soil_carbon_model(double dDoc_leaching_in,
                                    double dSoil_temperature_in)
    {
      int error_code = 1;
      dSoil_doc_inflow_external = dDoc_leaching_in;
      calculate_soil_heterotrophic_respiration(dSoil_temperature_in);

      // 50==================================================
      // run soil DOC/DIC model
      // 50==================================================
      dSoil_soc = dSoil_carbon * 0.3;
      // 50==================================================

      run_soil_dissolved_carbon_model(dSoil_temperature_in);

      return error_code;
    }

    int soil::run_soil_dissolved_carbon_model(
            double dSoil_temperature_in)
    {
      int error_code = 1;

      calculate_soil_dissolved_organic_carbon_adsorption();
      calculate_soil_dissolved_organic_carbon_desorption();
      calculate_soil_dissolved_organic_carbon_production(dSoil_temperature_in);
      calculate_soil_dissolved_organic_carbon_mineralization(dSoil_temperature_in);

      calculate_soil_potential_dissolved_organic_carbon_mineralization(
              dSoil_temperature_in);

      dSoil_spsoc_adsorption = dSoil_doc_adsorption;

      dSoil_spsoc_desorption = dSoil_doc_desorption;

      calculate_soil_dissolved_organic_carbon_concentration();


      //get new fluxes
      dSoil_doc_storage = dSoil_doc_concentration * dSoil_water_storage;

      dSoil_doc_outflow_groundwater = dSoil_doc_concentration * dSoil_water_outflow_groundwater;


      dSoil_doc_outflow_dunnian_runoff = dSoil_doc_concentration * dSoil_water_outflow_dunnian_runoff;
      dSoil_doc_outflow_downslope = dSoil_doc_concentration * dSoil_water_outflow_downslope;


      if (dSoil_doc_storage < tiny_value)
      {
        dSoil_doc_storage = 0.0;
      }

      return error_code;
    }
// 50==================================================
/*!
  initialize soil proprities based on soil type
  \param iSoil_type
  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
    int soil::update_soil_parameter(int iFlag_run_carbon_cascade,
                                    int iFlag_run_water_cascade,
                                    int iSoil_type,
                                    int iVegetation_type)
    {
      int error_code = 1;
      this->iFlag_run_water_cascade = iFlag_run_water_cascade;
      this->iFlag_run_carbon_cascade = iFlag_run_carbon_cascade;

      this->iSoil_type = iSoil_type;
      // check the soil type
      if (iSoil_type < 1 || iSoil_type > nsoil_type)
      {
      }
      else
      {

        // 50==================================================
        // The saturation threshold,
        // the average thickness of active layer depth in Tanana Soil is
        // around 1.5m therefore, the ALT can be used to set this parameter
        // 50==================================================
        dSoil_thickness = aSoil_thicknesses[iSoil_type - 1];
        dSoil_density = aSoil_density[iSoil_type - 1];
        dFraction_preferential = aFraction_preferential[iSoil_type - 1];
        dFraction_field_capacity = aFraction_field_capacity[iSoil_type - 1];
        dFraction_wilting_point = aFraction_wilting_point[iSoil_type - 1];

        switch (iSoil_type)
        {
          case 1:
            dSoil_clay_percentage = 0.33;
            dSoil_sand_percentage = 0.33;
            dSoil_silt_percentage = 0.33;
            break;
          case 2:
            dSoil_clay_percentage = 0.33;
            dSoil_sand_percentage = 0.33;
            dSoil_silt_percentage = 0.33;
            break;
          case 4:
            dSoil_clay_percentage = 0.24;
            dSoil_sand_percentage = 0.61;
            dSoil_silt_percentage = 0.15;
            break;
          case 5:
            dSoil_clay_percentage = 0.24;
            dSoil_sand_percentage = 0.61;
            dSoil_silt_percentage = 0.15;
            break;
          case 11:
            dSoil_clay_percentage = 0.33;
            dSoil_sand_percentage = 0.33;
            dSoil_silt_percentage = 0.33;
            break;
          case 13:
            dSoil_clay_percentage = 0.05;
            dSoil_sand_percentage = 0.90;
            dSoil_silt_percentage = 0.05;
            break;
          default:
            dSoil_clay_percentage = 0.33;
            dSoil_sand_percentage = 0.33;
            dSoil_silt_percentage = 0.33;
            break;
        }
        dSoil_porosity = dSoil_clay_percentage * 0.45 +
                dSoil_sand_percentage * 0.31 +
                dSoil_silt_percentage * 0.47;
        // 50==================================================
        dThreshold_saturation = dSoil_thickness * dSoil_porosity;
        dThreshold_preferential =
                dThreshold_saturation * (1.0 - dFraction_preferential);
        dThreshold_field_capacity = dThreshold_saturation * dFraction_field_capacity;
        dThreshold_wilting_point = dThreshold_saturation * dFraction_wilting_point;

        dSoil_storage_preferential_max =
                dThreshold_saturation - dThreshold_preferential;
        dSoil_storage_moisture_max =
                dThreshold_field_capacity - dThreshold_wilting_point;
        dSoil_storage_recharge_zone_max = dSoil_storage_moisture_max * 0.7;

        dSoil_storage_gravity_max =
                dThreshold_preferential - dThreshold_field_capacity;
        // 50==================================================
        // soil water
        // 50==================================================

        // 50==================================================
        dSoil_moisture_to_groundwater_max = dThreshold_saturation * 0.01;
        // 50==================================================
        // respiration
        dMoisture_max = aMoisture_max[iSoil_type - 1];
        dMoisture_min = aMoisture_min[iSoil_type - 1];
        dMoisture_optimization = aMoisture_optimization[iSoil_type - 1];
        // 50==================================================
        // soil thermal
        // 50==================================================
        for (int i = 0; i < nsoil_layer; i++)
        {
          aSoil_thickness[i] = dSoil_thickness / nsoil_layer;
        }

        dCoefficient_fast_linear_base =
                aCoefficient_fast_linear_base[iSoil_type - 1];
        dCoefficient_fast_square_base =
                aCoefficient_fast_square_base[iSoil_type - 1];
        dCoefficient_slow_linear_base =
                aCoefficient_slow_linear_base[iSoil_type - 1];
        dCoefficient_slow_square_base =
                aCoefficient_slow_square_base[iSoil_type - 1];
        dRate_gravity_to_groundwater_base =
                aRate_gravity_to_groundwater_base[iSoil_type - 1];

        dKd = aKd[iSoil_type - 1];

        dCoefficient_desorption = aCoefficient_desorption[iSoil_type - 1];//0.078 * 1.0E-1;
        // 50==================================================
        // soil carbon
        // 50==================================================

        // 50==================================================
        // initialize the microbe here
        // 50==================================================
        cMicrobe.update_microbe_parameter(iSoil_type, iVegetation_type);
      }
      return error_code;
    }
// 50==================================================
/*!
  update the major soil system variables
  \param dLitter_carbon_decomposition_in kg/m-2
  \return <ReturnValue>
*/
// 50==================================================
    int soil::update_soil_status(double dLitter_carbon_decomposition_in
    )
    {
      int error_code = 1;

      update_soil_water_status();
      update_soil_doc_status();
      update_soil_spsoc_status();

      dSoil_carbon = dSoil_carbon + dLitter_carbon_decomposition_in
              - dSoil_heterotrophic_respiration
              + dSoil_doc_inflow_upslope
              - dSoil_doc_outflow_downslope
              + dSoil_doc_inflow_external
              - dSoil_doc_outflow_dunnian_runoff
              - dSoil_doc_outflow_groundwater;

      if (dSoil_carbon < tiny_value)
      {
        dSoil_carbon = 0.0;
      }
      return error_code;
    }

    int soil::update_soil_doc_status()
    {
      int error_code = 1;

      return error_code;
    }

    int soil::update_soil_spsoc_status()
    {
      // 50==================================================
      int error_code = 1;
      double dVolume = dSoil_thickness * dArea;
      dSoil_spsoc_mass =
              dSoil_spsoc * (dSoil_density * dVolume) / dArea; // unit area kg/m2

      dSoil_spsoc_mass = dSoil_spsoc_mass
              - dSoil_spsoc_mineralization
              + dSoil_spsoc_adsorption
              - dSoil_spsoc_desorption;

      if (dSoil_spsoc_mass < tiny_value)
      {
        dSoil_spsoc_mass = 0.0;
      }

      dSoil_spsoc = (dSoil_spsoc_mass * dArea) / (dSoil_density * dVolume);
      if (dSoil_spsoc > 0.99)
      {
        dSoil_spsoc = 0.99;
        std::cout << " dSoil_spsoc error" << std::endl;
        error_code = 0;
      }
      return error_code;
    }

    int soil::update_soil_water_status()
    {
      int error_code = 1;

      dSoil_water_storage =
              dSoil_water_storage_moisture
                      + dSoil_water_storage_gravity
                      + dSoil_water_storage_preferential;

      dSoil_interflow_inflow_upslope =
              dSoil_interflow_fast_upslope
                      + dSoil_interflow_slow_upslope;

      return error_code;
    }

// 50==================================================
/*!
  calculate the moisture scalar on nitrogen uptake by root or microbe
  \param
  \return <ReturnValue>
*/
// 50==================================================
// int soil::calculate_moisture_scalar_nitrogen_uptake()
//{
//  int error_code = 1;
//  double dScalar_soil_moisture_nitrogen_uptake =
//  pow(dSoil_moisture_volume, 3.0); if (dScalar_soil_moisture_nitrogen_uptake <
//  0.00001)
//  {
//    dScalar_soil_moisture_nitrogen_uptake = 0.0;
//  }
//  return error_code;
//}

// 50==================================================
/*!
  <long-description>
  \param iSoil_type
  \return <ReturnValue>
*/
// 50==================================================
// int soil::initialize_thermal_model(int iSoil_type)
// {
//   int error_code = 1;
//   return error_code;
// }
// 50==================================================
/*!
  the soil water model in iTEM<long-description>
  \param dEt_surface_in
  \param dEt_vegetation_in
  \param dTimestep_eco3d_in
  \return <ReturnValue>
*/
// 50==================================================
//  int soil::calculate_soil_water_layer(double dEt_surface_in,
//                                       double dEt_vegetation_in,
//                                       double dTimestep_eco3d_in)
//  {
//    // use tridiagonal system of equations to solve one-dimensional water
//    balance:
//    // d wat
//    // dz ----- = -qi + qo - s
//    // dt
//    // with q = -k d(psi+z)/dz = -k (d psi/dz + 1) and with s=0
//    // this is the Richards equation for vertical water flow
//    // d wat d d wat d psi
//    // ----- = -- [ k(----- ----- + 1) ]
//    // dt dz dz d wat
//    // where: wat = volume of water per volume of soil (mm**3/mm**3)
//    // psi = soil matrix potential (mm)
//    // dt = time step (s)
//    // z = depth (mm)
//    // dz = thickness (mm)
//    // qi = inflow at top (mm h2o /s) (+ = up, - = down)
//    // qo = outflow at bottom (mm h2o /s) (+ = up, - = down)
//    // s = source/sink flux (mm h2o /s) (+ = loss, - = gain)
//    // k = hydraulic conductivity (mm h2o /s)
//    // solution: linearize k and psi about d wat and use tridiagonal system
//    // of equations to solve for d wat, where for layer i
//    // r_i = a_i [d wat_i-1] + b_i [d wat_i] + c_i [d wat_i+1]
//    // the solution conserves water as:
//    // [h2osoi(1)*aSoil_thickness(1)*1000 + ... +
//    h2osoi(nsoillayer)*aSoil_thickness(nsoillayer)*1000] n+1 =
//    // [h2osoi(1)*aSoil_thickness(1)*1000 + ... +
//    h2osoi(nsoillayer)*aSoil_thickness(nsoillayer)*1000] n +
//    // (qinfl - qseva - qtran - qdrai)*dttem
//    // code only executed for soils (ist = 1)
//    // --------------------------------------------------------------------
//    int error_code = 1;
//    int j;
//    int iter;
//    vector<double> r(nsoil_layer);        // solution matrix
//    vector<double> a(nsoil_layer);        // "a" vector for tridiagonal matrix
//    vector<double> b(nsoil_layer);        // "b" vector for tridiagonal matrix
//    vector<double> c(nsoil_layer);        // "c" vector for tridiagonal matrix
//    vector<double> dwat(nsoil_layer);   // change in soil water
//    vector<double> smp(nsoil_layer);    // soil matrix potential (mm)
//    vector<double> hk(nsoil_layer);        // hydraulic conductivity (mm
//    h2o/s) vector<double> hk2(nsoil_layer);    // hk**2 vector<double>
//    dsmpdw(nsoil_layer); // d(smp)/d(h2osoi) vector<double>
//    dhkdw(nsoil_layer);  // d(hk)/d(h2osoi) double s; // h2osoi/watsat double
//    hydcon;                        // hydraulic conductivity (mm h2o/s) double
//    qin;                            // flux of water into soil layer (mm
//    h2o/s) double qout;                        // flux of water out of soil
//    layer (mm h2o/s) double num;                            // used in
//    calculating qi, qout double den;                            // used in
//    calculating qi, qout double den2;                        // den**2 used in
//    calculating qi, qout double dqidw1;                        //
//    d(qin)/d(h2osoi(i-1)) double dqidw2;                        //
//    d(qin)/d(h2osoi(i)) double dqodw1;                        //
//    d(qout)/d(h2osoi(i)) double dqodw2;                        //
//    d(qout)/d(h2osoi(i+1)) double xs;                            // soil water
//    > sat or < some minimum (mm h2o) double axs;                            //
//    amount of xs added to a soil layer double x;                            //
//    temporary value of axs double newwat;                        // temporary
//    value of updated h2osoi double watmin;                        // limit
//    h2osoi >= watmin dRunoff_subsurface = 0; for (iter = 0; iter < (int)
//    (dTimestep_eco3d_in / dTimestep_soil); iter++)
//    {
//      // initialize xs
//      xs = 0;
//      // evaluate hydraulic conductivity, soil matrix potential,
//      // d(smp)/d(h2osoi), and d(hk)/d(h2osoi). limit s>=0.05
//      // when evaluating these terms. this helps prevent numerical
//      // problems for very small h2osoi. also limit hk >= some very
//      // small number for same reason
//      for (j = 0; j < nsoil_layer; j++)
//      {
//        s = max(aWater_content_volumetric[j] / dWater_content_saturated,
//        0.05); smp[j] = dSoil_matrix_saturated * pow(s, -bch); dsmpdw[j] =
//        -bch * smp[j] / aWater_content_volumetric[j]; hydcon =
//        dHydraulic_conductivity_saturated * pow(s, 2 * bch + 3); hk[j] =
//        max(hydcon, 1e-10); dhkdw[j] = hydcon * (2 * bch + 3) /
//        aWater_content_volumetric[j]; hk2[j] = hk[j] * hk[j];
//      }
//      // set up r, a, b, and c vectors for tridiagonal solution
//      // node j = 0
//      j = 0;
//      num = -2 * (smp[j] - smp[j + 1]) - 1000 * (aSoil_thickness[j] +
//      aSoil_thickness[j + 1]); den = 1000 * (aSoil_thickness[j] / hk[j] +
//      aSoil_thickness[j + 1] / hk[j + 1]); den2 = den * den; qout = num / den;
//      dqodw1 = (-2 * den * dsmpdw[j] + num * 1000 * aSoil_thickness[j] /
//      hk2[j] * dhkdw[j]) / den2; dqodw2 = (2 * den * dsmpdw[j + 1]
// + num * 1000 * aSoil_thickness[j + 1] / hk2[j + 1] * dhkdw[j + 1]) / den2;
//      r[j] = (dEt_surface_in + dEt_vegetation_in * dFraction_root[j]) -
//      dInfiltration - qout; a[j] = 0; b[j] = dqodw1 - 1000 *
//      aSoil_thickness[j] / dTimestep_soil; c[j] = dqodw2;
//      //debugtest
//      if (b[j] != b[j] || r[j] != r[j] || a[j] != a[j] || c[j] != c[j])
//      {
//        //double aaa = 1;
//      }
//      // node j = nsoillayer - 1
//      j = nsoil_layer - 1;
//      num = -2 * (smp[j - 1] - smp[j]) - 1000 * (aSoil_thickness[j - 1] +
//      aSoil_thickness[j]); den = 1000 * (aSoil_thickness[j - 1] / hk[j - 1] +
//      aSoil_thickness[j] / hk[j]); den2 = den * den; qin = num / den; dqidw1 =
//      (-2 * den * dsmpdw[j - 1]
// + num * 1000 * aSoil_thickness[j - 1] / hk2[j - 1] * dhkdw[j - 1]) / den2;
//      dqidw2 = (2 * den * dsmpdw[j] + num * 1000 * aSoil_thickness[j] / hk2[j]
//      * dhkdw[j]) / den2; qout = -hk[j]; dqodw1 = -dhkdw[j]; r[j] =
//      dEt_vegetation_in * dFraction_root[j] + qin - qout; a[j] = -dqidw1; b[j]
//      = dqodw1 - dqidw2 - 1000 * aSoil_thickness[j] / dTimestep_soil; c[j] =
//      0; for (j = 1; j < nsoil_layer - 1; j++)
//      {
//        num = -2 * (smp[j - 1] - smp[j]) - 1000 * (aSoil_thickness[j - 1] +
//        aSoil_thickness[j]); den = 1000 * (aSoil_thickness[j - 1] / hk[j - 1]
//        + aSoil_thickness[j] / hk[j]); den2 = den * den; qin = num / den;
//        dqidw1 = (-2 * den * dsmpdw[j - 1]
// + num * 1000 * aSoil_thickness[j - 1] / hk2[j - 1] * dhkdw[j - 1]) / den2;
//        dqidw2 = (2 * den * dsmpdw[j] + num * 1000 * aSoil_thickness[j] /
//        hk2[j] * dhkdw[j]) / den2; num = -2 * (smp[j] - smp[j + 1]) - 1000 *
//        (aSoil_thickness[j] + aSoil_thickness[j + 1]); den = 1000 *
//        (aSoil_thickness[j] / hk[j] + aSoil_thickness[j + 1] / hk[j + 1]);
//        den2 = den * den;
//        qout = num / den;
//        dqodw1 = (-2 * den * dsmpdw[j]
// + num * 1000 * aSoil_thickness[j] / hk2[j] * dhkdw[j]) / den2;
//        dqodw2 = (2 * den * dsmpdw[j + 1]
// + num * 1000 * aSoil_thickness[j + 1] / hk2[j + 1] * dhkdw[j + 1]) / den2;
//        r[j] = dEt_vegetation_in * dFraction_root[j] + qin - qout;
//        a[j] = -dqidw1;
//        b[j] = dqodw1 - dqidw2 - 1000 * aSoil_thickness[j] / dTimestep_soil;
//        c[j] = dqodw2;
//      }
//      // solve for dwat: a, b, c, r, dwat
//      tridia(nsoil_layer, a, b, c, r, dwat);
//      // could now update h2osoi = h2osoi + dwat except for one problem:
//      // need to make sure h2osoi <= watsat. if not, what to do with
//      // excess water? add total excess water back to soil profile to
//      // bring any layers less than watsat up to watsat. any remaining
//      // excess water is sub-surface runoff.
//      // update water, constraining h2osoi <= watsat. accumulate excess water
//      for (j = 0; j < nsoil_layer; j++)
//      {
//        newwat = aWater_content_volumetric[j] + dwat[j];
//        double dummy = max(newwat - dWater_content_saturated, 0.0);
//        xs = xs + dummy * 1000 * aSoil_thickness[j];
//        aWater_content_volumetric[j] = min(dWater_content_saturated, newwat);
//      }
//      // add excess water back to bring soil layers up to saturation
//      for (j = 0; j < nsoil_layer; j++)
//      {
//        x = min((dWater_content_saturated - aWater_content_volumetric[j]) *
//        1000 * aSoil_thickness[j], xs); axs = x; xs = xs - axs;
//        aWater_content_volumetric[j] = aWater_content_volumetric[j] + axs /
//        (1000 * aSoil_thickness[j]);
//      }
//      // sub-surface drainage (accumulate over dttem/dtsoi iterations)
//      dRunoff_subsurface = dRunoff_subsurface + xs + (hk[nsoil_layer - 1]
// + dhkdw[nsoil_layer - 1] * dwat[nsoil_layer - 1]) * dTimestep_soil;
//    }
//    // sub-surface drainage over time step = dttem
//    dRunoff_subsurface = dRunoff_subsurface / dTimestep_eco3d_in;
//    // limit h2osoi >= watmin. get water needed to bring h2osoi = watmin
//    // from lower layer. do for all points so inner loop vectorizes
//    watmin = 0.01;
//    for (j = 0; j < nsoil_layer - 1; j++)
//    {
//      if (aWater_content_volumetric[j] < watmin)
//      {
//        xs = (watmin - aWater_content_volumetric[j]) * 1000 *
//        aSoil_thickness[j];
//      }
//      else
//      {
//        xs = 0;
//      }
//      aWater_content_volumetric[j] = aWater_content_volumetric[j] + xs / (1000
//      * aSoil_thickness[j]); aWater_content_volumetric[j + 1] =
//      aWater_content_volumetric[j + 1]
// - xs / (1000 * aSoil_thickness[j + 1]);
//      //debugtest
//      if (aWater_content_volumetric[j] != aWater_content_volumetric[j])
//      {
//        //double aaa = 1;
//      }
//    }
//    j = nsoil_layer - 1;
//    if (aWater_content_volumetric[j] < watmin)
//    {
//      xs = (watmin - aWater_content_volumetric[j]) * 1000 *
//      aSoil_thickness[j];
//    }
//    else
//    {
//      xs = 0;
//    }
//    aWater_content_volumetric[j] = aWater_content_volumetric[j] + xs / (1000 *
//    aSoil_thickness[j]);
//    //debugtest
//    if (aWater_content_volumetric[j] != aWater_content_volumetric[j])
//    {
//      //double aaa = 1;
//    }
//    dRunoff_subsurface = dRunoff_subsurface - xs / dTimestep_eco3d_in;
//    a.clear();
//    b.clear();
//    c.clear();
//    r.clear();
//    dwat.clear();
//    smp.clear();
//    hk.clear();
//    hk2.clear();
//    dsmpdw.clear();
//    dhkdw.clear();
//    return error_code;
//  }
//
//  //50==================================================
//  /*!
//    <long-description>
//    \param
//    \return <ReturnValue>
//  */
//  //50==================================================
//  int soil::calculate_thermal_proprieties()
//  {
//    int error_code = 1;
//    for (int j = 0; j < nsoil_layer; j++)
//    {
//      dLatent_heat_volumetric = aWater_content_volumetric[j] *
//      dLatent_heat_water * dDensity_water; dThermal_conductivity_unfrozen =
//      (pow(dThermal_conductivity_solid, 1 - dWater_content_saturated)
// * pow(tkwat,
//
//     aWater_content_volumetric[j]) - dThermal_conductivity_dry) *
//     (aWater_content_volumetric[j]
// / dWater_content_saturated) + dThermal_conductivity_dry;
//      dThermal_conductivity_frozen = (pow(dThermal_conductivity_solid, 1 -
//      dWater_content_saturated)
// * pow(tkice,
//
// aWater_content_volumetric[j]) - dThermal_conductivity_dry)
// * (aWater_content_volumetric[j] / dWater_content_saturated) +
// dThermal_conductivity_dry;
//      dSpecific_heat_volumetric_unfrozen = (1 - dWater_content_saturated) *
//      dSpecific_heat_capacity_solid
// + cwat * aWater_content_volumetric[j];
//      dSpecific_heat_volumetric_frozen = (1 - dWater_content_saturated) *
//      dSpecific_heat_capacity_solid
// + cice * aWater_content_volumetric[j];
//      if (aTemperature_soil[j] > dFrozen_temperature + dFrozen_range)
//      {
//        aThermal_conductivity[j] = dThermal_conductivity_unfrozen;
//        aSpecific_heat_volumetric[j] = dSpecific_heat_volumetric_unfrozen;
//      }
//      if (aTemperature_soil[j] >= dFrozen_temperature
// - dFrozen_range && aTemperature_soil[j] <= dFrozen_temperature +
// dFrozen_range)
//      {
//        aThermal_conductivity[j] = dThermal_conductivity_frozen +
//        (dThermal_conductivity_unfrozen
// - dThermal_conductivity_frozen) * (aTemperature_soil[j] - dFrozen_temperature
// + dFrozen_range) / (2 * dFrozen_range);
//        aSpecific_heat_volumetric[j] = (dSpecific_heat_volumetric_frozen +
//        dSpecific_heat_volumetric_unfrozen)
// / 2.0 + dLatent_heat_volumetric / (2 * dFrozen_range);
//      }
//      if (aTemperature_soil[j] < dFrozen_temperature - dFrozen_range)
//      {
//        aThermal_conductivity[j] = dThermal_conductivity_frozen;
//        aSpecific_heat_volumetric[j] = dSpecific_heat_volumetric_frozen;
//      }
//    }
//    return error_code;
//  }
//  //50==================================================
//  /*!
//    <long-description>
//    \param kelvin
//  */
//  //50==================================================
//  int soil::calculate_soil_thermal(double dHeat_flux_in,
//                                   double dThickness_surface_in)
//  {
//    // calculate soil temperatures from one-dimensional thermal diffusion
//    // equation using apparent heat capacity to account for phase change
//    // d d ts d ts
//    // -- (k ----) = ca ----
//    // dz dz dt
//    // where: k = thermal conductivity (w/m/kelvin)
//    // ca = apparent specific heat capacity (j/m**3/kelvin)
//    // ts = temperature (kelvin)
//    // z = depth (m)
//    // t = time (s)
//    // use crank-nicholson method to set up tridiagonal system of equations to
//    // solve for ts at time n+1, where the temperature equation for layer i is
//    // r_i = a_i [ts_i-1] n+1 + b_i [ts_i] n+1 + c_i [ts_i+1] n+1
//    // the solution conserves energy as
//    // cv( 1)*([ts( 1)] n+1 - [ts( 1)] n)*dz( 1)/dt + ... +
//    // cv(nsoillayer)*([ts(nsoillayer)] n+1 - [ts(nsoillayer)]
//    n)*dz(nsoillayer)/dt = fgr
//    // where
//    // cv = heat capacity (j/m**3/kelvin)
//    // dz = thickness (m)
//    // dt = time step (s)
//    // [ts] n = old temperature (kelvin)
//    // [ts] n+1 = new temperature (kelvin)
//    // fgr = heat flux into the soil (w/m**2)
//    int error_code = 1;
//    int j;                            // do loop or array index
//    vector<double> r(nsoil_layer);  // solution matrix
//    vector<double> a(nsoil_layer);  // "a" vector for tridiagonal matrix
//    vector<double> b(nsoil_layer);  // "b" vector for tridiagonal matrix
//    vector<double> c(nsoil_layer);  // "c" vector for tridiagonal matrix
//    vector<double> u(nsoil_layer);  // solution vector from tridiagonal
//    solution vector<double> dz(nsoil_layer); // dzsoi adjusted for snow double
//    ocvts;                    // sum (cv*(tsoi[n ])*dzsoi
//    // double ncvts; // sum (cv*(tsoi[n+1])*dzsoi
//    double m1; // intermediate variable for calculating r, a, b, c
//    double m2; // intermediate variable for calculating r, a, b, c
//    double m3; // intermediate variable for calculating r, a, b, c
//    // need to use blended snow/soil for the first layer
//    //dz[0] = ground.dzsur;
//    dz[0] = dThickness_surface_in;
//    for (j = 1; j < nsoil_layer; j++)
//    {
//      dz[j] = aSoil_thickness[j];
//    }
//    // sum cv*tsoi*dz for energy check
//    ocvts = 0;
//    for (j = 0; j < nsoil_layer; j++)
//    {
//      ocvts += aSpecific_heat_volumetric[j] * aTemperature_soil[j] * dz[j];
//    }
//    // set up vector r and vectors a, b, c that define tridiagonal matrix
//    j = 0;
//    m2 = dz[j] / aThermal_conductivity[j] + dz[j + 1] /
//    aThermal_conductivity[j + 1]; m3 = dTimestep_soil / (dz[j] *
//    aSpecific_heat_volumetric[j]); r[j] = aTemperature_soil[j] + dHeat_flux_in
//    * m3 - (aTemperature_soil[j]
// - aTemperature_soil[j + 1]) * m3 / m2;
//    a[j] = 0;
//    b[j] = 1 + m3 / m2;
//    c[j] = -m3 / m2;
//    //debugtest
//    if (b[j] != b[j] || r[j] != r[j] || a[j] != a[j] || c[j] != c[j])
//    {
//      //double aaa = 1;
//    }
//    j = nsoil_layer - 1;
//    m1 = dz[j - 1] / aThermal_conductivity[j - 1] + dz[j] /
//    aThermal_conductivity[j]; m3 = dTimestep_soil / (dz[j] *
//    aSpecific_heat_volumetric[j]); r[j] = aTemperature_soil[j] +
//    (aTemperature_soil[j - 1] - aTemperature_soil[j]) * m3 / m1; a[j] = -m3 /
//    m1; b[j] = 1 + m3 / m1; c[j] = 0; for (j = 1; j < nsoil_layer - 1; j++)
//    {
//      m1 = dz[j - 1] / aThermal_conductivity[j - 1] + dz[j] /
//      aThermal_conductivity[j]; m2 = dz[j] / aThermal_conductivity[j] + dz[j +
//      1] / aThermal_conductivity[j + 1]; m3 = dTimestep_soil / (dz[j] *
//      aSpecific_heat_volumetric[j]); r[j] = aTemperature_soil[j] +
//      (aTemperature_soil[j - 1] - aTemperature_soil[j]) * m3 / m1
// - (aTemperature_soil[j] - aTemperature_soil[j + 1]) * m3 / m2;
//      a[j] = -m3 / m1;
//      b[j] = 1 + m3 / m1 + m3 / m2;
//      c[j] = -m3 / m2;
//    }
//    // solve for tsoi
//    tridia(nsoil_layer, a, b, c, r, u);
//    for (j = 0; j < nsoil_layer; j++)
//    {
//      aTemperature_soil[j] = u[j];
//    }
//    a.clear();
//    b.clear();
//    c.clear();
//    r.clear();
//    u.clear();
//    return error_code;
//  }
//

} // namespace ecohydrology
