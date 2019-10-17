#include "canopy.h"

namespace ecohydrology
{
// 50==================================================
/*!
  <long-description>

  \param void
*/
// 50==================================================
canopy::canopy()
{
  dCanopy_carbon = missing_value;

  dCanopy_lai = missing_value;
  dCanopy_lai_previous = missing_value;
  // dCanopy_nitrogen = missing_value;
}

canopy::~canopy(){};

///
/// \param iColumn_type
/// \param iVegetation_type
/// \return
int canopy::initialize_canopy(eColumn_type eCU_type, int iVegetation_type)
{
  int error_code = 1;
  switch (eCU_type)
    {
    case eCU_glacier:
      {
      }
      break;
    case eCU_lake:
      {
      }
      break;
    case eCU_land: // land
      {
        if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
          {
            dCanopy_carbon = 0.0;
            dCanopy_lai = 0.0;
            dCanopy_lai_previous = 0.0;
          }
        else
          {

            dCanopy_carbon = 0.0;
            dCanopy_lai = 0.5;
            dCanopy_lai_previous = 0.5;
            // dCanopy_nitrogen = 0.0;
          }
      }
      break;
    case eCU_stream:
      {
      }
      break;
    case eCU_swale:
      {
      }
      break;
    default:
      {
      }
      break;
    }
  // 50==================================================
  cInterception.initialize_interception(eCU_type, iVegetation_type);
  cPhotosynthesis.initialize_photosynthesis(eCU_type, iVegetation_type);
  cLitterfall.initialize_litterfall(eCU_type, iVegetation_type);
  return error_code;
}
// 50==================================================
/*!
  <calculate foliage scalar>
  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
int canopy::calculate_foliage_scalar(int iVegetation_type)
{
  int error_code = 1;
  double dScalar_foliage_out = 1.0 / (dkleafc * exp(dcov * dCanopy_carbon));
  if (dScalar_foliage_out < small_value)
    {
      dScalar_foliage_out = 0.0;
    }
  else
    {
      if (dScalar_foliage_out > 1.0)
        {
          dScalar_foliage_out = 1.0;
        }
    }
  this->dScalar_foliage = dScalar_foliage_out;
  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iMonth  the month of the year
  \param iPrecipitation_type   see atmosphere class
  \param dArea_in area of the grid, unit: square meter
  \param dPET_hru potential ET, unit: meter
  \param dLai_in  modis product lai, unitless, between 0 and 10
  \param dRain_in unit: meter
  \param dSnow_in  unit: meter
  \param dFpar_in modis product
  \param dPAR daily photosynthesis from radiation class  units: joule per square
  meter per day \param dTemperature_min_in minimum temperature unti kelvin
  \param dVapor_pressure_deficit_in      unit, pascal \return <ReturnValue>
*/
// 50==================================================

int canopy::run_canopy_model(int iMonth, int iPrecipitation_type,
                             int iVegetation_type, double dArea_in,
                             double dEt_in, double dFpar_in, double dLai_in,
                             double dRain_in, double dSnow_in, double dPar_in,
                             double dTemperature_mean_in,
                             double dVapor_pressure_deficit_in)
{
  int error_code = 1;

  // update lai
  this->dCanopy_lai = dLai_in;

  // interception
  error_code = cInterception.run_interception_model(
      iMonth, iPrecipitation_type, iVegetation_type, dArea_in, dEt_in,
      dCanopy_lai, dRain_in, dSnow_in, dTemperature_mean_in);

  // Photosynthesis
  error_code = cPhotosynthesis.run_photosynthesis_model(
      iVegetation_type,
      dFpar_in, // climate inputs
      dPar_in, dTemperature_mean_in, dVapor_pressure_deficit_in);
  // update nitrogen

  // litterfall

  error_code =
      cLitterfall.run_litterfall_model(iVegetation_type, dCanopy_carbon);

  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================

int canopy::update_canopy_parameter(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {

      // do we have to consider previous vegetation type?

      dkleafc = kleafc[iVegetation_type - 1];
      dcov = cov[iVegetation_type - 1];

      cInterception.update_interception_parameter(iVegetation_type);
      cPhotosynthesis.update_photosynthesis_parameter(iVegetation_type);
      cLitterfall.update_litterfall_parameter(iVegetation_type);
    }

  return error_code;
}

int canopy::update_canopy_status(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dCanopy_carbon = 0.0;
      dCanopy_lai_previous = 0.0;
    }
  else
    {
      if (dCanopy_lai < small_value)
        {
          dCanopy_carbon = 0.0;
        }
      else
        {
          dCanopy_carbon = dCanopy_carbon +
                           cPhotosynthesis.dPhotosynthesis_gpp * 0.4 -
                           cLitterfall.dLitterfall_carbon;
          if (dCanopy_carbon < near_zero)
            {
              dCanopy_carbon = 0.0;
            }
        }
      dCanopy_lai_previous = dCanopy_lai;
    }
  return error_code;
}
} // namespace ecohydrology
