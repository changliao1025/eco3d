// 50==================================================
/*!
  \file   snow.h
  \brief

  <long description>

  \author Chang Liao
  \date   2017-01-24
*/
// 50==================================================
#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include "date.h"
#include "dimension.h"
#include "global.h"

using namespace std;

// 50==================================================
const int iDay_albedo_max = 15; //

const double dSettle_const = 0.1;
// the fraction of free water stored in the snowpack ice form, unit,
// fraction
const double dFreewater_capacity_coefficient = 0.05;
const double dSwe_density_max = 0.6 * 1000; // maximum snow density, unit, kg/m3
const double dSwe_density_init =
    0.1 * 1000; // initial snow density unit kg per cubic meter
const double dEmissivity_no_precipitation = 0.757; // fraction
const double dFaction_rain_max_melt = 0.6;         // fraction
const double dFaction_rain_max_accumulate = 0.8;   // fraction
const double dSnow_reset_threshold_accumulate = 0.2 * inch_2_centimeter;
const double dSnow_reset_threshold_melt = 0.05 * inch_2_centimeter;
const double dTransmission_coefficient = 0.5;
const double dDensity_new_snow = 1000.0; // same as water
// the snow cover depletion curve
const std::array<double, 11> aSCA_curve = {
    {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 0.99}};
const std::array<double, 15> aAlbedo_accumulate = {
    {0.80, 0.77, 0.75, 0.72, 0.70, 0.69, 0.68, 0.67, 0.66, 0.65, 0.64, 0.63,
     0.62, 0.61, 0.60}};
const std::array<double, 15> aAlbedo_melt = {{0.72, 0.65, 0.60, 0.58, 0.56,
                                              0.54, 0.52, 0.50, 0.48, 0.46,
                                              0.44, 0.43, 0.42, 0.41, 0.40}};
// calories per degree
const std::array<double, nmonth> aConvection_coefficient = {
    {2.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 5.0, 4.0, 3.0, 2.0, 2.0}};

// 50==================================================
// the SI units are used whenever possible. However, you can convert to other
// units at will.  for rain and snow, the centimeter is used
// 50==================================================
namespace ecohydrology
{
class snow
{
public:
  snow();

  ~snow();

  // the snow system states variables
  // unlike other modules, we do NOT use "_hru" suffix to denote average any
  // more

  int iFlag_melt_season_potential; // potential day to check melt
  int iFlag_melt_season_force;     // snowmelt force starts

  int iFlag_old_snow; // flag to indicate whether there is old snowpack/previous

  int iFlag_has_snow;
  int iFlag_new_snow; // flag to indicate whether there is new snowpack
  // the method be used to calculate sca. 0 (irregular) or 1 (regular)
  int iFlag_sca_on_curve;
  int iFlag_partly_reset_albedo;
  int iFlag_albedo_method; // method to calculate the snow albedo

  int iDay_since_last_snow;
  int iDay_since_last_reset;
  int iCount_above_zero_day;

  double dSnow_cumulative_save; //??check later
  double dAlbedo;               // current albedo, unitless, between 0 and 1
  double dAlbedo_previous;      // previous albedo
  double dSca;                  // snow cover area, unit: fraction
  double dSca_previous;         // previous sca
  double dSnowmelt;             // the melt snow, unit: meter

  double dSwe; // snow water equivlent, unit: meter

  double dSwe_density; // swe density, unit: kg per cubic meter
  double dSwe_depth;   // swe depth, unit: meter
  // we define the heat deficit as negative (no larget than 0.0 )throughout the
  // model
  double dSnow_heat_deficit;

  double dSwe_ice;      // the ice content in swe, unit: meter
  double dSwe_max;      // the maximum swe, unit: meter
  double dSwe_plus;     // record the swe with 0.25 melt after departure
  double dSwe_previous; // record the swe before it departures from regular
  // curve
  double dSwe_sub_max;      // the record of current max swe
  double dSnow_temperature; // temperature of the snowpack, unit: kelvin
  double dSnow_temperature_celsius;
  double dSwe_threshold; // unit: meter
  double dSwe_water;     // free water stored in the snowpack, unit: meter

  double dSnow_et;
  double dSnow_et_hru;

  double dTemperature_surface_snow; // surface snow temperature, unit: kelvin

  int adjust_snowpack_freewater();

  int freeze_snowpack_freewater(double dTemperature_air_in);

  int melt_snowpack_ice(double dHeat_in);

  double calculate_convection_heat_flux(int iMonth, int iLand_cover_type,
                                        double dPrecipitation_basin_in,
                                        double dTemperature_air_in);

  double
  calculate_effective_shortwave_radiation(double dShortwave_radiation_in);

  double calculate_effective_longwave_radiation(int iFlag_time, int iStorm_type,
                                                double dLai_in,
                                                double dPrecipitation_basin_in,
                                                double dTemperature_air_in);

  double calculate_night_radiation(int iLand_cover_type, int iMonth_ymd,
                                   int iStorm_type, double dLai_in,
                                   double dPrecipitation_basin_in,
                                   double dTemperature_night_in);

  double calculate_day_radiation(int iLand_cover_type, int iMonth_ymd,
                                 int iStorm_type, double dLai_in,
                                 double dPrecipitation_basin_in,
                                 double dShortwave_effective_incoming,
                                 double dTemperature_day_in);

  int calculate_snow_albedo(int iPrecipitation_type, double dRain_percentage_in,
                            double dSnow_net_in);

  int calculate_snow_cover_area(int iFlag_new_snow, double dSnow_net_in);

  double calculate_snow_conduction_heat_flux();

  double calculate_snow_convection_heat_flux(int iMonth, int iLand_cover_type,
                                             double dTemperature_air_in,
                                             double dPrecipitation_basin_in);

  int update_snow_depth_and_density(double dSnow_in);

  int update_snow_temperature(double dTemperature_air_in);

  int calculate_sca_on_curve();

  // snow et
  int calculate_snow_evapotranspiration(int iFlag_transpiration_status,
                                        int iLand_cover_type,
                                        double dHru_pet_in);

  int determine_accumulate_or_melt_season(int iDay_doy);

  int determine_elevation_based_parameter(double dElevation_in);

  int initialize(eColumn_type eCU_type);

  int run_snow_model(int iDay_doy, int iFlag_transpiration,
                     int iLand_cover_type, int iMonth_ymd,
                     int iPrecipitation_type, int iStorm_type,
                     double dElevation_in, double dET_available_hru_in,
                     double dLai_in, double dPrecipitation_basin_in,
                     double dRain_net_in, double dSnow_net_in,
                     double dShortwave_radiation_in, double dTemperature_max_in,
                     double dTemperature_mean_in, double dTemperature_min_in,
                     double dTemperature_rain_in, double dTemperature_snow_in);

  int update_accumulate_or_melt_season();

  int update_snow_parameter();

  int update_snow_status();

  // double dSnowmelt_day;    // the daily snowmelt, unit, meter
  // double dSnowmelt_night;
  // double dSnowmelt_hru;
};
} // namespace ecohydrology
