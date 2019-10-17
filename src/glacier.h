// 50==================================================
/*!
  \file   glacier.h
  \brief
  <long description>
  \author Chang Liao
  \date   2017-07-16
*/
// 50==================================================
#pragma once

#include "dimension.h"
#include "evapotranspiration.h"
#include "geology.h"
#include "global.h"
#include "hru.h"
#include "landcover.h"
#include "snow.h"
#include "surface_runoff.h"
using namespace std;
namespace ecohydrology
{
class glacier
{
public:
  glacier();
  ~glacier();

  int iFlag_run_carbon_cascade;
  int iFlag_source_or_sink;

  double dGlacier_nep;
  double dGlacier_pet;

  double dGwe;

  double dGlacier_melt;

  double dTemperature_glacier_melt_base;
  eColumn_type eCU_type;

  snow cSnow; // snow layer
  surface_runoff cSurface_runoff;
  evapotranspiration cEvapotranspiration;
  geology cGeology;
  hru cHru;
  landcover cLandcover;

  int calculate_glacier_base_temperature(double dElevation_in);
  int initialize_glacier(eColumn_type eCU_type);
  int run_glacier_model(int iDay_ymd, int iFlag_transpiration, int iMonth_ymd,
                        int iPrecipitation_type, int iStorm_type,
                        double dLai_in, double dPrecipitation_basin_in,
                        double dRain_in, double dShortwave_radiation_in,
                        double dSnow_in, double dTemperature_max_in,
                        double dTemperature_mean_in, double dTemperature_min_in,
                        double dVapor_pressure_deficit_in,
                        double dTemperature_rain_in,
                        double dTemperature_snow_in);
  int update_glacier_status();
  int update_glacier_parameter(int iFlag_run_carbon_cascade,
                               int iLand_cover_type);
};
} // namespace ecohydrology
