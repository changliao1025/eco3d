// 50==================================================
/*!
  \file photosynthesis.cpp
  \brief

  <long description>

  \author Chang Liao
  \date 2017-01-20
*/
// 50==================================================
#include "photosynthesis.h"

namespace ecohydrology
{
photosynthesis::photosynthesis() { dPhotosynthesis_gpp = missing_value; }

photosynthesis::~photosynthesis() {}

///
/// \param eCU_type
/// \param iVegetation_type
/// \return
int photosynthesis::initialize_photosynthesis(eColumn_type eCU_type,
                                              int iVegetation_type)
{
  int error_code = 1;

  switch (eCU_type)
    {
    case eCU_glacier:
      {
      }
      break;
    case eCU_lake:
      break;
    case eCU_land: // land
      {
        if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
          {
            dPhotosynthesis_gpp = missing_value;
          }
        else
          {

            dPhotosynthesis_gpp = 0.0;
            dScalar_temperature = 1.0;
            dScalar_vpd = 1.0;
          }
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
// 50==================================================
/*!
  calculate daily GPP using modis algorithm>

  \param dFpar_in fraction, unitless
  \param dIpar_in instant daily par, units: mega joule per square meter per day
  \param dTemperature_min_in unit: kelvin
  \param dVapor_pressure_deficit_in unit: pascal
  \return <ReturnValue>
*/
// 50==================================================
int photosynthesis::calculate_daily_gpp(double dFpar_in, double dIpar_in,
                                        double dTemperature_min_in,
                                        double dVapor_pressure_deficit_in)
{
  // normal these inputs do not need quality control
  int error_code = 1;
  double dApar = dIpar_in * dFpar_in;
  double dEpsilon = calculate_epsilon(dEpsilon_max, dTemperature_min_in,
                                      dVapor_pressure_deficit_in);
  this->dPhotosynthesis_gpp = dEpsilon * dApar;

  if (this->dPhotosynthesis_gpp < near_zero)
    {
      dPhotosynthesis_gpp = 0.0;
    }
  // dPhotosynthesis_gpp_gram = dPhotosynthesis_gpp *
  // kilogram_per_square_meter_2_gram_per_square_meter;
  return error_code;
}

// 50==================================================
/*!
  calculate epsilon>

  \param dEpsilon_max_in
  \param dTemperature_min_in minimum temperature unit kelvin
  \param dVapor_pressure_deficit_in unit: pascal
  \return <epsilon>
*/
// 50==================================================

double photosynthesis::calculate_epsilon(double dEpsilon_max_in,
                                         double dTemperature_min_in,
                                         double dVapor_pressure_deficit_in)
{
  double dEpsilon_out;
  double dScalar_temperature =
      calculate_temperature_scalar(dTemperature_min_in);
  double dScalar_vpd = calculate_vpd_scalar(dVapor_pressure_deficit_in);
  dEpsilon_out = dEpsilon_max_in * dScalar_temperature * dScalar_vpd;
  // quality control
  if (dEpsilon_out < near_zero)
    {
      dEpsilon_out = 0.0;
    }

  return dEpsilon_out;
}

// 50==================================================
/*!
  temperature scalar

  \param dTemperature_min_in unit kelvin
  \return <scalar, unitless>
*/
// 50==================================================
double photosynthesis::calculate_temperature_scalar(double dTemperature_min_in)
{
  double dScalar_temperature_out;
  double dTemperature_min_celsisu = dTemperature_min_in + kelvin_2_celsius;
  double a = -1.0 / (dTmin_min - dTmin_max);
  double b = -1.0 * dTmin_min * a;
  dScalar_temperature_out = (dTemperature_min_celsisu)*a + b;
  // 50==================================================
  // quality control
  // 50==================================================

  if (dScalar_temperature_out < small_value)
    {
      dScalar_temperature_out = 0.0;
    }
  else
    {
      if (dScalar_temperature_out > 1.0)
        {
          dScalar_temperature_out = 1.0;
        }
    }

  return dScalar_temperature_out;
}

// 50==================================================
/*!
  vapor pressure deficit scalar

  \param dVapor_pressure_deficit_in unit: pascal
  \return <scalar, unitless>
*/
// 50==================================================
double photosynthesis::calculate_vpd_scalar(double dVapor_pressure_deficit_in)
{
  double dScalar_vpd_out;
  double a = 1.0 / (dVpd_min - dVpd_max);
  double b = -1.0 * dVpd_max * a;
  dScalar_vpd_out = dVapor_pressure_deficit_in * a + b;
  // quality control
  if (dScalar_vpd_out < small_value)
    {
      dScalar_vpd_out = 0.0;
    }
  else
    {
      if (dScalar_vpd_out > 1.0)
        {
          dScalar_vpd_out = 1.0;
        }
    }

  return dScalar_vpd_out;
}

// 50==================================================
/*!
  <long-description>

  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================

int photosynthesis::update_photosynthesis_parameter(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {

      dEpsilon_max = aEpsilon_max[iVegetation_type - 1];
      dTmin_max = aTmin_max[iVegetation_type - 1];
      dTmin_min = aTmin_min[iVegetation_type - 1];
      dVpd_max = aVpd_max[iVegetation_type - 1];
      dVpd_min = aVpd_min[iVegetation_type - 1];
    }
  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param dFpar_in modis product
  \param dPAR from radiation module joule per square meter per day
  \param dTemperature_min_in unit kelvin
  \param dVapor_pressure_deficit_in unit: pascal
  \return <ReturnValue>
*/
// 50==================================================

int photosynthesis::run_photosynthesis_model(int iVegetation_type,
                                             double dFpar_in, double dPar_in,
                                             double dTemperature_min_in,
                                             double dVapor_pressure_deficit_in)
{
  int error_code = 1;
  double dPar_megajoule = 0.0;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dPhotosynthesis_gpp = 0.0;
    }
  else
    {
      dPar_megajoule = dPar_in * joule_2_megajoule;
      // quality check

      if (dFpar_in < near_zero)
        {
          dFpar_in = 0.5;
        }

      calculate_daily_gpp(dFpar_in, dPar_megajoule, dTemperature_min_in,
                          dVapor_pressure_deficit_in);
    }
  return error_code;
}
} // namespace ecohydrology
