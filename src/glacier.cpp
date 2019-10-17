// 50==================================================
/*!
  \file   glacier.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-26
*/
// 50==================================================
#include "glacier.h"

namespace ecohydrology
{
glacier::glacier()
{
  dGwe = missing_value;
  dGlacier_melt = missing_value;
}

glacier::~glacier() = default;

// 50==================================================
/*!
  <long-description>

  \param iLand_cover_type
  \return <ReturnValue>
*/
// 50==================================================
int glacier::initialize_glacier(eColumn_type eCU_type)
{
  int error_code = 1;
  // pass from class column to class glacier
  this->eCU_type = eCU_type;

  if (eCU_type != eCU_glacier)
    {

      // something is wrong
      error_code = 0;
    }
  else
    {
      dGwe = 0.0;
      dGlacier_melt = 0.0;
      // initialize class member
      cEvapotranspiration.initialize_evapotranspiration();
      cHru.initialize_hru(eCU_type);
      cLandcover.initialize_landcover();
      cSnow.initialize(eCU_type);
      cSurface_runoff.initialize_surface_runoff(eCU_type);
    }
  return error_code;
}
// 50==================================================
/*!
  <long-description>

  \param iLand_cover_type
  \return <ReturnValue>
*/
// 50==================================================
int glacier::update_glacier_parameter(int iFlag_run_carbon_cascade,
                                      int iLand_cover_type)
{
  int error_code = 1;
  // int iSoil_type = 0;
  this->iFlag_run_carbon_cascade = iFlag_run_carbon_cascade;
  cLandcover.update_landcover_parameter(iLand_cover_type);

  cSnow.update_snow_parameter();
  cSurface_runoff.update_surface_runoff_parameter(iLand_cover_type, -1);
  return error_code;
}
// 50==================================================
/*!
  <long-description>

  \param dElevation_in
  \return <ReturnValue>
*/
// 50==================================================
int glacier::calculate_glacier_base_temperature(double dElevation_in)
{
  int error_code = 1;
  double dDifference;
  if (dElevation_in <= 2500)
    {
      dTemperature_glacier_melt_base = celsius_2_kelvin;
    }
  else
    {
      dDifference = dElevation_in - 2500.0;
      dTemperature_glacier_melt_base = celsius_2_kelvin - dDifference / 100.0;
    }
  return error_code;
}
// 50==================================================
/*!
  <long-description>

  \param iDay_ymd
  \param iFlag_transpiration
  \param iLand_cover_type
  \param iMonth_ymd
  \param iPrecipitation_type
  \param iStorm_type
  \param dElevation_in
  \param dLai_in
  \param dLatitude
  \param dPrecipitation_in
  \param dPrecipitation_basin_in
  \param dRain_in
  \param dShortwave_radiation_in
  \param dSlope
  \param dSnow_in
  \param dTemperature_basin_max_in
  \param dTemperature_dewpoint_in
  \param dTemperature_max_in
  \param dTemperature_mean_in
  \param dTemperature_min_in
  \param dVapor_pressure_deficit_in
  \return <ReturnValue>
*/
// 50==================================================
int glacier::run_glacier_model(
    int iDay_ymd, int iFlag_transpiration, int iMonth_ymd,
    int iPrecipitation_type, int iStorm_type, double dLai_in,

    double dPrecipitation_basin_in, double dRain_in,
    double dShortwave_radiation_in, double dSnow_in,

    double dTemperature_max_in, double dTemperature_mean_in,
    double dTemperature_min_in, double dTemperature_rain_in,
    double dTemperature_snow_in, double dVapor_pressure_deficit_in)
{
  int error_code = 1;
  int iLand_cover_type = cLandcover.iLand_cover_type;

  int iFlag_old_snow;
  int iFlag_has_snow;

  double dElevation = cGeology.dSurface_elevation;

  // 50==================================================

  double dEt;
  double dSca;
  double dSnowmelt;
  double dDifference_temperature;

  cEvapotranspiration.run_evapotranspiration_model(
      iMonth_ymd, dElevation,
      dShortwave_radiation_in, // radiation module
      dTemperature_mean_in, dVapor_pressure_deficit_in);
  dGlacier_pet = cEvapotranspiration.dPet_hru;
  dEt = dGlacier_pet;
  // glacier
  cSnow.run_snow_model(iDay_ymd, iFlag_transpiration, iLand_cover_type,
                       iMonth_ymd, iPrecipitation_type, iStorm_type, dElevation,
                       dGlacier_pet, dLai_in, dPrecipitation_basin_in, dRain_in,
                       dSnow_in, dShortwave_radiation_in, dTemperature_max_in,
                       dTemperature_mean_in, dTemperature_min_in,
                       dTemperature_rain_in, dTemperature_snow_in);
  iFlag_old_snow = cSnow.iFlag_old_snow;
  iFlag_has_snow = cSnow.iFlag_has_snow;
  dSca = cSnow.dSca;
  dSnowmelt = cSnow.dSnowmelt;
  dGwe = cSnow.dSwe;
  cSurface_runoff.run_surface_runoff_model(
      iFlag_old_snow, iFlag_has_snow, eCU_type, iLand_cover_type,
      iPrecipitation_type, dEt, dRain_in, dSca, dSnowmelt, dSnow_in,
      0, // system variable
      0, // soil proprities
      dTemperature_mean_in, 0, 0);
  calculate_glacier_base_temperature(dElevation);

  if (dGwe < tiny_value)
    {
      dGwe = 0.0;
      dGlacier_melt = 0.0;
    }
  else
    {
      dDifference_temperature =
          dTemperature_mean_in - dTemperature_glacier_melt_base;

      if (dDifference_temperature < near_zero)
        {
          // they are the same
          dGlacier_melt = 0.0;
        }
      else
        {
          if (dDifference_temperature <= -near_zero)
            {
              dGlacier_melt = 0.0;
            }
          else
            {
              dGlacier_melt = dDifference_temperature * dGwe * 0.00015;
              if (dGlacier_melt < tiny_value)
                {
                  dGlacier_melt = 0.0;
                }
            }
        }

      dGwe = dGwe - dGlacier_melt;
      if (dGwe < tiny_value)
        {
          dGwe = 0.0;
        }
    }

  return error_code;
}

int glacier::update_glacier_status()
{
  int error_code = 1;
  if (dGlacier_nep < 0.0)
    {
      iFlag_source_or_sink = 0;
    }
  else
    {
      iFlag_source_or_sink = 1;
    }

  return error_code;
}
} // namespace ecohydrology
