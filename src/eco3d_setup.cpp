// 50==================================================
/*!
  \file   ecosystem_setup.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-16
*/
// 50==================================================
#include "eco3d.h"

namespace ecohydrology
{
// 50==================================================
/*!
  <long-description>

  \param
  \return <ReturnValue>
*/
// 50==================================================
int eco3d::setup_eco3d()
{
  int error_code;
  error_code = setup_default_variable();
  if (1 == error_code)
    {
      error_code = setup_output_variable();
    }

  return error_code;
}

// 50==================================================
/*!
//initialize the configuration strings
\param
\return <ReturnValue>
*/
// 50==================================================
int eco3d::setup_default_variable()
{
  int error_code = 1;
  // 50==================================================
  // initialize system variables
  // 50==================================================
  iFlag_run_steady_state = 1; // we alway assume there is a ss unless specified
  iFlag_run_transient = 1;
  iFlag_run_cascade_model = 0;  // cascade is completed
  iFlag_run_carbon_cascade = 1; // by default, the carbon lateral flow is
                                // enabled
  iFlag_initial_state = 0;

  // 50==================================================
  // define some time variables
  // 50==================================================
  // steady state
  iYear_start_ss = 1700;
  iMonth_start_ss = 1;
  iDay_start_ss = 1;
  iYear_end_ss = 1840;
  iMonth_end_ss = 12;
  iDay_end_ss = 31;
  // transient
  iYear_start_tr = 1841;
  iMonth_start_tr = 1;
  iDay_start_tr = 1;
  iYear_end_tr = 2015;
  iMonth_end_tr = 12;
  iDay_end_tr = 31;
  // save
  iDay_end_save = 31;
  iDay_start_save = 1;
  iMonth_end_save = 12;
  iMonth_start_save = 1;
  iYear_end_save = 2015;
  iYear_start_save = 1980;
  // current available data range
  iYear_available_start = 1980;
  iYear_available_end = 2015;
  // 50==================================================
  // put necessary initialization code here!
  // add map key and default value here
  // 50==================================================
  mParameter.clear(); // clean all the keys first
  // 50==================================================
  // all the workspaces
  // 50==================================================
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_data",
      sWorkspace_data)); // where most global data is stored
  mParameter.insert(std::pair<std::string, std::string>("workspace_scratch",
                                                        sWorkspace_scratch));
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_out",
      sWorkspace_out)); // the output workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_dewpoint",
      sWorkspace_dewpoint)); // the dewpoint data workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_fpar",
      sWorkspace_fpar)); // the fpar data workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_lai",
      sWorkspace_lai)); // the lai data workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_land_cover",
      sWorkspace_land_cover_type)); // the land cover data workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_prec",
      sWorkspace_prec)); // the precipitation data workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_tmax",
      sWorkspace_tmax)); // the maximum air temperature data workspace
  mParameter.insert(std::pair<std::string, std::string>(
      "workspace_tmin",
      sWorkspace_tmin)); // the minimum temperature data workspace

  mParameter.insert(std::pair<std::string, std::string>(
          "workspace_reach_inflow",
          sWorkspace_tmin)); // the minimum temperature data workspace
  // 50==================================================
  mParameter.insert(std::pair<std::string, std::string>(
      "aspect",
      sFilename_aspect)); // the elevation aspect file
  mParameter.insert(std::pair<std::string, std::string>(
      "boundary", sFilename_boundary)); // the boundary file
  mParameter.insert(std::pair<std::string, std::string>(
      "column_index",
      sFilename_column_index)); // the index file of each column
  mParameter.insert(std::pair<std::string, std::string>(
      "dem", sFilename_elevation)); // the dem file
  mParameter.insert(std::pair<std::string, std::string>(
      "header",
      sFilename_header)); // the envi header file for most raster files
  mParameter.insert(std::pair<std::string, std::string>(
      "hru_type", sFilename_hru_type)); // the hru type file
  mParameter.insert(std::pair<std::string, std::string>(
      "latitude", sFilename_latitude)); // the latitude file

  mParameter.insert(std::pair<std::string, std::string>(
      "slope",
      sFilename_slope)); // the elevation slope file

  mParameter.insert(std::pair<std::string, std::string>(
      "stream_reach",
      sFilename_stream_reach)); // the reach file
  mParameter.insert(std::pair<std::string, std::string>(
      "stream_segment",
      sFilename_stream_segment)); // the stream segment file
  mParameter.insert(std::pair<std::string, std::string>(
      "stream_order",
      sFilename_stream_order)); // the stream segment file
  mParameter.insert(std::pair<std::string, std::string>(
      "stream_segment_topology",
      sFilename_stream_segment_topology)); // the segment topology relationship
                                           // file

  mParameter.insert(std::pair<std::string, std::string>(
      "soil_type",
      sFilename_soil_type)); // the soil type file
  mParameter.insert(std::pair<std::string, std::string>(
      "starlog",
      sFilename_log)); // the model simulation log file

  mParameter.insert(
      std::pair<std::string, std::string>("initial_soil_carbon",
                                          sFilename_soil_carbon_initial)); //
  mParameter.insert(std::pair<std::string, std::string>(
      "initial_vegetation_carbon",
      sFilename_vegetation_carbon_initial)); //
  mParameter.insert(
      std::pair<std::string, std::string>("initial_litter_carbon",
                                          sFilename_litter_carbon_initial)); //

  mParameter.insert(std::pair<std::string, std::string>(
      "steady_state_flag",
      convert_integer_to_string(iFlag_run_steady_state, 1)));
  mParameter.insert(std::pair<std::string, std::string>(
      "transient_flag", convert_integer_to_string(iFlag_run_transient, 1)));
  mParameter.insert(std::pair<std::string, std::string>(
      "cascade_flag", convert_integer_to_string(iFlag_run_cascade_model, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
      "run_cascade_flag", convert_integer_to_string(iFlag_run_cascade, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
          "carbon_cascade_flag",
          convert_integer_to_string(iFlag_run_carbon_cascade, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
      "debug_flag", convert_integer_to_string(iFlag_debug, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
      "debug_flag_cascade", convert_integer_to_string(iFlag_debug_cascade, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
      "debug_flag_land", convert_integer_to_string(iFlag_debug_land, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
      "debug_flag_reach", convert_integer_to_string(iFlag_debug_reach, 1)));

  mParameter.insert(std::pair<std::string, std::string>(
      "debug_cu_index", convert_long_to_string(lCU_index_debug)));



  // time variable

  mParameter.insert(std::pair<std::string, std::string>(
      "save_start_year", convert_integer_to_string(iYear_start_save, 4)));
  mParameter.insert(std::pair<std::string, std::string>(
      "save_end_year", convert_integer_to_string(iYear_end_save, 4)));
  mParameter.insert(std::pair<std::string, std::string>(
      "save_start_month", convert_integer_to_string(iMonth_start_save, 2)));
  mParameter.insert(std::pair<std::string, std::string>(
      "save_end_month", convert_integer_to_string(iMonth_end_save, 2)));
  mParameter.insert(std::pair<std::string, std::string>(
      "save_start_day", convert_integer_to_string(iDay_start_save, 2)));
  mParameter.insert(std::pair<std::string, std::string>(
      "save_end_day", convert_integer_to_string(iDay_end_save, 2)));

  mParameter.insert(std::pair<std::string, std::string>(
      "data_start_year", convert_integer_to_string(iYear_available_start, 4)));
  mParameter.insert(std::pair<std::string, std::string>(
      "data_end_year", convert_integer_to_string(iYear_available_end, 4)));
  // 50==================================================

  // 50==================================================
  dWatershed_carbon = 0.0;

  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param
  \return <ReturnValue>
*/
// 50==================================================
int eco3d::setup_output_variable()
{
  int error_code = 1;

  // 50==================================================
  // add output variables look up table
  // the order is very imporant, the look-up-table is listed in the report
  // in general, the variables are listed by modules sequence and alphabeta
  // sequence however, this might not be a good solution since new algorithms
  // will be added later. 50==================================================
  mVariable_system.clear();
  // temperature
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "maximal_temperature", eV_max_temperature));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "minimal_temperature", eV_min_temperature));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "mean_temperature", eV_mean_temperature));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "dewpoint_temperature", eV_dewpoint_temperature));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "temperature_cumulative", eV_temperature_cumulative));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "vapor_pressure_deficit", eV_vapor_pressure_deficit));
  // precipitation
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "precipitation_type", eV_precipitation_type));
  mVariable_system.insert(std::pair<std::string, eVariable>("rain", eV_rain));
  mVariable_system.insert(std::pair<std::string, eVariable>("snow", eV_snow));
  // radiation
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "photosynthetically_active_radiation",
      eV_photosynthetically_active_radiation));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "shortwave_radiation", eV_shortwave_radiation));
  // potential et
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "potential_evapotranspiration", eV_potential_evapotranspiration));
  // canopy
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "canopy_precipitation_type", eV_canopy_precipitation_type));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "canopy_water_depth", eV_canopy_water_depth));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "canopy_water_storage", eV_canopy_water_storage));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "canopy_evapotranspiration", eV_canopy_evapotranspiration));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("canopy_rain", eV_canopy_rain));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("canopy_snow", eV_canopy_snow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "canopy_net_precipitation", eV_canopy_net_precipitation));
  // photosynthesis
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "gross_primary_production", eV_gross_primary_production));
  // litterfall
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litterfall_carbon", eV_litterfall_carbon));
  //	mVariable_system.insert(std::pair<std::string,
  // eVariable>("litterfall_nitrogen", eV_litterfall_nitrogen)); canopy
  mVariable_system.insert(
      std::pair<std::string, eVariable>("canopy_carbon", eV_canopy_carbon));
  //	mVariable_system.insert(std::pair<std::string,
  // eVariable>("canopy_nitrogen", eV_canopy_nitrogen));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("stem_carbon", eV_stem_carbon));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "root_carbon_decomposition", eV_root_carbon_decomposition));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("root_carbon", eV_root_carbon));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("vegetation_maintenance_respiration",
                                        eV_vegetation_maintenance_respiration));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "vegetation_growth_respiration", eV_vegetation_growth_respiration));

  mVariable_system.insert(
      std::pair<std::string, eVariable>("vegetation_autotrophic_respiration",
                                        eV_vegetation_autotrophic_respiration));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "vegetation_carbon", eV_vegetation_carbon));
  // snow
  mVariable_system.insert(
      std::pair<std::string, eVariable>("snow_cover_area", eV_snow_cover_area));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("snow_albedo", eV_snow_albedo));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "snow_evapotranspiration", eV_snow_evapotranspiration));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("snow_melt", eV_snow_melt));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "snow_water_equivalent", eV_snow_water_equivalent));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "snow_temperature", eV_snow_temperature));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "snow_heat_deficit", eV_snow_heat_deficit));
  // glacier

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "glacier_water_equivalent", eV_glacier_water_equivalent));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("glacier_melt", eV_glacier_melt));
  // infiltration
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_infiltration", eV_surface_infiltration));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_excess", eV_surface_runoff_excess));
  // surface runoff
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_inflow", eV_surface_runoff_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_outflow", eV_surface_runoff_outflow));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_inflow_upslope", eV_surface_runoff_inflow_upslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_inflow_external", eV_surface_runoff_inflow_external));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_outflow_downslope", eV_surface_runoff_outflow_downslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "surface_runoff_outflow_infiltration",
      eV_surface_runoff_outflow_infiltration));

  // litter

  mVariable_system.insert(
      std::pair<std::string, eVariable>("litter_carbon", eV_litter_carbon));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_carbon_influx", eV_litter_carbon_influx));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_carbon_outflux_humus", eV_litter_carbon_outflux_humus));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "undecomposed_litter_carbon", eV_undecomposed_litter_carbon));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "decomposed_litter_carbon", eV_decomposed_litter_carbon));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "decomposed_litter_solid_carbon", eV_decomposed_litter_ioc));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "decomposed_litter_spsoc", eV_decomposed_litter_soc));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_dissolved_organic_carbon_inflow",
      eV_litter_dissolved_organic_carbon_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_dissolved_organic_carbon_outflow",
      eV_litter_dissolved_organic_carbon_outflow));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_dissolved_organic_carbon_inflow_upslope",
      eV_litter_dissolved_organic_carbon_inflow_upslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_dissolved_organic_carbon_inflow_external",
      eV_litter_dissolved_organic_carbon_inflow_external));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_dissolved_organic_carbon_outflow_downslope",
      eV_litter_dissolved_organic_carbon_outflow_downslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "litter_dissolved_organic_carbon_outflow_leaching",
      eV_litter_dissolved_organic_carbon_outflow_leaching));
  // soil
  mVariable_system.insert(
      std::pair<std::string, eVariable>("dunnian_runoff", eV_dunnian_runoff));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_interflow_inflow", eV_soil_interflow_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_interflow_outflow", eV_soil_interflow_outflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_interflow_storage", eV_soil_interflow_storage));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_interflow_inflow_upslope", eV_soil_interflow_inflow_upslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_interflow_inflow_external", eV_soil_interflow_inflow_external));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_interflow_outflow_downslope", eV_soil_interflow_outflow_downslope));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_evapotranspiration", eV_soil_evapotranspiration));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("soil_moisture", eV_soil_moisture));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_moisture_volumetric", eV_soil_moisture_volumetric));
  // doc
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_inflow",
      eV_soil_dissolved_organic_carbon_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_outflow",
      eV_soil_dissolved_organic_carbon_outflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_storage",
      eV_soil_dissolved_organic_carbon_storage));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_concentration",
      eV_soil_dissolved_organic_carbon_concentration));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_production",
      eV_soil_dissolved_organic_carbon_production));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_mineralization",
      eV_soil_dissolved_organic_carbon_mineralization));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_adsorption",
      eV_soil_dissolved_organic_carbon_adsorption));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_desorption",
      eV_soil_dissolved_organic_carbon_desorption));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_inflow_upslope",
      eV_soil_dissolved_organic_carbon_inflow_upslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_inflow_external",
      eV_soil_dissolved_organic_carbon_inflow_external));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_inflow_groundwater",
      eV_soil_dissolved_organic_carbon_inflow_groundwater));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_outflow_downslope",
      eV_soil_dissolved_organic_carbon_outflow_downslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_dissolved_organic_carbon_outflow_groundwater",
      eV_soil_dissolved_organic_carbon_outflow_groundwater));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_spsoc_mineralization", eV_soil_spsoc_mineralization));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_spsoc_adsorption", eV_soil_spsoc_adsorption));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "soil_spsoc_desorption", eV_soil_spsoc_desorption));

  // groundwater
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_inflow", eV_groundwater_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_outflow", eV_groundwater_outflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_storage", eV_groundwater_storage));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_inflow_soil", eV_groundwater_inflow_soil));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_inflow_upslope", eV_groundwater_inflow_upslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_inflow_external", eV_groundwater_inflow_external));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_outflow_downslope", eV_groundwater_outflow_downslope));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "groundwater_outflow_soil", eV_groundwater_outflow_soil));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "heterotrophic_respiration", eV_soil_heterotrophic_respiration));

  // reach reach

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_inflow", eV_reach_discharge_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_outflow", eV_reach_discharge_outflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_storage", eV_reach_discharge_storage));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_inflow_upstream", eV_reach_discharge_inflow_upstream));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_inflow_lateral", eV_reach_discharge_inflow_lateral));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_inflow_external", eV_reach_discharge_inflow_external));
  mVariable_system.insert(
      std::pair<std::string, eVariable>("reach_discharge_inflow_groundwater",
                                        eV_reach_discharge_inflow_groundwater));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_inflow_precipitation",
      eV_reach_discharge_inflow_precipitation));

  mVariable_system.insert(
      std::pair<std::string, eVariable>("reach_discharge_outflow_downstream",
                                        eV_reach_discharge_outflow_downstream));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_outflow_groundwater",
      eV_reach_discharge_outflow_groundwater));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_discharge_outflow_evapotranspiration",
      eV_reach_discharge_outflow_evapotranspiration));

  // stream doc
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_inflow",
      eV_reach_dissolved_organic_carbon_inflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_outflow",
      eV_reach_dissolved_organic_carbon_outflow));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_storage",
      eV_reach_dissolved_organic_carbon_storage));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_concentration",
      eV_reach_dissolved_organic_concentration));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_inflow_upstream",
      eV_reach_dissolved_organic_carbon_inflow_upstream));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_inflow_lateral",
      eV_reach_dissolved_organic_carbon_inflow_lateral));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_inflow_external",
      eV_reach_dissolved_organic_carbon_inflow_external));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_inflow_groundwater",
      eV_reach_dissolved_organic_carbon_inflow_groundwater));

  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_outflow_downstream",
      eV_reach_dissolved_organic_carbon_outflow_downstream));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "reach_dissolved_organic_carbon_outflow_groundwater",
      eV_reach_dissolved_organic_carbon_outflow_groundwater));

  mVariable_system.insert(
      std::pair<std::string, eVariable>("soil_carbon", eV_soil_carbon));

  mVariable_system.insert(
      std::pair<std::string, eVariable>("stream_carbon", ev_stream_carbon));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "net_primary_production", eV_net_primary_production));
  mVariable_system.insert(std::pair<std::string, eVariable>(
      "net_ecosystem_production", eV_net_ecosystem_production));
  return error_code;
}
} // namespace ecohydrology
