// 50==================================================
/*!
  \file   snow.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-26
*/
// 50==================================================
#include "snow.h"

namespace ecohydrology
{
snow::snow()
{
  // initialize the snow system
  dSwe = missing_value;               // snow water equivlent, unit: meter
  dSwe_density = missing_value;       // swe density, unit: kg/meter^3
  dSwe_depth = missing_value;         // swe depth, unit: meter
  dSnow_heat_deficit = missing_value; // snowpack heat deficit, unit: joules
  dSwe_ice = missing_value;           // the ice content in swe, unit: meter
  dSwe_max = missing_value;           // the maximum swe, unit: meter
  dSwe_plus = missing_value;
  dSwe_previous = missing_value;
  dSwe_sub_max = missing_value;
  dSnow_temperature = celsius_2_kelvin;
  dSwe_water = missing_value; // free water stored in the snowpack, unit: meter
  dSnow_cumulative_save = missing_value;
  dAlbedo = missing_value;
  dSca = missing_value;
  dSca_previous = missing_value;
  dSnow_et = missing_value;
  dSnow_et_hru = missing_value;
  dSnowmelt = missing_value;
}

snow::~snow() {}

double snow::calculate_convection_heat_flux(int iMonth, int iLand_cover_type,
                                            double dPrecipitation_basin_in,
                                            double dTemperature_air_in)
{
  double dHeat_convection_out;
  double dConvection_coefficient = aConvection_coefficient[iMonth - 1] * 0.5;
  double dTemperature_air_celsius = dTemperature_air_in + kelvin_2_celsius;
  if (iFlag_has_snow == 1)
    {
      if (iLand_cover_type >= 1 && iLand_cover_type <= 5)
        {
          // if the land cover type is tree, reduce the above
          // convection-condensation by half
          dConvection_coefficient = dConvection_coefficient * 0.5;
        }

      // convection and condensation
      if (dTemperature_air_in > celsius_2_kelvin) // above 0.0 degree
        {
          if (dPrecipitation_basin_in > 0.001)
            {
              // double check this part of precipitation
              // precipitation higher than 1mm
              dHeat_convection_out =
                  dConvection_coefficient *
                  dTemperature_air_celsius; // the equation requires celsius,
              // covert from calorie to joule
              dHeat_convection_out = dHeat_convection_out * calorie_2_joule;
            }
          else
            {
              // nothing here
              dHeat_convection_out = 0.0;
            }
        }
      else
        {
          // nothing here
          dHeat_convection_out = 0.0;
        }
    }
  else
    {
      dHeat_convection_out = 0.0;
    }
  return dHeat_convection_out;
}

// 50==================================================
/*!
  Calculate effective longwave radiation
  \param iFlag_time: a flag for night or day
  \param iStorm_type: storm type
  \param dLai_in 0-10
  \param dPrecipitation_basin_in: the basin mean precipitation, unit: meter
  \param dTemperature_air_in: basin mean temperature, unit: kelvin
  \return <ReturnValue>
*/
// 50==================================================
double snow::calculate_effective_longwave_radiation(
    int iFlag_time, int iStorm_type, double dLai_in,
    double dPrecipitation_basin_in, double dTemperature_air_in)
{
  double dEmissivity_effective; // 0-1
  // total longwave joule per square meter
  double dLongwave_effective_incoming_out;
  double dLongwave_surface_snow_outgoing; // outgoing radiation
  double dLongwave_sky_incoming;          // incoming radiation
  double dLongwave_canopy_incoming;       // canopy radiation
  // 50==================================================
  // longwave equation
  // https://en.wikipedia.org/wiki/Stefan%E2%80%93Boltzmann_law  units: w m-2
  // K-4 , remember time is not considered here
  // 50==================================================
  double dLongwave_radiation =
      dStefan_boltzmann * (pow(dTemperature_air_in, 4.0));
  // when it is 0.0 celsius,
  //    double dLongwave_radiation_base = dStefan_boltzmann *
  //    (pow(celsius_2_kelvin, 4.0));
  dLongwave_surface_snow_outgoing =
      dStefan_boltzmann * (pow(dTemperature_surface_snow, 4.0));
  // emissivity depends on precipitation and storm
  if (dPrecipitation_basin_in > small_value)
    {
      // at least 1 mm precipitation
      if (iStorm_type == 2)
        {
          if (iFlag_time == 1)
            {
              // night
              dEmissivity_effective = 0.85;
            }
          else
            {
              // day
              dEmissivity_effective = 0.90;
            }
        }
      else
        {
          dEmissivity_effective = 1.0;
        }
    }
  else
    {
      // if there is no precipitation event, consider using another flag in
      // future version
      dEmissivity_effective = dEmissivity_no_precipitation;
    }
  // 50==================================================
  // air temperature consideration, calculate surface
  // temperature and incoming longwave radiation
  // 50==================================================
  // 50==================================================
  // canopy lai consideration
  // 50==================================================
  if (dLai_in < 1)
    {
      dLongwave_sky_incoming =
          (1.0 - dLai_in) * (dEmissivity_effective * dLongwave_radiation -
                             dLongwave_surface_snow_outgoing);
      dLongwave_canopy_incoming =
          dLai_in * (dLongwave_radiation - dLongwave_surface_snow_outgoing);
    }
  else
    {
      // all covered by canopy
      dLongwave_sky_incoming = 0.0;
      dLongwave_canopy_incoming =
          1.0 * (dLongwave_radiation - dLongwave_surface_snow_outgoing);
    }
  dLongwave_effective_incoming_out =
      dLongwave_sky_incoming + dLongwave_canopy_incoming;
  // 50==================================================
  // time consideration, only for half day
  // 50==================================================
  dLongwave_effective_incoming_out =
      dLongwave_effective_incoming_out * 12.0 * 3600.0;
  return dLongwave_effective_incoming_out;
}
// 50==================================================
/*!
  Calculate effective shortwave radiation
  \param dShortwave_radiation_in unit: joule per square meter per day
  \return <ReturnValue>
*/
// 50==================================================
double
snow::calculate_effective_shortwave_radiation(double dShortwave_radiation_in)
{
  // need to check albedo?
  double dShortwave_effective_incoming_out = dShortwave_radiation_in *
                                             (1.0 - dAlbedo_previous) *
                                             dTransmission_coefficient;
  if (dShortwave_effective_incoming_out < 0.001)
    {
      dShortwave_effective_incoming_out = 0.0;
    }
  else
    {
      // nothing
    }
  return dShortwave_effective_incoming_out;
}

double snow::calculate_night_radiation(int iLand_cover_type, int iMonth_ymd,
                                       int iStorm_type, double dLai_in,
                                       double dPrecipitation_basin_in,
                                       double dTemperature_night_in)
{
  double dHeat_total = 0.0;
  double dShortwave_effective_incoming = 0.0;
  double dLongwave_effective_incoming = 0.0;
  double dConvection_heat;
  if (iFlag_has_snow == 1)
    {
      if (dTemperature_night_in < celsius_2_kelvin)
        {
          // if the air temperature is below 0.0, snow can emitt radiation at
          // the same temperature
          dTemperature_surface_snow = dTemperature_night_in;
        }
      else
        {
          // or else, if the air temperature is above 0.0,
          // but the snow temperature is set to 0.0 celsius because it cannot be
          // higher than 0.0
          dTemperature_surface_snow = celsius_2_kelvin;
        }

      dConvection_heat = calculate_convection_heat_flux(
          iMonth_ymd, iLand_cover_type, dPrecipitation_basin_in,
          dTemperature_night_in);
      // longwave
      dLongwave_effective_incoming = calculate_effective_longwave_radiation(
          0, iStorm_type, dLai_in, dPrecipitation_basin_in,
          dTemperature_night_in);
      // there is no incoming shortwave
      dShortwave_effective_incoming = 0.0;
      dHeat_total = dShortwave_effective_incoming +
                    dLongwave_effective_incoming + dConvection_heat;
    }
  else
    {
      dHeat_total = 0.0;
    }

  return dHeat_total;
}

double snow::calculate_day_radiation(int iLand_cover_type, int iMonth_ymd,
                                     int iStorm_type, double dLai_in,
                                     double dPrecipitation_basin_in,
                                     double dShortwave_radiation_in,
                                     double dTemperature_day_in)
{
  double dHeat_total = 0.0;

  double dLongwave_effective_incoming = 0.0;
  double dShortwave_effective_incoming = 0.0;
  double dConvection_heat = 0.0;
  if (iFlag_has_snow == 1)
    {
      if (dTemperature_day_in < celsius_2_kelvin)
        {
          // if the air temperature is below 0.0, snow can emitt radiation at
          // the same temperature
          dTemperature_surface_snow = dTemperature_day_in;
        }
      else
        {
          // or else, if the air temperature is above 0.0,
          // but the snow temperature is set to 0.0 celsius because it cannot be
          // higher than 0.0
          dTemperature_surface_snow = celsius_2_kelvin;
        }
      dConvection_heat = calculate_convection_heat_flux(
          iMonth_ymd, iLand_cover_type, dPrecipitation_basin_in,
          dTemperature_day_in);
      // longwave
      dLongwave_effective_incoming = calculate_effective_longwave_radiation(
          0, iStorm_type, dLai_in, dPrecipitation_basin_in,
          dTemperature_day_in);
      // shortwave
      dShortwave_effective_incoming =
          calculate_effective_shortwave_radiation(dShortwave_radiation_in);

      dHeat_total = dShortwave_effective_incoming +
                    dLongwave_effective_incoming + dConvection_heat;
    }
  else
    {
      dHeat_total = 0.0;
    }

  return dHeat_total;
}

// 50==================================================
/*!
  Calculate sca on curve
  \param
  \return <ReturnValue>
*/
// 50==================================================
int snow::calculate_sca_on_curve()
{
  int error_code = 1;
  int iIndex;
  int jIndex;
  double af;
  double dify;
  double difx;
  double dFraction = dSwe / dSwe_max;
  iIndex = int(10.0 * (dFraction + 0.2));
  jIndex = iIndex - 1;
  af = double(jIndex - 1);
  dify = (dFraction * 10.0) - af;
  if (iIndex < 1)
    {
      iIndex = 1;
    }
  if (jIndex < 1)
    {
      jIndex = 1;
    }
  if (iIndex > 11)
    {
      iIndex = 11;
    }
  if (jIndex > 11)
    {
      jIndex = 11;
    }
  difx = aSCA_curve[iIndex - 1] - aSCA_curve[jIndex - 1];
  dSca = aSCA_curve[jIndex - 1] + dify * difx;
  return error_code;
}
// 50==================================================
/*!
  Calculate the snow albedo
  \param iPrecipitation_type
  \param dRain_percentage_in fraction of rain in precipitation
  \param dSnow_in unit meter
  \return <ReturnValue>
*/
// 50==================================================
int snow::calculate_snow_albedo(int iPrecipitation_type,
                                double dRain_percentage_in, double dSnow_in)
{
  int error_code = 1;

  int iDay;
  // two options
  if (iFlag_new_snow == 0)
    {
      // no new snow
      if (iFlag_partly_reset_albedo == 0) //
        {
          // not reset at all
        }
      else
        {
          // too less snow to reset the albedo, this is during melt season
          iDay_since_last_snow = iDay_since_last_reset - 3;
          if (iDay_since_last_snow < 1)
            {
              iDay_since_last_snow = 1;
            }
          // reset shallow snow
          iFlag_partly_reset_albedo = 0;
          dSnow_cumulative_save = 0.0;
        }
    }
  else
    {
      // there is new snow
      // two options
      if (iFlag_melt_season_force == 0)
        {
          // in accumulate season
          switch (iPrecipitation_type)
            {
            case 1:
              // all rain? rain day in winter
              iDay_since_last_snow = 0;
              iFlag_partly_reset_albedo = 0;
              break;
            case 2:
              // all snow, always reset the albedo
              iDay_since_last_snow = 0;
              iFlag_partly_reset_albedo = 0;
              break;
            case 3: // mixture event
              if (dRain_percentage_in >= dFaction_rain_max_accumulate)
                {
                  iFlag_partly_reset_albedo = 0;
                }
              else
                {
                  if (dSnow_in >= dSnow_reset_threshold_accumulate)
                    {
                      iDay_since_last_snow = 0;
                      iFlag_partly_reset_albedo = 0;
                    }
                  else
                    {
                      iDay_since_last_snow = iDay_since_last_snow - 3;
                      if (iDay_since_last_snow < 0)
                        {
                          iDay_since_last_snow = 0;
                        }
                      if (iDay_since_last_snow > 5)
                        {
                          iDay_since_last_snow = 5;
                        }
                      iFlag_partly_reset_albedo = 0;
                    }
                }
              break;
            default:
              break;
            }
          dSnow_cumulative_save = 0.0;
        } // end of accumulate season
      else
        {
          // melt season
          if (dRain_percentage_in <= dFaction_rain_max_melt)
            {
              // rain must be less than a fraction in order to reset the snow
              // dAlbedo
              if (dSnow_in >= dSnow_reset_threshold_melt) // completely reset
                {
                  iDay_since_last_snow = 0;
                  iFlag_partly_reset_albedo = 0;
                  dSnow_cumulative_save = 0.0;
                }
              else
                {
                  // not enough snow to reset yet
                  dSnow_cumulative_save = dSnow_cumulative_save + dSnow_in;
                  if (dSnow_cumulative_save >= dSnow_reset_threshold_melt)
                    {
                      iDay_since_last_snow = 0;
                      iFlag_partly_reset_albedo = 0;
                      dSnow_cumulative_save = 0.0;
                    }
                  else
                    {
                      // If the accumulated shallow snow is still not enough to
                      // reset the dAlbedo curve
                      if (iFlag_partly_reset_albedo == 0)
                        {
                          iDay_since_last_reset = iDay_since_last_snow;
                        }
                      else
                        {
                          //
                        }
                      iDay_since_last_snow = 0;
                      iFlag_partly_reset_albedo = 1;
                    }
                }
            } // end of rain fraction
          else
            {
              //
            }
        } // end of melt season
    }     // end of there is new snow
  iDay = lround(iDay_since_last_snow + 0.5);
  iDay_since_last_snow = iDay_since_last_snow + 1;
  if (iDay > 0)
    {
      // albedo is only different when day pass since reset
      if (iFlag_albedo_method == 0) // accumulate season
        {
          if (iDay <= iDay_albedo_max)
            {
              dAlbedo = aAlbedo_accumulate[iDay - 1];
            }
          else
            {
              iDay = iDay - 12;
              if (iDay > iDay_albedo_max)
                {
                  iDay = iDay_albedo_max;
                }
              dAlbedo = aAlbedo_melt[iDay - 1];
            }
        }
      else
        {
          // melt season
          if (iDay > iDay_albedo_max)
            {
              iDay = iDay_albedo_max;
            }
          dAlbedo = aAlbedo_melt[iDay - 1];
        }
    }
  else
    {
      // new snow reset the albedo
      if (iFlag_melt_season_force == 0) // acumulate
        {
          dAlbedo = 0.91;
          iFlag_albedo_method = 0;
        }
      else
        {
          // melt
          dAlbedo = 0.72;
          iFlag_albedo_method = 1;
        }
    }
  return error_code;
}
// 50==================================================
/*!
  Calculate the snow cover area
  \param iFlag_new_snow
  \param dSnow_in unit: meter
  \return <ReturnValue>
*/
// 50==================================================
int snow::calculate_snow_cover_area(int iFlag_new_snow, double dSnow_in)
{
  int error_code = 1;
  // 50==================================================
  // to calculate to SCA, the previous status must be stored and flaged.
  // two methods are used to calculate the new snow cover area,
  // on the depletion cureve
  // (0) and interpolation (1). set the default to 0.
  // There are 3 potential conditions for the snow area curve:
  // A. snow is accumulating and the pack is currently at its
  // maximum level,
  // B. snow is depleting and the area is determined by the
  // snow area curve
  // C. new snow has occured on a depleting pack, temporarily
  // resetting to 100% cover.
  // For case (C), the snow covered area is linearly interpolated
  // between 100% and the snow covered area before the new snow.
  // In general, 1/4 of the new snow has to melt before the snow
  // covered area goes below 100%, and then the remaining 3/4 has
  // to melt to return to the previous snow covered area.
  // First, the code decides whether snow is accumulating (A)
  // or not (B/C).
  // 50==================================================
  // temperoal variables
  // 50==================================================
  double dSca_difference;
  double dSwe_difference;
  double dFraction;
  // it is all about memory,
  // max 100, sub 70, now 80? max is unknown, but threshold 120 is known
  if (dSwe > dSwe_sub_max)
    {
      // if swe is still increasing, replace the max swe
      dSwe_sub_max = dSwe;
    }
  dSwe_max = dSwe_sub_max; // update the global max swe
  if (dSwe_max > dSwe_threshold)
    {
      // global max can't be larger than threshold
      dSwe_max = dSwe_threshold;
    }
  if (dSwe >= dSwe_max)
    {
      // case A, swe increase to the maximum
      // sca can't increase anymore
      iFlag_sca_on_curve = 1;
      dSca = aSCA_curve[10];
    }
  else
    {
      // swe is not at the global max value, so it is either case B or C
      if (iFlag_new_snow == 0)
        {
          // no new snow
          if (iFlag_sca_on_curve == 0)
            {
              // it is irregular
              if (dSwe > dSwe_plus)
                {
                  // less than 0.25 of dSnow_in has melted, so no change
                  error_code = 1;
                }
              else
                {
                  // more than 0.25 has melted
                  // but not sure whether it has returned to the original curve
                  // or not
                  if (dSwe >= dSwe_previous)
                    {
                      // swe is still large than before, so interpolation is
                      // needed
                      dSca_difference = dSca - dSca_previous;
                      dSwe_difference =
                          dSwe_plus -
                          dSwe_previous; // plus alway large than previous
                      if (dSwe_difference >= 0.01)
                        {
                          dFraction = (dSwe - dSwe_previous) / dSwe_difference;
                        }
                      else
                        {
                          dFraction = 0.0;
                        }
                      dSca = dSca_previous + dFraction * dSca_difference;
                    }
                  else
                    {
                      // it has returned to origianl regular place on the curve.
                      iFlag_sca_on_curve = 1;
                      // call curve function
                      calculate_sca_on_curve();
                    }
                }
            }
          else
            {
              // it is case B
              // if gets here, it means the curve method is needed.
              calculate_sca_on_curve();
            }
        }
      else
        {
          // there is new snow and swe is accumulating
          // even there is new snow, there are two options.
          // because the previous state may be on the curve or off the curve.
          if (iFlag_sca_on_curve == 1)
            {
              // The previous snow area is regular, and the state needs to be
              // saved
              dSwe_plus = 0.75 * dSnow_in; // 0.75 of the dSnow_in is added
              iFlag_sca_on_curve = 0; // sca will be off the curve next time
            }
          else
            {
              // the previous sca is irregular
              // keep going
              dSwe_plus =
                  dSwe_plus + (0.75 * dSnow_in); // add new snow to the swe
            }
        }
    }
  return error_code;
}

double snow::calculate_snow_conduction_heat_flux()
{
  double dHeat_conduction_out = 0.0; // units: joule per meter per day
  // convert from kg per cubic meter to g per cubic centimeter
  double dSwe_density_prms =
      dSwe_density * kilogram_per_cubic_meter_2_gram_per_cubic_centimeter;
  // since unit in this empirical equation is unknown, use interface conversion
  // here
  double dConductivity_effective =
      0.0077 * pow(dSwe_density_prms, 2.0); // unit: [calorie / (sec g degC)]
  // convert unit from joule kg kelvin to calorie/ (g * kelvin )
  double dSpecific_heat_ice_prms =
      dSpecific_heat_ice * gram_2_kilogram * joule_2_calorie;
  // calcualte heat conductivity
  // See "Development and testing of snow pack energy balance equations"
  double dummy0 = 2 * dConductivity_effective;
  double dummy1 = sqrt(12 * hour_2_second); // half day
  double dummy2 = pi * dConductivity_effective;
  double dummy3 = dSpecific_heat_ice_prms * dSwe_density_prms;
  double dummy4 = sqrt(dummy2 / dummy3);
  double dHeat_conductivity = dummy0 * dummy1 / dummy4;
  // convert from [cal/(cm^2 degC)] to joule per square meter kelvin
  dHeat_conductivity =
      dHeat_conductivity * calorie_2_joule * square_meter_2_square_centimeter;
  // calcualte heat conduction
  // dHeat_conduction_out, units: joule per square meter per half day
  dHeat_conduction_out =
      dHeat_conductivity * (dTemperature_surface_snow - dSnow_temperature);
  if (abs(dHeat_conduction_out) < near_zero)
    {
      dHeat_conduction_out = 0.0;
    }
  return dHeat_conduction_out;
}

double snow::calculate_snow_convection_heat_flux(int iMonth,
                                                 int iLand_cover_type,
                                                 double dPrecipitation_basin_in,
                                                 double dTemperature_air_in)
{
  double dHeat_convection_out;
  double dConvection_coefficient = aConvection_coefficient[iMonth - 1] * 0.5;
  double dTemperature_air_celsius = dTemperature_air_in + kelvin_2_celsius;
  if (iLand_cover_type >= 1 && iLand_cover_type <= 5)
    {
      // if the land cover type is tree, reduce the above
      // convection-condensation by half
      dConvection_coefficient = dConvection_coefficient * 0.5;
    }
  else
    {
      // nothing changes
    }
  // convection and condensation
  if (dTemperature_air_in > celsius_2_kelvin) // above 0.0 degree
    {
      if (dPrecipitation_basin_in > 0.001)
        {
          // double check this part of precipitation
          // precipitation higher than 1mm
          dHeat_convection_out =
              dConvection_coefficient *
              dTemperature_air_celsius; // the equation requires celsius,
          // covert from calorie to joule
          dHeat_convection_out = dHeat_convection_out * calorie_2_joule;
        }
      else
        {
          // nothing here
          dHeat_convection_out = 0.0;
        }
    }
  else
    {
      // nothing here
      dHeat_convection_out = 0.0;
    }
  return dHeat_convection_out;
}
// 50==================================================
/*!
  Calculate the snow depth
  \param dSnow_in
  \return <ReturnValue>
*/
// 50==================================================
int snow::update_snow_depth_and_density(double dSnow_in)
{
  int error_code = 1;
  double dMass;                          // kg
  double dVolume;                        // cubic meter
  double dThickness;                     // meter
  double dSwe_depth_before_settle = 0.0; // meter
  double dSwe_depth_after_settle = 0.0;  // meter
  dSwe_previous = dSwe_previous + dSnow_in;
  // calculate the total mass of snow,
  // remember that new snow density of the same as rain 1000kg m-3
  dVolume = dSnow_in * dArea;
  dMass = dDensity_new_snow * dVolume; // kg m-3 * m * m2 = kg
  // afterwards we will use the new snowpack density
  dVolume = dMass / dSwe_density_init; // kg/ (kgm-3) = m3
  dThickness = dVolume / dArea;        // m3 / m2 = m
  dSwe_depth_before_settle = dSwe_depth + dThickness;
  dVolume = dSwe_previous * dArea;
  dMass = dDensity_water * dVolume;
  dSwe_depth_after_settle = (dMass / dSwe_density_max) / dArea;
  dSwe_depth =
      dSwe_depth_before_settle +
      dSettle_const * (dSwe_depth_after_settle - dSwe_depth_before_settle);

  dVolume = dSwe * dArea;
  dMass = dDensity_water * dVolume;
  dVolume = dSwe_depth * dArea;
  dSwe_density = dMass / dVolume; // kg / ( m * m2) = kgm-3

  return error_code;
}
// 50==================================================
/*!
  Snow ET
  \param iFlag_transpiration_status
  \param iLand_cover_type
  \param dHru_pet_in meter
  \return <ReturnValue>
*/
// 50==================================================
int snow::calculate_snow_evapotranspiration(int iFlag_transpiration_status,
                                            int iLand_cover_type,
                                            double dHru_pet_in)
{
  int error_code = 1;
  int iFlag_et;
  // 50==================================================
  // temporal variables
  // 50==================================================
  double dET_available = dHru_pet_in;
  double dDifference_ET;
  double dWater_difference;
  if (dET_available < tiny_value)
    {
      dSnow_et = 0.0;
      dSnow_et_hru = 0.0;
    }
  else
    {

      if (iFlag_transpiration_status == 0)
        {
          iFlag_et = 1;
        }
      else
        {
          if (iLand_cover_type == 0 || iLand_cover_type == 10 ||
              iLand_cover_type == 13 || iLand_cover_type > 14)
            {
              iFlag_et = 1;
            }
          else
            {
              iFlag_et = 0;
            }
        }
      if (iFlag_et == 1)
        {

          dDifference_ET = dET_available - dSwe;
          if (abs(dDifference_ET) < tiny_value)
            {
              dSnow_et = dSwe;
              dSnow_et_hru = dSnow_et * dSca;
              dSwe = 0.0;
              update_snow_status();
            }
          else
            {
              if (dDifference_ET <= -tiny_value)
                {
                  // only a portion
                  dSnow_et = dET_available;
                  dSnow_et_hru = dSnow_et * dSca;

                  // we first take free water

                  if (dSwe_water > tiny_value)
                    {
                      // check whether freewater is enough
                      dWater_difference = dSnow_et - dSwe_water;
                      if (abs(dWater_difference) < tiny_value)
                        {
                          // just enough
                          dSwe_water = 0.0;
                          dSwe = dSwe_ice + dSwe_water;
                          dSnow_heat_deficit = 0.0;
                          dSnow_temperature = celsius_2_kelvin;
                        }
                      else
                        {
                          if (dWater_difference < -tiny_value)
                            {
                              // free water is enough
                              dSwe_water = -dWater_difference;
                              dSwe = dSwe_ice + dSwe_water;
                              dSnow_heat_deficit = 0.0;
                              dSnow_temperature = celsius_2_kelvin;
                            }
                          else
                            {
                              // not enough
                              dSwe_water = 0.0;
                              dSwe_ice = dSwe_ice - dWater_difference;
                              dSwe = dSwe_ice + dSwe_water;
                              dSnow_heat_deficit = 0.0;
                              dSnow_temperature = celsius_2_kelvin;
                            }
                        }
                    }
                  else
                    {
                      // no freewater
                      dSwe_water = 0.0;
                      dSwe_ice = dSwe_ice - dSnow_et;
                      dSwe = dSwe_ice + dSwe_water;
                      dSnow_temperature_celsius =
                          dSnow_temperature + kelvin_2_celsius;

                      dSnow_heat_deficit = dSnow_temperature_celsius * dSwe *
                                           dDensity_water * dSpecific_heat_ice;
                    }
                }
              else
                {
                  dSnow_et = dSwe;
                  dSnow_et_hru = dSnow_et * dSca;
                  dSwe = 0.0;
                  update_snow_status();
                }
            }
        }
      else
        {
          // transpiration not started yet
          dSnow_et = 0.0;
          dSnow_et_hru = 0.0;
        }
    }
  update_snow_status();
  return error_code;
}

// 50==================================================
/*!
  Determine whether melt season start or not based on date
  \param iDay_ymd day of the day
  \return <ReturnValue>
*/
// 50==================================================
int snow::determine_accumulate_or_melt_season(int iDay_doy)
{
  int error_code = 1;
  if (iDay_doy < 80)
    {
      // if it is still winter time
      iFlag_melt_season_potential = 0;
      iFlag_melt_season_force = 0;
    }
  else
    {
      iFlag_melt_season_potential = 1;
      if (iDay_doy < 140)
        {
          // this is a very arbitrary parameter so far
          iFlag_melt_season_force = 0;
        }
      else
        {
          iFlag_melt_season_force = 1;
        }
    }
  return error_code;
}
// 50==================================================
/*!
  determine elevation based parameters
  \param dElevation_in: surface elevation, unit: meter
  \return <ReturnValue>
*/
// 50==================================================
int snow::determine_elevation_based_parameter(double dElevation_in)
{
  int error_code = 1;
  if (dElevation_in < 1000.0)
    {
      // in lowlands
      dSwe_threshold = 0.1; // unit: meter
    }
  else
    {
      if (dElevation_in < 2000.0)
        {
          dSwe_threshold = 0.15;
        }
      else
        {
          if (dElevation_in < 2500.0)
            {
              dSwe_threshold = 0.2;
            }
          else
            {
              dSwe_threshold = 0.8;
              if (dElevation_in < 3500.0)
                {
                  dSwe_threshold = 1.0;
                }
              else
                {
                  // pamenent galcier
                  dSwe_threshold = 3.0;
                  iFlag_melt_season_force = 0;
                }
            }
        }
    }
  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iColumn_type
  \return <ReturnValue>
*/
// 50==================================================
int snow::initialize(eColumn_type eCU_type)
{
  int error_code = 1;
  switch (eCU_type)
    {
    case eCU_glacier: // glacier
      {
        iFlag_old_snow = 0;
        iFlag_has_snow = 0;
        iFlag_new_snow = 0;
        iDay_since_last_snow = 0;
        iFlag_sca_on_curve = 1;
        iDay_since_last_reset = 0;
        iFlag_partly_reset_albedo = 0;
        iCount_above_zero_day = 0;
        iFlag_albedo_method = 0;
        dSwe = 0.0;               // snow water equivlent, unit: meter
        dSwe_density = 0.0;       // swe density, unit: kg/meter^3
        dSwe_depth = 0.0;         // swe depth, unit: meter
        dSnow_heat_deficit = 0.0; // snowpack heat deficit, unit: joules
        dSwe_ice = 0.0;           // the ice content in swe, unit: meter
        dSwe_max = 0.0;           // the maximum swe, unit: meter
        dSwe_plus = 0.0;
        dSwe_previous = 0.0;
        dSwe_sub_max = 0.0;
        dSnow_temperature = celsius_2_kelvin;
        dSwe_water = 0.0;
        dSnow_cumulative_save = 0.0;
        dAlbedo = 0.0;
        dSca = 0.0;
        dSca_previous = 0.0;
        dSnow_et = 0.0;
        dSnow_et_hru = 0.0;
        dSnowmelt = 0.0;
      }
      break;
    case eCU_lake: // lake
      break;
    case eCU_land: // land
      {
        iFlag_old_snow = 0;
        iFlag_has_snow = 0;
        iFlag_new_snow = 0;
        iDay_since_last_snow = 0;
        iFlag_sca_on_curve = 1;
        iDay_since_last_reset = 0;
        iFlag_partly_reset_albedo = 0;
        iCount_above_zero_day = 0;
        iFlag_albedo_method = 0;
        dSwe = 0.0;               // snow water equivlent, unit: meter
        dSwe_density = 0.0;       // swe density, unit: kg/meter^3
        dSwe_depth = 0.0;         // swe depth, unit: meter
        dSnow_heat_deficit = 0.0; // snowpack heat deficit, unit: joules
        dSwe_ice = 0.0;           // the ice content in swe, unit: meter
        dSwe_max = 0.0;           // the maximum swe, unit: meter
        dSwe_plus = 0.0;
        dSwe_previous = 0.0;
        dSwe_sub_max = 0.0;
        dSnow_temperature = celsius_2_kelvin;
        dSwe_water = 0.0; // free water stored in the snowpack, unit: meter
        dSnow_cumulative_save = 0.0;
        dAlbedo = 0.0;
        dSca = 0.0;
        dSca_previous = 0.0;
        dSnow_et = 0.0;
        dSnow_et_hru = 0.0;
        dSnowmelt = 0.0;
      }
      break;
    case eCU_stream:
      break;
    case eCU_swale:
      break;
    default:
      break;
    }
  return error_code;
}

int snow::adjust_snowpack_freewater()
{
  int error_code = 1;
  double dFreewater_capacity; // the maximum water that can be stored in the
  // snowpack
  double dFreewater_difference; // the difference

  dFreewater_capacity = dSwe_ice * dFreewater_capacity_coefficient;
  // the difference between actual free water and potential capacity
  dFreewater_difference = dFreewater_capacity - dSwe_water;
  if (abs(dFreewater_difference) < tiny_value)
    {
      // just enough to hold it
      // nothing will change
      dSwe_water = dFreewater_capacity;
      dSnowmelt = 0.0;
    }
  else
    {
      if (dFreewater_difference < -tiny_value)
        {
          // cannot hold the water, so free water will go out
          dSwe_water = dFreewater_capacity;
          // ice content remain the same

          // add to snowmelt
          dSnowmelt = -dFreewater_difference;
          // calculate snowpack density

          // even though there is no new snow, the density will change due
          // to snowmelt

          dSwe_depth = dSwe * dDensity_water / dSwe_density;
        }
      else
        {
          // capacity is much larger, so it can hold
          // nothing will change
          dSnowmelt = 0.0;
        }
    }
  dSwe = dSwe_ice + dSwe_water;
  dSnow_heat_deficit = 0.0;
  dSnow_temperature = celsius_2_kelvin;
  return error_code;
}

int snow::freeze_snowpack_freewater(double dTemperature_air_in)
{
  int error_code = 1;
  double dWater_freeze_potential;
  double dWater_freeze_difference;
  // no additional heat, so heat deficit may freeze incoming
  // rain
  dWater_freeze_potential =
      -dSnow_heat_deficit / dLatent_heat_water / dDensity_water;
  dWater_freeze_difference = dWater_freeze_potential - dSwe_water;
  if (abs(dWater_freeze_difference) < tiny_value)
    {
      dSwe_ice = dSwe_ice + dWater_freeze_potential;
      dSwe_water = 0.0;
      dSwe = dSwe_ice + dSwe_water;
      dSnow_temperature = celsius_2_kelvin;
      dSnow_heat_deficit = 0.0;
      dSnowmelt = 0.0;
      update_snow_depth_and_density(dWater_freeze_potential);
    }
  else
    {
      if (dWater_freeze_difference < -tiny_value)
        {
          // too much rain, it cannot freeze all water
          dSwe_ice = dSwe_ice + dWater_freeze_potential;
          dSwe_water = dSwe_water - dWater_freeze_potential;
          adjust_snowpack_freewater();
        }
      else
        {
          // too less rain, all will be frozen
          dSwe_ice = dSwe_ice + dSwe_water;
          dSwe_water = 0.0;
          dSwe = dSwe_ice + dSwe_water;
          dSnow_temperature = celsius_2_kelvin;
          dSnow_heat_deficit =
              -dWater_freeze_difference * dLatent_heat_water * dDensity_water;
          if (abs(dSnow_heat_deficit) < tiny_value)
            {
              dSnow_heat_deficit = 0.0;
            }
          dSnowmelt = 0.0;
          update_snow_temperature(dTemperature_air_in);
          update_snow_depth_and_density(dSwe_water);
        }
    }
  return error_code;
}

int snow::melt_snowpack_ice(double dHeat_in)
{
  int error_code = 1;
  double dSnowmelt_potential;
  double dSwe_ice_difference;
  // not all become snowmelt
  dSnowmelt_potential = dHeat_in / dLatent_heat_water / dDensity_water;
  dSwe_ice_difference = dSnowmelt_potential - dSwe_ice;

  if (abs(dSwe_ice_difference) < tiny_value)
    {
      dSnowmelt = dSwe_ice + dSwe_water;
      dSwe = 0.0;
      update_snow_status();
    }
  else
    {
      if (dSwe_ice_difference < -tiny_value)
        {
          dSwe_ice = dSwe_ice - dSnowmelt_potential;
          dSwe_water = dSwe_water + dSnowmelt_potential;
          adjust_snowpack_freewater();
        }
      else
        {
          // all melt as well
          dSnowmelt = dSwe_ice + dSwe_water;
          dSwe = 0.0;
          update_snow_status();
        }
    }
  return error_code;
}
// 50==================================================
/*!
  Run snow module
  \param iDay_doy day of the year
  \param iFlag_transpiration the flag whether plant transpriation occurs or not
  \param iLand_cover_type land cover type
  \param iMonth_ymd month of the year
  \param iPrecipitation_type the precipitation type
  \param iStorm_type storm type
  \param dElevation_in elevation, meter
  \param dPET_hru_in potential et, unit: meter
  \param dLai_in lai from modis 0-10
  \param dPrecipitation_basin_in basin average precipitation unit meter
  \param dRain_net_in net rain unit meter
  \param dSnow_net_in net snow unit meter
  \param dShortwave_radiation_in radiation, joule per suqare meter per day
  \param dTemperature_max_in temperature kelvin
  \param dTemperature_mean_in temperature kelvin
  \param dTemperature_min_in temperature kelvin
  \return <ReturnValue>
*/
// 50==================================================
int snow::run_snow_model(
    int iDay_doy, int iFlag_transpiration, int iLand_cover_type, int iMonth_ymd,
    int iPrecipitation_type, int iStorm_type, double dElevation_in,
    double dPET_hru_in, double dLai_in, double dPrecipitation_basin_in,
    double dRain_net_in, double dSnow_net_in, double dShortwave_radiation_in,
    double dTemperature_max_in, double dTemperature_mean_in,
    double dTemperature_min_in, double dTemperature_rain_in,
    double dTemperature_snow_in)
{
  int error_code = 1;

  double dHeat_rain = 0.0;
  double dHeat_snow = 0.0;
  double dHeat_night = 0.0;
  double dHeat_day = 0.0;
  double dHeat_convection = 0.0;

  double dHeat_total1 = 0.0;
  double dHeat_total2 = 0.0;
  double dHeat_total3 = 0.0;
  double dHeat_total4 = 0.0;

  double dHeat_difference1 = 0.0;
  double dHeat_difference2 = 0.0;
  double dHeat_difference3 = 0.0;
  double dHeat_difference4 = 0.0;

  double dTemperature_rain_celsius;
  double dTemperature_snow_celsius;
  double dTemperature_adjust;
  double dFraction_rain;
  double dSnow_difference = 0.0;

  double dPrecipitation = dRain_net_in + dSnow_net_in;

  dTemperature_adjust = dTemperature_snow_in;
  if (dTemperature_snow_in > dTemperature_min_in)
    {
      dTemperature_adjust = dTemperature_min_in;
    }

  dSnowmelt = 0.0;
  if (dPrecipitation > 0.0)
    {
      dFraction_rain = (dRain_net_in) / (dRain_net_in + dSnow_net_in);
    }
  else
    {
      dFraction_rain = 0.0;
    }
  if (iFlag_has_snow == 1)
    {
      iFlag_old_snow = 1;
    }
  else
    {
      iFlag_old_snow = 0;
    }

  // melt season based on date
  determine_accumulate_or_melt_season(iDay_doy);
  // swe threshold based on elevation
  determine_elevation_based_parameter(dElevation_in);

  dTemperature_rain_celsius = dTemperature_rain_in + kelvin_2_celsius;

  dTemperature_snow_celsius = dTemperature_snow_in + kelvin_2_celsius;

  dHeat_night = calculate_night_radiation(
      iLand_cover_type, iMonth_ymd, iStorm_type, dLai_in,
      dPrecipitation_basin_in, dTemperature_snow_in);
  dHeat_day = calculate_day_radiation(
      iLand_cover_type, iMonth_ymd, iStorm_type, dLai_in,
      dPrecipitation_basin_in, dShortwave_radiation_in, dTemperature_rain_in);
  dHeat_rain = dTemperature_rain_celsius * dSpecific_heat_water * dRain_net_in;

  dHeat_snow = dTemperature_snow_celsius * dSpecific_heat_ice * dSnow_net_in;

  // dHeat_latent = dLatent_heat_water * dRain_net_in;

  dHeat_convection = calculate_snow_convection_heat_flux(
      iMonth_ymd, iLand_cover_type, dPrecipitation_basin_in,
      dTemperature_mean_in);

  switch (iPrecipitation_type)
    {
    case 1: // here comes the rain
      {
        iFlag_new_snow = 0;
        dHeat_total1 = dHeat_night + dHeat_day + dHeat_rain + dHeat_convection;
        if (iFlag_has_snow == 1)
          {
            if (dSnow_heat_deficit == 0.0)
              {
                // snow is at isothermal state
                dSwe_water = dSwe_water + dRain_net_in;
                if (abs(dHeat_total1) < tiny_value)
                  {
                    // incoming heat can be ignored

                    adjust_snowpack_freewater();
                  }
                else
                  {
                    if (dHeat_total1 < -tiny_value)
                      {
                        // snowpack needs to lose heat
                        // calculate how much water can be frozen
                        dSnow_heat_deficit = dHeat_total1;
                        freeze_snowpack_freewater(dTemperature_adjust);
                      }
                    else
                      {
                        // positive heat is added to isothermal snowpack

                        melt_snowpack_ice(dHeat_total1);
                      }
                  }
              }
            else
              {
                // heat deficit must be negative of course
                dSwe_water = dRain_net_in;
                if (abs(dHeat_total1) < tiny_value)
                  {
                    // no additional heat, so heat deficit may freeze incoming
                    // rain
                    freeze_snowpack_freewater(dTemperature_adjust);
                  }
                else
                  {
                    if (dHeat_total1 < -tiny_value)
                      {
                        // snowpack needs to lose heat
                        // but the heat deficit is already negative

                        dSnow_heat_deficit = dSnow_heat_deficit + dHeat_total1;
                        freeze_snowpack_freewater(dTemperature_adjust);
                      }
                    else
                      {

                        dHeat_difference1 =
                            dSnow_heat_deficit +
                            dHeat_total1; // new heat deficit may be 0.0

                        if (abs(dHeat_difference1) < tiny_value)
                          {
                            dSnow_heat_deficit = 0.0;
                            dSnow_temperature = celsius_2_kelvin;

                            adjust_snowpack_freewater();
                          }
                        else
                          {
                            // there are difference, so maybe water can be
                            // frozen or melt
                            if (dHeat_difference1 < -tiny_value)
                              {
                                // try the maximum amount of frozen rain
                                dSnow_heat_deficit = dHeat_difference1;

                                freeze_snowpack_freewater(dTemperature_adjust);
                              }
                            else
                              {
                                melt_snowpack_ice(dHeat_difference1);
                              }
                          }
                      }
                  }
              }
          }
        else
          {
            // rain on a non-snow day, nothing happens
          }
      }
      break;
    case 2:
      {
        // snow on snow
        iFlag_new_snow = 1;
        dHeat_total2 = dHeat_night + dHeat_day + dHeat_snow + dHeat_convection;
        if (iFlag_has_snow == 1)
          {
            dSwe_ice = dSwe_ice + dSnow_net_in;
            if (dSnow_heat_deficit == 0.0)
              {
                // at isothermal state

                if (abs(dHeat_total2) < tiny_value) // heat can be ignored
                  {
                    // heat can be ignored, but snow will be add

                    dSwe = dSwe_ice + dSwe_water;
                    update_snow_depth_and_density(dSnow_net_in);
                    update_snow_temperature(dTemperature_adjust);
                    dSnowmelt = 0.0;
                  }
                else // the heat cannot be ignored
                  {

                    if (dHeat_total2 < -tiny_value)
                      {
                        // lose heat
                        dSnow_heat_deficit = dHeat_total2;
                        freeze_snowpack_freewater(dTemperature_adjust);
                      }
                    else
                      {
                        // heat is added, so it could melt ice
                        melt_snowpack_ice(dHeat_total2);
                      }
                  }
              }
            else
              {

                // not at isothermal state
                if (abs(dHeat_total2) < tiny_value)
                  {
                    // do not lose heat, but itself has deficit
                    dSwe_water = 0.0;
                    dSwe = dSwe_ice + dSwe_water;
                    update_snow_depth_and_density(dSnow_net_in);
                    dSnowmelt = 0.0;
                  }
                else
                  {
                    if (dHeat_total2 < -tiny_value)
                      {
                        // both are negative, so it may lose heat
                        dSnow_heat_deficit = dSnow_heat_deficit + dHeat_total2;
                        dSwe_water = 0.0;

                        dSwe = dSwe_ice + dSwe_water;

                        update_snow_depth_and_density(dSnow_net_in);
                        update_snow_temperature(dTemperature_adjust);
                        dSnowmelt = 0.0;
                      }
                    else
                      {
                        // may be negative, 0, or position
                        dHeat_difference2 =
                            dSnow_heat_deficit +
                            dHeat_total2; // heat deficit may be 0.0
                        if (abs(dHeat_difference2) < tiny_value)
                          {
                            dSnow_heat_deficit = 0.0;
                            dSnow_temperature = celsius_2_kelvin;
                            dSwe_water = 0.0;

                            dSwe = dSwe_ice + dSwe_water;
                            update_snow_depth_and_density(dSnow_net_in);
                            dSnowmelt = 0.0;
                          }
                        else
                          {
                            if (dHeat_difference2 < -tiny_value)
                              {
                                dSnow_heat_deficit =
                                    dSnow_heat_deficit + dHeat_difference2;
                                dSwe_water = 0.0;

                                dSwe = dSwe_ice + dSwe_water;
                                update_snow_depth_and_density(dSnow_net_in);
                                update_snow_temperature(dTemperature_adjust);
                                dSnowmelt = 0.0;
                              }
                            else
                              {
                                // positive, so add heat to snowpack
                                melt_snowpack_ice(dHeat_difference2);
                              }
                          }
                      }
                  }
              }
          }
        else
          {
            // there is no snow previously
            // the new snow may be melt away by the heat as well

            dSwe_ice = dSnow_net_in;
            dSwe_density = dSwe_density_init;
            if (abs(dHeat_total2) < tiny_value)
              {

                dSwe_water = 0.0;
                dSwe = dSwe_ice + dSwe_water;
                dSnow_heat_deficit = 0.0;
                dSnow_temperature = celsius_2_kelvin;
                update_snow_depth_and_density(dSnow_net_in);
                dSnowmelt = 0.0;
              }
            else
              {
                if (dHeat_total2 < -tiny_value)
                  {
                    // snow can stay with deficit

                    dSnow_heat_deficit = dHeat_total2;

                    dSwe_water = 0.0;
                    dSwe = dSwe_ice + dSwe_water;
                    update_snow_temperature(dTemperature_adjust);
                    update_snow_depth_and_density(dSnow_net_in);

                    dSnowmelt = 0.0;
                  }
                else
                  {
                    // all snow will be melt
                    dSnowmelt = dSnow_net_in;
                    dSwe = 0.0;
                    update_snow_status();
                  }
              }
          }
      }
      break;
    case 3: // a mixture event
      {
        iFlag_new_snow = 1;
        dHeat_total3 = dHeat_night + dHeat_day + dHeat_rain + dHeat_snow +
                       dHeat_convection;
        if (iFlag_has_snow == 1)
          {
            dSwe_ice = dSwe_ice + dSnow_net_in;
            dSwe_water = dSwe_water + dRain_net_in;

            if (dSnow_heat_deficit == 0.0)
              {
                // at isothermal state

                if (abs(dHeat_total3) < tiny_value) // heat can be ignored
                  {
                    // no losing, stay at isothermal state
                    dSnow_heat_deficit = 0.0;
                    dSnow_temperature = celsius_2_kelvin;
                    adjust_snowpack_freewater();
                  }
                else
                  {
                    if (dHeat_total3 < -tiny_value)
                      {
                        // lost heat
                        dSnow_heat_deficit = dHeat_total3;
                        freeze_snowpack_freewater(dTemperature_adjust);
                      }
                    else
                      {
                        melt_snowpack_ice(dHeat_total3);
                      }
                  }
              }
            else
              {
                if (abs(dHeat_total3) < tiny_value)
                  {
                    freeze_snowpack_freewater(dTemperature_adjust);
                  }
                else
                  {
                    if (dHeat_total3 < -tiny_value)
                      {
                        dSnow_heat_deficit = dSnow_heat_deficit + dHeat_total3;
                        update_snow_temperature(dTemperature_adjust);
                        freeze_snowpack_freewater(dTemperature_adjust);
                      }
                    else
                      {
                        dHeat_difference3 = dSnow_heat_deficit + dHeat_total3;
                        if (abs(dHeat_difference3) < tiny_value)
                          {

                            // no losing, stay at isothermal state
                            dSnow_heat_deficit = 0.0;
                            dSnow_temperature = celsius_2_kelvin;
                            adjust_snowpack_freewater();
                          }
                        else
                          {
                            if (dHeat_difference3 < -tiny_value)
                              {
                                // even more deficit
                                // heat deficit may freeze
                                dSnow_heat_deficit = dHeat_difference3;
                                update_snow_temperature(dTemperature_adjust);
                                freeze_snowpack_freewater(dTemperature_adjust);
                              }
                            else
                              {
                                melt_snowpack_ice(dHeat_difference3);
                              }
                          }
                      }
                  }
              }
          }

        else
          {
            // there is no snow previously
            dSwe_ice = dSnow_net_in;
            dSwe_water = dRain_net_in;
            dSwe_density = dSwe_density_init;
            // the new snow may be melt away by the heat as well
            if (abs(dHeat_total3) < tiny_value)
              {

                dSnow_heat_deficit = 0.0;
                dSnow_temperature = celsius_2_kelvin;
                adjust_snowpack_freewater();
                update_snow_depth_and_density(dSnow_net_in);
              }
            else
              {
                if (dHeat_total3 < -tiny_value)
                  {
                    dSnow_heat_deficit = dHeat_total3;
                    freeze_snowpack_freewater(dTemperature_adjust);
                  }
                else
                  {
                    melt_snowpack_ice(dHeat_total3);
                  }
              }
          }
      }
      break;
    case 4: // no precipitation
      {
        iFlag_new_snow = 0;
        dHeat_total4 = dHeat_night + dHeat_day + dHeat_convection;
        if (iFlag_has_snow == 1)
          {
            if (dSnow_heat_deficit == 0)
              {
                // there could be freewater
                if (abs(dHeat_total4) < tiny_value)
                  {
                    // nothing happens
                  }
                else
                  {
                    // there could be freewater
                    if (dHeat_total4 < -tiny_value)
                      {
                        // lose heat
                        dSnow_heat_deficit = dHeat_total4;

                        freeze_snowpack_freewater(dTemperature_adjust);
                      }
                    else
                      {
                        melt_snowpack_ice(dHeat_total4);
                      }
                  }
              }
            else
              {

                if (abs(dHeat_total4) < tiny_value)
                  {
                    // remain the same
                    dSnowmelt = 0.0;
                  }

                else
                  {
                    dHeat_difference4 = dSnow_heat_deficit + dHeat_total4;
                    if (dHeat_total4 < -tiny_value)
                      {

                        // lose heat, but there is no freewater at all
                        dSnow_heat_deficit = dHeat_difference4;
                        update_snow_temperature(dTemperature_adjust);
                        dSnowmelt = 0.0;
                      }
                    else
                      {

                        if (abs(dHeat_difference4) < tiny_value)
                          {
                            // nothing happens
                            dSnowmelt = 0.0;
                            dSnow_heat_deficit = 0.0;
                            dSnow_temperature = celsius_2_kelvin;
                          }
                        else
                          {
                            if (dHeat_difference4 < -tiny_value)
                              {
                                // lose heat, but there is no freewater at all
                                dSnow_heat_deficit = dHeat_difference4;
                                update_snow_temperature(dTemperature_adjust);
                                dSnowmelt = 0.0;
                              }
                            else
                              {
                                // add heat, there is not freewater previously
                                melt_snowpack_ice(dHeat_difference4);
                              }
                          }
                      }
                  }

                //
              }
          }
        else
          {
            // nothing
          }
      }
      break;
    default:
      {
        iFlag_new_snow = 0;
      }
      break;
    }
  update_snow_status();
  if (iFlag_has_snow == 1)
    {
      if (iPrecipitation_type == 4)
        {
          calculate_snow_evapotranspiration(iFlag_transpiration,
                                            iLand_cover_type, dPET_hru_in);
        }
      else
        {
          dSnow_et = 0.0;
          dSnow_et_hru = 0.0;
        }
      if (iFlag_has_snow == 1)
        {
          calculate_snow_cover_area(iFlag_new_snow, dSnow_net_in);
          calculate_snow_albedo(iPrecipitation_type, dFraction_rain,
                                dSnow_net_in);
          // final step
          dSwe_previous = dSwe;
          dSca_previous = dSca;
          dAlbedo_previous = dAlbedo;
          update_accumulate_or_melt_season();

          if (iFlag_partly_reset_albedo > 0)
            {
              dSnow_difference = dSnow_cumulative_save - dSnowmelt;

              if (abs(dSnow_difference) < tiny_value)
                {
                  // all snow turn into melt
                  dSnowmelt = dSnow_cumulative_save;
                  dSnow_cumulative_save = 0.0;
                }
              else
                {
                  if (dSnow_difference <= -tiny_value)
                    {
                      dSnowmelt = dSnow_cumulative_save;
                      dSnow_cumulative_save = 0.0;
                    }
                  else
                    {
                      dSnow_cumulative_save = dSnow_difference;
                    }
                }
            }
        }
      update_snow_status();
    }
  else
    {
      update_snow_status();
    }

  return error_code;
}

// 50==================================================
/*!
  update the accumulation and melt
  \param
  \return <ReturnValue>
*/
// 50==================================================
int snow::update_accumulate_or_melt_season()
{
  int error_code = 1;
  if (iFlag_melt_season_force == 0) // before the force melt day
    {
      if (iFlag_melt_season_potential == 0) // before the potential melt day
        {
          // nothing
        }
      else // after the potential melt day
        {
          if (dSnow_temperature >= celsius_2_kelvin)
            {
              iCount_above_zero_day = iCount_above_zero_day + 1;
              if (iCount_above_zero_day > 4)
                {
                  iFlag_melt_season_force = 1;
                  iCount_above_zero_day = 0;
                }
            }
          else
            {
              iCount_above_zero_day = 0;
            }
        }
    }
  return error_code;
}

///
/// \return
int snow::update_snow_parameter()
{
  int error_code = 1;
  return error_code;
}

int snow::update_snow_temperature(double dTemperature_air_in)
{
  int error_code = 1;
  double dTemperature_air_celsius = dTemperature_air_in + kelvin_2_celsius;
  dSnow_temperature_celsius =
      dSnow_heat_deficit / (dSwe * dDensity_water * dSpecific_heat_ice);

  if (dSnow_temperature_celsius < dTemperature_air_celsius)
    {
      dSnow_temperature_celsius = dTemperature_air_celsius;
      dSnow_heat_deficit =
          dSnow_temperature_celsius * dDensity_water * dSpecific_heat_ice;
    }

  dSnow_temperature = dSnow_temperature_celsius + celsius_2_kelvin;

  return error_code;
}
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
int snow::update_snow_status()
{
  int error_code = 1;
  if (dSwe < tiny_value)
    {
      // no snow cover since swe is near zero
      iFlag_has_snow = 0;
      iFlag_sca_on_curve = 1;
      dSwe_ice = 0.0;
      dSwe_water = 0.0;
      dSwe = 0.0;
      dSca = 0.0;
      dSnow_temperature = celsius_2_kelvin;
      dSwe_density = 0.0;
      dSwe_depth = 0.0;
      dAlbedo = 0.0;
      dSnow_heat_deficit = 0.0;
      dSnow_et = 0.0;
      dSnow_et_hru = 0.0;
      // history
      dSwe_previous = 0.0;
      dSca_previous = 0.0;
      dAlbedo_previous = 0.0;
      dSwe_max = 0.0;
      dSwe_sub_max = 0.0;
    }
  else
    {
      iFlag_has_snow = 1;
      if (abs(dSnow_heat_deficit) < tiny_value)
        {
          dSnow_heat_deficit = 0.0;
        }
    }
  return error_code;
}
} // namespace ecohydrology
