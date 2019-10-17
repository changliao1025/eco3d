#pragma once

#include "dimension.h"
#include "evapotranspiration.h"
#include "geology.h"
#include "groundwater.h"
#include "hru.h"
#include "landcover.h"
#include "litter.h"
#include "snow.h"
#include "soil.h"
#include "surface_runoff.h"
#include "vegetation.h"

using namespace std;
namespace ecohydrology
{
class land
{
public:
  land();

  ~land();

  int iFlag_run_carbon_cascade;
  int iFlag_source_or_sink;
  // int iColumn_type;
  eColumn_type eCU_type;

  double dLand_carbon;
  double dLand_pet;
  double dLand_nep;

  // 50==================================================
  // class member
  // 50==================================================
  evapotranspiration cEvapotranspiration;
  geology cGeology;
  groundwater cGroundwater;
  landcover cLandcover;
  litter cLitter;
  snow cSnow;
  soil cSoil;
  vegetation cVegetation;
  surface_runoff cSurface_runoff;
  hru cHru;

  // 50==================================================
  // function
  // 50==================================================
  int initialize_land(eColumn_type eCU_type);

  int run_land_model(int iDay_doy, int iFlag_transpiration, int iMonth_ymd,
                     int iPrecipitation_type, int iStorm_type, double dFpar,
                     double dLai, double dPar, double dPrecipitation,
                     double dPrecipitation_basin, double dRain,
                     double dShortwave_radiation, double dSnow,
                     double dTemperature_basin_max,
                     double dTemperature_dewpoint, double dTemperature_max,
                     double dTemperature_mean, double dTemperature_min,
                     double dTemperature_rain_in, double dTemperature_snow_in,
                     double dVapor_pressure_deficit);

  int update_land_status();

  int update_land_parameter(int iFlag_run_carbon_cascade,int iFlag_run_water_cascade, int iLand_cover_type,
                            int iSoil_type, int iVegetation_type,
                            double dSurface_depression_percentage_in,
                            double dImpervious_surface_percentage_in,
                            double dPervious_surface_percentage_in);
};
} // namespace ecohydrology
