// 50==================================================
/*!
  \file   column.cpp
  \brief
  <long description>
  \author Chang Liao
  \date   2017-07-21
*/
// 50==================================================
#include "column.h"

namespace ecohydrology
{
column::column(){};

column::~column(){};

int column::calculate_column_nep()
{
  int error_code = 1;

  switch (this->eCU_type)
  {
    case eCU_glacier: // glacier
    {

    }
      break;
    case eCU_lake: // lake
    {
    }
      break;
    case eCU_land: // land
    {
      dColumn_nep= cLand.dLand_nep;
      iFlag_source_or_sink = cLand.iFlag_source_or_sink;
    }
      break;
    case eCU_stream: // stream
    {

    }
      break;
    case eCU_swale:
      break;
    default:
      break;
  }

  return error_code;
}

int column::update_column_status()
{
  int error_code = 1;

  switch (this->eCU_type)
    {
    case eCU_glacier: // glacier
      {
        cGlacier.update_glacier_status();
      }
      break;
    case eCU_lake: // lake
      {
      }
      break;
    case eCU_land: // land
      {
        cLand.update_land_status();

      }
      break;
    case eCU_stream: // stream
      {
        cReach.update_reach_status();
      }
      break;
    case eCU_swale:
      break;
    default:
      break;
    }

  calculate_column_nep ();



  return error_code;
}

int column::initialize_column()
{
  int error_code = 1;

  // setup neightbors
  aNeighbor_index.fill(-1);
  aNeighbor_cascade_percentage.fill(0.0);

  eColumn_type eCU_type = this->eCU_type;
  switch (eCU_type)
    {
    case eCU_glacier: // glacier
      {
        cGlacier.initialize_glacier(eCU_type);
      }
      break;
    case eCU_lake: // lake
      {
        cLake.initialize_lake(eCU_type);
      }
      break;
    case eCU_land: // land
      {

        cLand.initialize_land(eCU_type);
      }
      break;
    case eCU_stream: // stream
      {
        cReach.initialize_reach(eCU_type);
      }
      break;
    case eCU_swale: // swale
      {
      }
      break;
    default: // land current unreacheable
      {
      }
      break;
    }
  return error_code;
}

int column::update_column_parameter(int iFlag_run_carbon_cascade,
                                    int iFlag_run_water_cascade,
                                    int iLand_cover_type, int iSoil_type,
                                    int iVegetation_type,
                                    double dImpervious_surface_percentage_in,
                                    double dPervious_surface_percentage_in,
                                    double dSurface_depression_percentage_in)
{
  int error_code = 1;
  switch (this->eCU_type)
    {
    case eCU_glacier: // glacier
      cGlacier.update_glacier_parameter(iFlag_run_carbon_cascade,
                                        iLand_cover_type);
      break;
    case eCU_lake: // lake

      cLake.update_lake_parameter(iLand_cover_type);
      break;
    case eCU_land: // land
      cLand.update_land_parameter(
          iFlag_run_carbon_cascade, iFlag_run_water_cascade,
          iLand_cover_type, iSoil_type,
          iVegetation_type, dImpervious_surface_percentage_in,
          dPervious_surface_percentage_in, dSurface_depression_percentage_in);
      break;
    case eCU_stream: // stream;
      cReach.update_reach_parameter();
      break;
    case eCU_swale: // swale
      break;
    default:
      break;
    }
  return error_code;
}

int column::run_column_model(int iDay_doy, int iMonth_ymd, double dFpar_in,
                             double dLai_in, double dPrecipitation_in,
                             double dPrecipitation_basin_in,
                             double dTemperature_basin_max_in,
                             double dTemperature_dewpoint_in,
                             double dTemperature_max_in,
                             double dTemperature_min_in)
{
  int error_code = 1;
  int iFlag_transpiration;
  int iPrecipitation_type;
  int iStorm_type;
  double dTemperature_max;
  double dTemperature_mean;

  double dTemperature_min;
  double dTemperature_rain;
  double dTemperature_snow;
  double dVapor_pressure_deficit;
  double dShortwave_radiation;
  double dPar;
  double dRain;
  double dSnow;
  double dAspect = cGeology.dAspect;
  double dSlope = cGeology.dSlope;
  double dLatitude = cGeology.dLatitude;
  // double dElevation = cGeology.dElevation;
  cAtmosphere.run_atmosphere_model(
      iDay_doy, iMonth_ymd, dAspect, dLatitude, dPrecipitation_in, dSlope,
      dTemperature_basin_max_in, dTemperature_dewpoint_in, dTemperature_max_in,
      dTemperature_min_in);
  dTemperature_max = cAtmosphere.cTemperature.dTemperature_max;
  dTemperature_mean = cAtmosphere.cTemperature.dTemperature_mean;
  dTemperature_min = cAtmosphere.cTemperature.dTemperature_min;

  dTemperature_rain = cAtmosphere.cPrecipitation.dTemperature_rain;
  dTemperature_snow = cAtmosphere.cPrecipitation.dTemperature_snow;

  dVapor_pressure_deficit = cAtmosphere.cTemperature.dVapor_pressure_deficit;
  dShortwave_radiation = cAtmosphere.cRadiation.dShortwave_radiation_actual;
  dPar = cAtmosphere.cRadiation.dPar;
  iFlag_transpiration = cAtmosphere.cTemperature.iFlag_transpiration;
  iPrecipitation_type = cAtmosphere.cPrecipitation.iPrecipitation_type;
  dRain = cAtmosphere.cPrecipitation.dRain;
  dSnow = cAtmosphere.cPrecipitation.dSnow;
  iStorm_type = cAtmosphere.cPrecipitation.iStorm_type;
  switch (eCU_type)
    {
    case eCU_glacier:
      {
        cGlacier.run_glacier_model(
            iDay_doy, iFlag_transpiration, iMonth_ymd, iPrecipitation_type,
            iStorm_type, dLai_in,

            dPrecipitation_basin_in, dRain, dShortwave_radiation, dSnow,

            dTemperature_max, dTemperature_mean, dTemperature_min,
            dTemperature_rain, dTemperature_snow,

            dVapor_pressure_deficit);
      }
      break;
    case eCU_lake:
      {
      }
      break;
    case eCU_land:
      {
        cLand.run_land_model(
            iDay_doy, iFlag_transpiration, iMonth_ymd, iPrecipitation_type,
            iStorm_type, dFpar_in, dLai_in, dPar, dPrecipitation_in,
            dPrecipitation_basin_in, dRain, dShortwave_radiation, dSnow,
            dTemperature_basin_max_in, dTemperature_dewpoint_in,
            dTemperature_max, dTemperature_mean, dTemperature_min,
            dTemperature_rain, dTemperature_snow, dVapor_pressure_deficit);
      }
      break;
      // stream is very different with other column type

    case eCU_stream:
      {
      }
      break;
    case eCU_swale:
      {
        break;
      }
    }
  return error_code;
}
} // namespace ecohydrology
