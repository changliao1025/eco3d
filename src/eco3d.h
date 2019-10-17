// 50==================================================
/*!
  \file ecosystem.h
  \brief
  The head quarter of ECO3D model.
  \author Chang Liao
  \date 2016-09-28
*/
// 50==================================================
#pragma once
// c header==========================
#include <cmath>
// c++ header========================
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string> //filenames and variables
#include <vector>
// 50==================================================
// local headers
// 50==================================================
#include "cascade.h"
#include "column.h"
#include "conversion.h"
#include "data.h"
#include "date.h"
#include "global.h"
#include "mathematics.h"
#include "miscellaneous.h"
#include "system.h"
// 50==================================================
// cros-platform header
#ifdef _WIN32
// define something for Windows (32-bit and 64-bit, this part is common)
#include <omp.h>
#ifdef _WIN64
// define something for Windows (64-bit only)
#else
// define something for Windows (32-bit only)
#endif
#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
// linux
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif
#if defined(__linux__)
#else
#endif
using namespace std;
// 50==================================================
// the lookup table for user output variables
// 50==================================================
enum eVariable
{
  eV_max_temperature,
  eV_min_temperature,
  eV_mean_temperature,
  eV_dewpoint_temperature,
  eV_temperature_cumulative,
  eV_vapor_pressure_deficit,
  eV_precipitation_type,
  eV_rain,
  eV_snow,
  eV_photosynthetically_active_radiation,
  eV_shortwave_radiation,
  eV_potential_evapotranspiration,

  eV_canopy_precipitation_type,
  eV_canopy_water_depth,
  eV_canopy_water_storage,
  eV_canopy_evapotranspiration,
  eV_canopy_rain,
  eV_canopy_snow,
  eV_canopy_net_precipitation,

  eV_gross_primary_production,
  eV_litterfall_carbon,
  eV_canopy_carbon,
  eV_stem_carbon,
  eV_root_carbon_decomposition,
  eV_root_carbon,

  eV_vegetation_maintenance_respiration,
  eV_vegetation_growth_respiration,

  eV_vegetation_autotrophic_respiration,
  eV_vegetation_carbon,

  eV_snow_cover_area,
  eV_snow_albedo,
  eV_snow_evapotranspiration,
  eV_snow_melt,
  eV_snow_water_equivalent,
  eV_snow_temperature,
  eV_snow_heat_deficit,

  eV_glacier_water_equivalent,
  eV_glacier_melt,

  eV_surface_infiltration,
  eV_surface_runoff_excess,

  eV_surface_runoff_inflow,
  eV_surface_runoff_outflow,

  eV_surface_runoff_inflow_upslope,
  eV_surface_runoff_inflow_external,
  eV_surface_runoff_outflow_downslope,
  eV_surface_runoff_outflow_infiltration,

  eV_litter_carbon,
  eV_litter_carbon_influx,
  eV_litter_carbon_outflux_humus,
  eV_undecomposed_litter_carbon,
  eV_decomposed_litter_carbon,
  eV_decomposed_litter_ioc,
  eV_decomposed_litter_soc,

  eV_litter_heterotrophic_respiration,

  eV_litter_dissolved_organic_carbon_inflow,
  eV_litter_dissolved_organic_carbon_outflow,

  eV_litter_dissolved_organic_carbon_inflow_upslope,
  eV_litter_dissolved_organic_carbon_inflow_external,
  eV_litter_dissolved_organic_carbon_outflow_downslope,
  eV_litter_dissolved_organic_carbon_outflow_leaching,

  eV_dunnian_runoff,

  eV_soil_interflow_inflow,
  eV_soil_interflow_outflow,
  eV_soil_interflow_storage,
  eV_soil_interflow_inflow_upslope,
  eV_soil_interflow_inflow_external,

  eV_soil_interflow_outflow_downslope,

  eV_soil_evapotranspiration,

  eV_soil_moisture,
  eV_soil_moisture_volumetric,

  eV_soil_dissolved_organic_carbon_inflow,
  eV_soil_dissolved_organic_carbon_outflow,
  eV_soil_dissolved_organic_carbon_storage,
  eV_soil_dissolved_organic_carbon_concentration,
  eV_soil_dissolved_organic_carbon_production,
  eV_soil_dissolved_organic_carbon_mineralization,
  eV_soil_dissolved_organic_carbon_adsorption,
  eV_soil_dissolved_organic_carbon_desorption,
  eV_soil_dissolved_organic_carbon_inflow_upslope,
  eV_soil_dissolved_organic_carbon_inflow_external,
  eV_soil_dissolved_organic_carbon_inflow_groundwater,
  eV_soil_dissolved_organic_carbon_outflow_downslope,
  eV_soil_dissolved_organic_carbon_outflow_groundwater,
  eV_soil_spsoc_mineralization,
  eV_soil_spsoc_adsorption,
  eV_soil_spsoc_desorption,
  eV_groundwater_inflow,
  eV_groundwater_outflow,
  eV_groundwater_storage,
  eV_groundwater_inflow_soil,
  eV_groundwater_inflow_upslope,
  eV_groundwater_inflow_external,
  eV_groundwater_outflow_downslope,
  eV_groundwater_outflow_soil,

  eV_soil_heterotrophic_respiration,

  eV_reach_discharge_inflow,
  eV_reach_discharge_outflow,
  eV_reach_discharge_storage,
  eV_reach_discharge_inflow_upstream,
  eV_reach_discharge_inflow_lateral,
  eV_reach_discharge_inflow_external,
  eV_reach_discharge_inflow_groundwater,
  eV_reach_discharge_inflow_precipitation,
  eV_reach_discharge_outflow_downstream,
  eV_reach_discharge_outflow_groundwater,
  eV_reach_discharge_outflow_evapotranspiration,

  eV_reach_dissolved_organic_carbon_inflow,
  eV_reach_dissolved_organic_carbon_outflow,
  eV_reach_dissolved_organic_carbon_storage,
  eV_reach_dissolved_organic_concentration,
  eV_reach_dissolved_organic_carbon_inflow_upstream,
  eV_reach_dissolved_organic_carbon_inflow_lateral,
  eV_reach_dissolved_organic_carbon_inflow_external,
  eV_reach_dissolved_organic_carbon_inflow_groundwater,
  eV_reach_dissolved_organic_carbon_outflow_downstream,
  eV_reach_dissolved_organic_carbon_outflow_groundwater,

  eV_soil_carbon,

  ev_stream_carbon,
  eV_net_primary_production,
  eV_net_ecosystem_production,
};
// 50==================================================
namespace ecohydrology
{
class eco3d
{
public:
  eco3d();

  // 50==================================================
  // construct with a parameter file is usually required,
  // most inputs are directly read from the configuration file
  // 50==================================================
  explicit eco3d(std::string sFilename_configuration);

  ~eco3d();

  // 50==================================================
  // members
  // 50==================================================
  // used for debug
  //int iFlag_save_budget;
  int iFlag_debug;
  int iFlag_debug_cascade;
  int iFlag_debug_land;
  int iFlag_debug_reach;
  int iFlag_run_cascade;
  int iFlag_run_water_cascade;
  // flag for lateral carbon flow through DOC/DIC
  int iFlag_run_carbon_cascade;
  // a flag for configuration file
  int iFlag_configuration_file;
  // whether the model reach steady state or not
  int iFlag_steady_state;
  // the flag whether the surface cascade model will be run.
  int iFlag_run_cascade_model;
  // the flag whether to run a steady state simulation or a transient simulation
  int iFlag_run_steady_state;
  int iFlag_run_transient;
  int iFlag_soil_carbon_initial;
  int iFlag_vegetation_carbon_initial;
  int iFlag_litter_carbon_initial;
  int iFlag_initial_state;
  // 50==================================================
  // time variables
  // 50==================================================

  // steady state
  int iDay_end_ss;
  int iDay_start_ss;
  int iMonth_end_ss;
  int iMonth_start_ss;
  int iYear_end_ss;
  int iYear_start_ss;
  // transient
  int iDay_end_tr;
  int iDay_start_tr;
  int iMonth_end_tr;
  int iMonth_start_tr;
  int iYear_end_tr;
  int iYear_start_tr;
  // save
  int iDay_end_save;
  int iDay_start_save;
  int iMonth_end_save;
  int iMonth_start_save;
  int iYear_end_save;
  int iYear_start_save;
  // the input data range
  int iYear_available_start;
  int iYear_available_end;
  long lJulianDay_available_start; // the julian day of available data
  long lColumn_count;
  long lCU_index_debug;
  double dWatershed_carbon;
  double dWatershed_vegetation_carbon;
  double dWatershed_soil_carbon;
  double dWatershed_litter_carbon;
  double dWatershed_heterotrophic_respiration;
  double dWatershed_soil_doc;
  double dWatershed_soil_doc_production;
  double dWatershed_soil_doc_mineralization;
  double dWatershed_litter_doc_leaching;
  double dWatershed_reach_doc_lateral;
    double  dWatershed_reach_doc_lateral_litter;
    double  dWatershed_reach_doc_lateral_soil;
  // history
  double dWatershed_carbon_previous;
  double dWatershed_carbon_vegetation_previous;
  double dWatershed_carbon_soil_previous;
  double dWatershed_carbon_litter_previous;
  std::string sExtension_header;
  std::string sExtension_envi;
  std::string sExtension_text;
  std::string sPrefix_dewpoint;
  std::string sPrefix_fpar;
  std::string sPrefix_lai;
  std::string sPrefix_land_cover_type;
  std::string sPrefix_prec;
  std::string sPrefix_tmax;
  std::string sPrefix_tmin;
  std::string sModule_cascade;
  std::string sModule_snow;
  std::string sFilename_configuration;
  // 50==================================================
  std::string sWorkspace_data; // the general data workspace
  std::string sWorkspace_scratch;
  std::string sWorkspace_out;      // the output workspace
  std::string sWorkspace_out_lateral_carbon; //simulation with lateral carbon
  std::string sWorkspace_out_no_lateral_carbon; //simulation without lateral carbon flow
  std::string sWorkspace_out_no_lateral_water; //simulation without lateral water flow
  std::string sWorkspace_dewpoint; // the input workspace of dewpoint data
  std::string sWorkspace_fpar;     // the input workspace of fpar data
  std::string sWorkspace_lai;      // the input workspace of lai data
  std::string sWorkspace_tmax;     // the input workspace of tmax data
  std::string sWorkspace_tmin;     // the input workspace of tmin data
  std::string sWorkspace_prec;     // the input workspace of prec data
  std::string
      sWorkspace_land_cover_type; // the input workspace of land cover type data
  std::string sWorkspace_reach_inflow;
  // 50==================================================
  // global data files
  // 50==================================================
  std::string sFilename_aspect;   // surface elevation aspect
  std::string sFilename_boundary; // boundary file, it also includes the erosion
  std::string sFilename_column_index; // the index of each column to be
                                      // simulated
  std::string sFilename_elevation; // surface elevation
  std::string sFilename_header;    // the header file used to output file
  std::string sFilename_hru_type;  // the hru file
  std::string sFilename_log;       // the log file
  std::string sFilename_debug_table;
  std::string sFilename_budget_table;
  std::string sLog;
  std::string sFilename_latitude;     // latitude file, for radiation use mainly
  std::string sFilename_stream_reach; // the raw text file exported from ArcMap
  std::string
      sFilename_stream_segment; // the segment index file exported from ArcMap
  std::string
      sFilename_stream_order; // the stream order from watershed delineation
  std::string
      sFilename_stream_segment_topology; // the upstream downstream relation
                                         // from watershed delineation
  std::string sFilename_slope;     // surface elevation slope
  std::string sFilename_soil_type; // soil type file
  // initial pools
  std::string sFilename_soil_carbon_initial;
  std::string sFilename_vegetation_carbon_initial;
  std::string sFilename_litter_carbon_initial;
  // 50==================================================
  // array for global data, which does not change with time
  // 50==================================================
    std::vector<long> vTime_step;
  std::vector<double> vAspect;       // unit: degree
  std::vector<double> vBoundary;     // unit: see boundary file description
  std::vector<double> vColumn_index; // the index of each column
  std::vector<double>
      vElevation; // the surface elevation of each grid cell, unit: meter
  std::vector<double> vHru_type; // refer to the hru file definition
  std::vector<double> vLatitude; // the latitude of the grid cell, unit: degree
  std::vector<double> vSlope;    // the slope of the grid cell, unit: degree
  // vSegment_reach stores the reach information for each segment
  std::vector<double> vStream_reach; // the vector contact for each segment
  std::vector<double>
      vStream_segment; // the segment index from the watershed delineation
  std::vector<double> vStream_order;
  std::vector<double> vStream_reach_travel_time;
  std::vector<double> vStream_reach_width;
  // 50==================================================
  std::vector<double>
      vSoil_type; // the soil type obtained from the USDA soil classification
  std::vector<double> vLand_cover_type_init;
  std::vector<double> vSoil_carbon_init;
  std::vector<double> vVegetation_carbon_init;
  std::vector<double> vLitter_carbon_init;
  // 50==================================================
  // the array to store the segment topology information
  std::array<std::array<int, 2>, nsegment> aStream_segment_topology;
  // the array to store the reach topology information
  // so dealing with reach index, use this vector whenever possible
  std::vector<std::vector<long>> vStream_segment_reach;
  std::vector<std::vector<std::vector<double>>> vReach_inflow_data_daily;
  // 50==================================================
  // surface cascade parameter
  // 50==================================================
  std::vector<std::vector<double>> vCascade_parameter;
  // 50==================================================
  // parameter map read from the configuration file
  // 50==================================================
  std::map<std::string, std::string> mParameter; // for input data and
                                                 // parameters
  // 50==================================================
  // mVariable_user is used to store all the variables users want to save
  // the key store the key in the dictionary (mVariable_system)
  // user can change the prefix of the output name using the value
  // 50==================================================
  std::map<std::string, std::string> mVariable_user;
  // the map for all possible output
  std::map<std::string, eVariable> mVariable_system;
  // 50==================================================
  std::vector<column> vColumn; // the container of all the column/grids
  // 50==================================================
  // class members
  // 50==================================================
  column cColumn; // the normal simulation
  // segment cSegment; //the stream routing module, which must be independent
  // with the column based algorithms
  cascade cCascade; // the surface cascade module
  // 50==================================================
  // std::vector< std::ifstream > vIfstram_stream; //for the sake of extension,
  // I do not plan to oprn too much file during runtime.
  std::ofstream ofs_log; // used for IO starlog file
  std::ofstream ofs_debug;
  std::ofstream ofs_budget;
  // 50==================================================
  // functions
  // 50==================================================
  // setup component
  // 50==================================================
  int setup_eco3d();

  int setup_default_variable();

  int setup_output_variable();

  // 50==================================================
  // read component
  // 50==================================================
  int read_eco3d();

  // 50==================================================
  // read the configuration file
  // 50==================================================
  int read_configuration_file(); // read configuration file from arguments
  // 50==================================================
  // read the time-invariant variable, such as dem
  // 50==================================================
  int retrieve_user_input();
  int read_cascade_parameter();

  int read_global_data(); // time-invariant file
  // 50==================================================
  // read stream module related files
  // 50==================================================
  int read_stream_segment_topology();

  int check_stream_segment_reach_daily_data(
      std::vector<std::vector<std::vector<double>>> vDaily_data_in);

  int create_stream_segment_reach_topology();

  std::vector<std::vector<std::vector<double>>>
  read_stream_reach_inflow(int iYear_ymd, int iMonth_ymd, int iDay_ymd);

  // 50==================================================
  // initialize the model
  // 50==================================================
  int initialize_eco3d();
  int initialize_time_step();
  int find_nearest_time_step(int iTime_step_in);
  int initialize_spatial_domain();

  int assign_time_invariant_data();

  int initialize_ecosystem();

  int initialize_ecosystem_from_initial_file();

  int run_and_save_eco3d();
  int setup_cascade_parameter();
  int call_cascade_model();

  // 50==================================================
  // run steady state simulation
  // 50==================================================
  int run_steady_state_simulation();

  // 50==================================================
  // run transient modules
  // 50==================================================
  int run_transient_simulation();

  // 50==================================================
  // run individual modules
  // 50==================================================
  int run_modules(int iFlag_mode, int iDay_end, int iDay_start, int iMonth_end,
                  int iMonth_start, int iYear_end, int iYear_start);

  int all_cascade(long iRow_in, long lColumn_in);

  // 50==================================================
  // retrieve the column index based on 2D i and j index
  // 50==================================================
  long calculate_index_from_coordinates(long iRow_in, long lColumn_in);

  int run_stream_reach();

  // 50==================================================
  // calculate some system budget
  // 50==================================================
  int summarize(int iYear, int iDay);

  int check_steady_state();
  int save_system_parameter();
  int save_system_status(int iDay, int iYear);
  int cleanup_eco3d();
};
} // namespace ecohydrology
