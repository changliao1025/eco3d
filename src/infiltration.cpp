// 50==================================================
/*!
  \file infiltration.cpp
  \brief
  <long description>
  \author Chang Liao
  \date 2017-01-30
*/
// 50==================================================
#include "infiltration.h"
namespace ecohydrology
{
// 50==================================================
/*!
      Default initialization
      \param void
*/
// 50==================================================
infiltration::infiltration()
{
  dInfiltration = missing_value;
  dInfiltration_hru = missing_value;
  dSurface_runoff_excess = missing_value;
}
infiltration::~infiltration() = default;
// 50==================================================
/*!
      <long-description>
      \param iColumn_type
      \return <ReturnValue>
*/
// 50==================================================
int infiltration::initialize_infiltration(eColumn_type eCU_type)
{
  int error_code = 1;
  switch (eCU_type)
    {
    case eCU_glacier:
      {
        // glacier
        dInfiltration = 0.0;
        dInfiltration_hru = 0.0;
        dSurface_runoff_excess = 0.0;
      }
      break;
    case eCU_lake:
      // lake
      break;
    case eCU_land:
      {
        // land
        dInfiltration = 0.0;
        dInfiltration_hru = 0.0;
        dSurface_runoff_excess = 0.0;
      }
      break;
    case eCU_stream: // stream
      break;
    case eCU_swale: // swale
      dInfiltration = 0.0;
      dInfiltration_hru = 0.0;
      dSurface_runoff_excess = 0.0;
      break;
    default:
      break;
    }
  return error_code;
}
// 50==================================================
/*!
      calculate the excessive surfae runoff rate
      \param dSoil_moisture_in: the current soil moisture, unit: meter
      \param dSoil_moisture_max_in: the maximum soil moisture , unit: meter
      \return <ReturnValue>
*/
// 50==================================================
int infiltration::calculate_excess_snowmelt_infiltration(
    double dSoil_moisture_in, double dSoil_moisture_max_in)
{
  int error_code = 1;
  double dCapacity;
  double dMoisture_available;
  double dDifference_flow;
  dMoisture_available = dSoil_moisture_max_in - dSoil_moisture_in;
  if (abs(dMoisture_available) < tiny_value)
    {
      // already full
      dCapacity = dSnow_infiltration_max;
      // there is still space
      dDifference_flow = dCapacity - dInfiltration;
      if (abs(dDifference_flow) < tiny_value)
        {
          // they are the same
          // nothing changed
          dSurface_runoff_excess = 0.0;
        }
      else
        {
          if (dDifference_flow <= -tiny_value)
            {
              dInfiltration = dCapacity;
              dSurface_runoff_excess = -dDifference_flow;
            }
          else
            {
              // no excess
              dSurface_runoff_excess = 0.0;
            }
        }
    }
  else
    {
      if (dMoisture_available <= -tiny_value)
        {
          // impossible
        }
      else
        {
          // there are space
          dCapacity = dMoisture_available + dSnow_infiltration_max;
          // there is still space
          dDifference_flow = dCapacity - dInfiltration;
          if (abs(dDifference_flow) < tiny_value)
            {
              // they are the same
              // nothing changed
              dSurface_runoff_excess = 0.0;
            }
          else
            {
              if (dDifference_flow <= -tiny_value)
                {
                  dInfiltration = dCapacity;
                  dSurface_runoff_excess = -dDifference_flow;
                }
              else
                {
                  // no excess
                  dSurface_runoff_excess = 0.0;
                }
            }
        }
    }
  return error_code;
}
// 50==================================================
/*!
      Calculate_infiltration
      \param iFlag_old_snow
      \param iHru_type
      \param iPrecipitation_type : the precipitation type from canopy
      \param dPervious_surface_percentage_in
      \param dRunoff_pervious_in
      \param dSoil_moisture_in
      \param dSoil_moisture_max_in
      \param dWater_available_pervious_in
      \return <ReturnValue>
*/
// 50==================================================
int infiltration::calculate_infiltration(int iFlag_old_snow,

                                         int iPrecipitation_type,
                                         double dPervious_surface_percentage_in,
                                         double dRunoff_pervious_in,
                                         double dSoil_moisture_in,

                                         double dSoil_moisture_max_in,
                                         double dWater_available_pervious_in)
{
  int error_code = 1;
  // if there is no snowpack, only the net precipitation from
  // canopy and upslope water is considered, snowmelt is negligible.
  if (iFlag_old_snow == 0)
    {
      // no previous snowpack,
      switch (iPrecipitation_type)
        {
        case 1: // only rain
          {
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
          }
          break;
        case 2: // only snow,
          {
            // there maybe snowmelt from current precipitation,
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
            calculate_excess_snowmelt_infiltration(dSoil_moisture_in,
                                                   dSoil_moisture_max_in);
          }
          break;
        case 3: // mixture
          {
            // all net rain is considered as snowmelt since rain is added to
            // snow
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
            calculate_excess_snowmelt_infiltration(dSoil_moisture_in,
                                                   dSoil_moisture_max_in);
          }
          break;
        default: // no precipitation
          {
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
          }
          break;
        }
    }
  else
    {
      ////there is old snowpack, in this case, all water is consider as snowmelt
      /// and the max snowmelt infiltration is applied
      switch (iPrecipitation_type)
        {
        case 1: // rain
          {
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
            calculate_excess_snowmelt_infiltration(dSoil_moisture_in,
                                                   dSoil_moisture_max_in);
          }
          break;
        case 2: // snow
          {
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
            calculate_excess_snowmelt_infiltration(dSoil_moisture_in,
                                                   dSoil_moisture_max_in);
          }
          break;
        case 3:
          {
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
            calculate_excess_snowmelt_infiltration(dSoil_moisture_in,
                                                   dSoil_moisture_max_in);
          }
          break;
        default: // no precipitation,
          {
            dInfiltration = dWater_available_pervious_in - dRunoff_pervious_in;
            calculate_excess_snowmelt_infiltration(dSoil_moisture_in,
                                                   dSoil_moisture_max_in);
          }
          break;
        }
    }
  if (dInfiltration < tiny_value)
    {
      dInfiltration = 0.0;
      // std::cout << " something is wrong in infiltration" << std::endl;
    }
  dInfiltration_hru = dInfiltration * dPervious_surface_percentage_in;
  return error_code;
}
// 50==================================================
/*!
      Initialize infiltration
      \param iSoil_type
      \return <ReturnValue>
*/
// 50==================================================
int infiltration::update_infiltration_parameter(int iSoil_type)
{
  int error_code = 1;

  if (iSoil_type < 1 || iSoil_type > nsoil_type)
    {
      // if it has no soil, infiltration is not calculated
    }
  else
    {
      dSnow_infiltration_max_base = aSnow_infiltration_max_base[iSoil_type - 1];
    }
  return error_code;
}
// 50==================================================
/*!
      Run infiltration module
      \param iFlag_old_snow
      \param iHru_type
      \param iPrecipitation_type
      \param dPervious_surface_percentage_in
      \param dRunoff_pervious_in
      \param dSoil_moisture_in
      \param dSoil_moisture_max_in
      \param dWater_available_pervious_in
      \return <ReturnValue>
*/
// 50==================================================
int infiltration::run_infiltration_model(int iFlag_old_snow,

                                         int iPrecipitation_type,
                                         double dPervious_surface_percentage_in,
                                         double dRunoff_pervious_in,
                                         double dSoil_moisture_in,
                                         double dSoil_moisture_max_in,
                                         double dSoil_temperature_in,
                                         double dWater_available_pervious_in)
{
  int error_code = 1;
  double dSoil_temperature_celsius = dSoil_temperature_in + kelvin_2_celsius;
  if (dSoil_temperature_celsius < small_value)
    {
      dSnow_infiltration_max = dSnow_infiltration_max_base;
    }
  else
    {
      dSnow_infiltration_max = dSnow_infiltration_max_base *
                               pow(2.0, (dSoil_temperature_celsius) / 10);
    }
  calculate_infiltration(iFlag_old_snow, iPrecipitation_type,
                         dPervious_surface_percentage_in, dRunoff_pervious_in,
                         dSoil_moisture_in, dSoil_moisture_max_in,
                         dWater_available_pervious_in);

  return error_code;
}
} // namespace ecohydrology
