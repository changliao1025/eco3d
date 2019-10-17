// 50==================================================
/*!
  \file surface_runoff.cpp
  \brief
  <long description>
  \author Chang Liao
  \date 2017-01-30
*/
// 50==================================================
#include "surface_runoff.h"

namespace ecohydrology
{
// 50==================================================
/*!
      \param void
*/
// 50==================================================
surface_runoff::surface_runoff()
{
  iFlag_infiltration = 1;
  dImpervious_surface_et = missing_value;
  dImpervious_surface_et_hru = missing_value;
  dImpervious_surface_percentage = missing_value;
  dPervious_surface_percentage = 1.0;
  dSurface_runoff_impervious = missing_value;
  dSurface_runoff_impervious_hru = missing_value;
  dSurface_runoff_pervious = missing_value;
  dSurface_runoff_pervious_hru = missing_value;
  dSurface_storage_impervious = missing_value;
  dSurface_runoff_storage = missing_value;
  dSurface_runoff_inflow = missing_value;
  dSurface_runoff_outflow = missing_value;
  dSurface_runoff_storage = missing_value;
  dSurface_runoff_inflow_external = missing_value;
  dSurface_runoff_inflow_upslope = missing_value;
  //  dSurface_runoff_outflow_dunnian = missing_value;
  dSurface_runoff_outflow_infiltration = missing_value;
  dSurface_runoff_outflow_downslope = missing_value;

  dSurface_et_hru = missing_value;
}

surface_runoff::~surface_runoff() {}

///
/// \param iColumn_type
/// \return
int surface_runoff::initialize_surface_runoff(eColumn_type eCU_type)
{
  int error_code = 1;
  switch (eCU_type)
    {
    case eCU_glacier: // glacier
      {
        iFlag_infiltration = 0;
        dImpervious_surface_et = 0.0;
        dImpervious_surface_et_hru = 0.0;
        dImpervious_surface_percentage = 1.0;
        dPervious_surface_percentage = 0.0;
        dSurface_runoff_impervious = 0.0;
        dSurface_runoff_impervious_hru = 0.0;
        dSurface_runoff_pervious = 0.0;
        dSurface_storage_impervious = 0.0;
        dSurface_runoff_storage = 0.0;
        dSurface_runoff_inflow = 0.0;
        dSurface_runoff_outflow = 0.0;
        dSurface_runoff_storage = 0.0;
        dSurface_runoff_inflow_external = 0.0;
        dSurface_runoff_inflow_upslope = 0.0;
        // dSurface_runoff_outflow_dunnian = 0.0;
        dSurface_runoff_outflow_infiltration = 0.0;
        dSurface_runoff_outflow_downslope = 0.0;

        dSurface_et_hru = 0.0;

        dSurface_runoff_pervious_hru = 0.0;
      }
      break;
    case eCU_lake: // lake
      {
        // not implemented yet
      }
      break;
    case eCU_land: // land
      {
        iFlag_infiltration = 1;
        dImpervious_surface_et = 0.0;
        dImpervious_surface_et_hru = 0.0;
        dImpervious_surface_percentage = 0.0;
        dPervious_surface_percentage = 1.0;
        dSurface_runoff_impervious = 0.0;
        dSurface_runoff_impervious_hru = 0.0;
        dSurface_runoff_pervious = 0.0;
        dSurface_runoff_pervious_hru = 0.0;
        dSurface_storage_impervious = 0.0;
        dSurface_runoff_storage = 0.0;
        dSurface_runoff_inflow = 0.0;
        dSurface_runoff_outflow = 0.0;
        dSurface_runoff_storage = 0.0;
        dSurface_runoff_inflow_external = 0.0;
        dSurface_runoff_inflow_upslope = 0.0;
        //  dSurface_runoff_outflow_dunnian = 0.0;
        dSurface_runoff_outflow_infiltration = 0.0;
        dSurface_runoff_outflow_downslope = 0.0;

        dSurface_et_hru = 0.0;
      }
      break;
    case eCU_stream: // stream
      {
      }
      break;
    case eCU_swale: // swale
      {
        dSurface_runoff_inflow_upslope = 0.0;
      }
      break;
    default:
      break;
    }
  // initialize the infiltration component
  error_code = cInfiltration.initialize_infiltration(eCU_type);
  if (error_code != 1)
    {
      // something is incorrect
    }
  return error_code;
}
// 50==================================================
/*!
      calculate the storage and et of the impervious surface
      \param dEt_available_in： available ET
      \param dSCA
      \param dWater_available_in
      \return <ReturnValue>
*/
// 50==================================================
int surface_runoff::calculate_impervious_surface_runoff(
    double dEt_available_in, double dSca_in, double dWater_available_in)
{
  int error_code = 1;
  if (dImpervious_surface_percentage <
      small_value) // no impervious area in this grid
    {
      dSurface_storage_impervious = 0.0;
      dSurface_runoff_impervious = 0.0;
      dImpervious_surface_et = 0.0;
    }
  else
    {

      error_code = calculate_impervious_surface_storage(dWater_available_in);

      error_code = calculate_impervious_surface_evapotranspiration(
          dEt_available_in, dSca_in, dWater_available_in);
      // remove et from storage
      dSurface_storage_impervious =
          dSurface_storage_impervious - dImpervious_surface_et;
      if (abs(dSurface_storage_impervious) < tiny_value)
        {
          dSurface_storage_impervious = 0.0;
        }
      else
        {
          if (dSurface_storage_impervious <= -tiny_value)
            {
              dSurface_storage_impervious = 0.0;
            }
        }
    }
  dSurface_storage_impervious_hru =
      dSurface_storage_impervious * dImpervious_surface_percentage;
  dSurface_runoff_impervious_hru =
      dSurface_runoff_impervious * dImpervious_surface_percentage;
  dImpervious_surface_et_hru =
      dImpervious_surface_et * dImpervious_surface_percentage;
  return error_code;
}
// 50==================================================
/*!
      calculate et from impervious surface
      \param dET_available
      \param dSCA
      \param dWater_available_in
      \return <ReturnValue>
*/
// 50==================================================
int surface_runoff::calculate_impervious_surface_evapotranspiration(
    double dEt_available_in, double dSca_in, double dWater_available_in)
{
  int error_code = 1;
  double dDifference;
  // because snow et was calculated in the snow module, the sca is used here to
  // only consider the et without snow cover
  if (dSca_in < 1.0)
    {
      if (dWater_available_in < tiny_value)
        {
          dImpervious_surface_et = 0.0;
        }
      else
        {
          if (dEt_available_in < tiny_value)
            {
              dImpervious_surface_et = 0.0;
            }
          else
            {
              dDifference = dWater_available_in - dEt_available_in;
              if (abs(dDifference) < tiny_value)
                {
                  dImpervious_surface_et = 0.0;
                }
              else
                {
                  if (dDifference <= -tiny_value)
                    {
                      dImpervious_surface_et =
                          dWater_available_in * (1.0 - dSca_in);
                    }
                  else
                    {
                      dImpervious_surface_et =
                          dEt_available_in * (1.0 - dSca_in);
                    }
                }
            }
        }
    }
  else
    {
      dImpervious_surface_et = 0.0;
    }
  return error_code;
}
// 50==================================================
/*!
      calculate storage
      \param dWater_available_in
      \return <ReturnValue>
*/
// 50==================================================
int surface_runoff::calculate_impervious_surface_storage(
    double dWater_available_in)
{
  int error_code = 1;
  double dDifference_storage;
  // the available water on impervious surface should be same with the average
  dSurface_storage_impervious =
      dSurface_storage_impervious + dWater_available_in; // intensity
  dDifference_storage = dStorage_impervious_max - dSurface_storage_impervious;
  if (abs(dDifference_storage) < tiny_value)
    {
      dSurface_runoff_impervious = 0.0;
    }
  else
    {
      if (dDifference_storage <= -tiny_value)
        {
          dSurface_storage_impervious =
              dStorage_impervious_max; // cannot exceed max storage depth
          // over land runoff
          dSurface_runoff_impervious = -dDifference_storage;
        }
      else
        {
          dSurface_runoff_impervious = 0.0;
          // available water added into storage
        }
    }
  return error_code;
}
// 50==================================================
/*!
      calculate_pervious_surface
      \param iFlag_old_snow
      \param iHru_type
      \param iPrecipitation_type
      \param dSoil_moisture_in
      \param dSoil_moisture_max_in
      \param dStorage_recharge_zone_in
      \param dStorage_recharge_zone_max_in
      \param dWater_available_pervious_in
      \return <ReturnValue>
*/
// 50==================================================
int surface_runoff::calculate_pervious_surface_runoff(
    int iFlag_old_snow, int iPrecipitation_type, double dSoil_moisture_in,
    double dSoil_moisture_max_in, double dStorage_recharge_zone_in,
    double dStorage_recharge_zone_max_in, double dTemperature_mean_in,
    double dWater_available_pervious_in)
{
  int error_code = 1;
  dContribution_area = calculate_variable_source_area(
      dStorage_recharge_zone_in, dStorage_recharge_zone_max_in);
  dSurface_runoff_pervious = dWater_available_pervious_in * dContribution_area;
  // run infiltration first
  cInfiltration.run_infiltration_model(
      iFlag_old_snow, iPrecipitation_type, dPervious_surface_percentage,
      dSurface_runoff_pervious, dSoil_moisture_in, dSoil_moisture_max_in,
      dTemperature_mean_in, dWater_available_pervious_in);
  // remove excess runoff
  dSurface_runoff_pervious =
      dSurface_runoff_pervious + cInfiltration.dSurface_runoff_excess;
  dSurface_runoff_pervious_hru =
      dSurface_runoff_pervious * dPervious_surface_percentage; // hru average

  return error_code;
}
// 50==================================================
/*!
      <long-description>
      \param dStorage_recharge_zone_in
      \param dStorage_recharge_zone_max_in
      \return <ReturnValue>
*/
// 50==================================================
double surface_runoff::calculate_variable_source_area(
    double dStorage_recharge_zone_in, double dStorage_recharge_zone_max_in)
{
  double dContribution_area_out =
      dContribution_area_min +
      (dContribution_area_max - dContribution_area_min) *
          dStorage_recharge_zone_in / dStorage_recharge_zone_max_in;
  this->dContribution_area = dContribution_area_out;
  return dContribution_area_out;
}
// 50==================================================
/*!
      Initialize the surface runoff module. The water land cover type (0) is
      excluded.
      \param iLand_cover_type
      \param iSoil_type
      \return <ReturnValue>
*/
// 50==================================================
int surface_runoff::update_surface_runoff_parameter(int iLand_cover_type,
                                                    int iSoil_type)
{
  int error_code = 1;

  if (iLand_cover_type >= 1)
    {
      dContribution_area_max = aContribution_area_max[iLand_cover_type - 1];
      dContribution_area_min = aContribution_area_min[iLand_cover_type - 1];
    }
  else
    {
      // if it is water or glacier, no pervious surface will be calculated
      dContribution_area_max = 1.0;
      dContribution_area_min = 0.8;
    }
  if (iSoil_type < 1 || iSoil_type > nsoil_type)
    {
      // if it has no soil, infiltration is not calculated
      iFlag_infiltration = 0;
    }
  else
    {
      if (iSoil_type == 13)
        {
          dContribution_area_max = 1.0;
          dContribution_area_min = 0.6;
        }
      iFlag_infiltration = 1;
      cInfiltration.update_infiltration_parameter(iSoil_type);
    }

  return error_code;
}
// 50==================================================
/*!
     <long-description>
     \param iFlag_old_snow
     \param iHru_type
     \param iLand_cover_type
     \param iPrecipitation_type
     \param dET_available_hru
     \param dRain_net_in
     \param dSCA
     \param dSnowmelt_in
     \param dSoil_moisture_in
     \param dSoil_moisture_max_in
     \param dStorage_recharge_zone_in
     \param dStorage_recharge_zone_max_in
     \return <ReturnValue>
*/
// 50==================================================
int surface_runoff::run_surface_runoff_model(
    int iFlag_old_snow, int iFlag_has_snow, eColumn_type eCU_type,
    int iLand_cover_type, int iPrecipitation_type, double dEt_available_hru_in,
    double dRain_net_in, double dSca_in, double dSnowmelt_in,
    double dSnow_net_in, double dSoil_moisture_in, double dSoil_moisture_max_in,
    double dSoil_temperature_in, double dStorage_recharge_zone_in,
    double dStorage_recharge_zone_max_in)
{
  int error_code = 1;
  double dEt_available_impervious = dEt_available_hru_in;
  double dWater_available_hru = 0.0;
  double dWater_available_impervious = 0.0;
  double dWater_available_pervious = 0.0;
  // prepare parameters

  if (iFlag_old_snow == 0) // no old snow
    {
      // there is no old snowpack
      if (iFlag_has_snow == 1)
        {
          // it could possible has new snow
          switch (iPrecipitation_type)
            {
            case 1: // only rain
              std::cout << "Impossible: only rain" << std::endl;
              break;
            case 2: // snow
              dSurface_runoff_inflow_external = dSnowmelt_in;
              break;
            case 3: // mixture
              // because rain is not added to snow
              dSurface_runoff_inflow_external = dSnowmelt_in + dRain_net_in;
              break;
            case 4: // no precipitation
              std::cout << "Impossible: no precipitation" << std::endl;
              break;
            default:

              break;
            }
        }
      else
        {
          // there is also no new snow formed
          // but it could be possible melt
          switch (iPrecipitation_type)
            {
            case 1: // only rain
              dSurface_runoff_inflow_external = dRain_net_in;
              break;
            case 2: // snow
              dSurface_runoff_inflow_external = dSnowmelt_in;
              break;
            case 3: // mixture
              // because rain is not added to snow
              dSurface_runoff_inflow_external = dRain_net_in + dSnowmelt_in;
              break;
            case 4: // no precipitation
              dSurface_runoff_inflow_external = 0.0;
              break;
            default:
              break;
            }
        }
    }
  else // there is old snow
    {

      switch (iPrecipitation_type)
        {
        case 1: // rain
          dSurface_runoff_inflow_external = dSnowmelt_in;
          break;
        case 2: // snow
          dSurface_runoff_inflow_external = dSnowmelt_in;
          break;
        case 3: // mixture
          dSurface_runoff_inflow_external = dSnowmelt_in;
          break;
        case 4: // no precipitation
          dSurface_runoff_inflow_external = dSnowmelt_in;
          break;
        default:
          dSurface_runoff_inflow_external = dSnowmelt_in;
          break;
        }
    }
  dSurface_runoff_inflow =
      dSurface_runoff_inflow_external + dSurface_runoff_inflow_upslope;

  dWater_available_hru = dSurface_runoff_storage + dSurface_runoff_inflow;
  if (dWater_available_hru < tiny_value)
    {
      dImpervious_surface_et_hru = 0.0;
      dSurface_runoff_outflow = 0.0;
      dSurface_runoff_outflow_downslope = 0.0;
      dSurface_runoff_outflow_infiltration = 0.0;
      cInfiltration.dInfiltration = 0.0;
      cInfiltration.dInfiltration_hru = 0.0;
      cInfiltration.dSurface_runoff_excess = 0.0;
    }
  else
    {
      // reset first
      dImpervious_surface_et_hru = 0.0;
      cInfiltration.dInfiltration_hru = 0.0;
      dWater_available_impervious = dWater_available_hru;
      dWater_available_pervious = dWater_available_hru;
      switch (eCU_type)
        {
        case eCU_glacier: // glacier
          // there are three type of surface components
          // 1: impervious
          calculate_impervious_surface_runoff(dEt_available_impervious, dSca_in,
                                              dWater_available_impervious);
          // 2: pervious
          dSurface_runoff_pervious_hru = 0.0;
          // run surface runoff

          // 3: depression, not enabled yet
          break;
        case eCU_lake: // lake
          break;
        case eCU_land: // land

          // there are three type of surface components
          // 1: impervious
          calculate_impervious_surface_runoff(dEt_available_impervious, dSca_in,
                                              dWater_available_impervious);
          // 2: pervious

          // run surface runoff
          if (0 == iFlag_infiltration)
            {
              dSurface_runoff_pervious_hru = 0.0;
            }
          else
            {
              calculate_pervious_surface_runoff(
                  iFlag_old_snow, iPrecipitation_type, dSoil_moisture_in,
                  dSoil_moisture_max_in, dStorage_recharge_zone_in,
                  dStorage_recharge_zone_max_in, dSoil_temperature_in,
                  dWater_available_pervious);
            }

          // 3: depression, not enabled yet
          break;
        case eCU_stream: // stream
          break;
        case eCU_swale: // swale
          break;
        default: // inactive
          break;
        }
      dSurface_et_hru = dImpervious_surface_et_hru;
      dSurface_runoff_hru =
          dSurface_runoff_impervious_hru + dSurface_runoff_pervious_hru;
      dSurface_runoff_outflow_downslope = dSurface_runoff_hru;

      dSurface_runoff_outflow_infiltration = cInfiltration.dInfiltration_hru;
      dSurface_runoff_outflow = dSurface_runoff_outflow_downslope +
                                dSurface_runoff_outflow_infiltration;
    }

  return error_code;
}
} // namespace ecohydrology
