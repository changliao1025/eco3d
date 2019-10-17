// 50==================================================
/*!
  \file interception.cpp
  \brief
  <long description>
  \author Chang Liao
  \date 2017-01-22
*/
// 50==================================================
#include "interception.h"

namespace ecohydrology
{
// 50==================================================
/*!
  <long-description>
  \param void
*/
// 50==================================================
interception::interception()
{
  iIntercept_type = 4;

  iPrecipitation_type = -9999;

  dLai = 0.5;
  dCanopy_depth = missing_value;
  dCanopy_depth_hru = missing_value;
  dCanopy_depth_rain = missing_value;
  dCanopy_depth_snow = missing_value;
  dCanopy_storage = missing_value;
  dCanopy_storage_rain = missing_value;
  dCanopy_storage_snow = missing_value;

  dEt_canopy_rain = missing_value;
  dEt_canopy_snow = missing_value;
  dEt_canopy = missing_value;
  dEt_canopy_hru = missing_value;
  dPrecipitation_net = missing_value;

  dRain_net = missing_value;
  dSnow_net = missing_value;
  dThroughfall = missing_value;
  dThroughfall_rain = missing_value;
  dThroughfall_snow = missing_value;
}

interception::~interception() {}

// 50==================================================
/*!
  <calculate the et at the canopy level, >
  \param iMonth month of the year
  \param iIntercept_type type of interception
  \param dPET: hru averaged potential ET, unit meter
  \return <ReturnValue>
*/
// 50==================================================
double interception::calculate_canopy_evapotranspiration(int iIntercept_type,
                                                      int iMonth,
                                                      double dPet_hru_in)
{
  int error_code = 1;
  double dEt_canopy_dummy;
  double dDifference_et;
  if (dPet_hru_in < tiny_value)
    {
      dEt_canopy = 0.0;
      dEt_canopy_hru = 0.0;
    }
  else
    {
      if (dLai < small_value)
        {
          // when there is literally no lai, et is 0.0
          dEt_canopy = 0.0;
          dEt_canopy_hru = 0.0;
        }
      else
        {
          if (dLai <= 1.0)
            { // lai is less than 1, in this case, Pet_hru equals to Pet
              // intensity
              if (dCanopy_depth < tiny_value)
                {
                  // canopy depth is ignorable
                  dEt_canopy = 0.0;
                  dEt_canopy_hru = 0.0;
                }
              else
                {
                  // types of interception
                  switch (iIntercept_type)
                    {
                    case 1:
                      dEt_canopy_rain =
                          dPet_hru_in / aCoefficient_pan[iMonth - 1];
                      dEt_canopy_snow = 0.0;
                      break;
                    case 2:
                      dEt_canopy_snow = dPet_hru_in * dFraction_sublimation;
                      // std::cout << "snow et from canopy?" << std::endl;
                      dEt_canopy_rain = 0.0;
                      break;
                    case 3:
                      dEt_canopy_rain =
                          0.5 * dPet_hru_in / aCoefficient_pan[iMonth - 1];
                      dEt_canopy_snow =
                          0.5 * dPet_hru_in * dFraction_sublimation;
                      break;
                    case 4:

                      break;
                    default:

                      break;
                    }

                  dEt_canopy_dummy = dEt_canopy_rain + dEt_canopy_snow;
                  // so far, et is calculated as intensity still
                  dDifference_et = dCanopy_depth - dEt_canopy_dummy;
                  if (abs(dDifference_et) < tiny_value)
                    {
                      // just enough for ET
                      dEt_canopy_dummy = dCanopy_depth;
                    }
                  else
                    {
                      if (dDifference_et < -tiny_value)
                        {
                          // more than depth
                          dEt_canopy_dummy =
                              dCanopy_depth; // all canopy becomes ET
                        }
                      else
                        {
                        }
                    }
                  // we then will consider the pet
                  dDifference_et = dPet_hru_in - dEt_canopy_dummy;

                  if (abs(dDifference_et) < tiny_value)
                    {
                      // it implies lai can't be 0.0
                      // since there is storage
                      dEt_canopy = dPet_hru_in;
                      dCanopy_depth = 0.0;
                    }
                  else
                    {
                      if (dDifference_et < -tiny_value)
                        {
                          // dEt_canopy_out too large
                          dEt_canopy = dPet_hru_in;
                          dCanopy_depth = 0.0;
                        }
                      else
                        {
                          dEt_canopy = dEt_canopy_dummy;
                          dCanopy_depth = dCanopy_depth - dEt_canopy_dummy;
                        }
                    }

                  // then we need to consider the lai adjustment,
                  dEt_canopy_hru = dEt_canopy_dummy * dLai;
                  dCanopy_depth_hru = dCanopy_depth * dLai;
                }
            }
          else
            { // lai great than 1.0
              if (dCanopy_depth_hru < tiny_value)
                {
                  // canopy depth is ignorable
                  dEt_canopy = 0.0;
                  dEt_canopy_hru = 0.0;
                }
              else
                {
                  // types of interception
                  switch (iIntercept_type)
                    {
                    case 1:
                      dEt_canopy_rain =
                          dPet_hru_in / aCoefficient_pan[iMonth - 1];
                      dEt_canopy_snow = 0.0;
                      break;
                    case 2:
                      dEt_canopy_snow = dPet_hru_in * dFraction_sublimation;
                      // std::cout << "snow et from canopy?" << std::endl;
                      dEt_canopy_rain = 0.0;
                      break;
                    case 3:
                      dEt_canopy_rain =
                          0.5 * dPet_hru_in / aCoefficient_pan[iMonth - 1];
                      dEt_canopy_snow =
                          0.5 * dPet_hru_in * dFraction_sublimation;
                      break;
                    case 4:

                      break;
                    default:

                      break;
                    }

                  dEt_canopy_dummy = dEt_canopy_rain + dEt_canopy_snow;

                  // so far, et is calculated as intensity still
                  dDifference_et = dCanopy_depth_hru - dEt_canopy_dummy;
                  if (abs(dDifference_et) < tiny_value) // just enough for ET
                    {
                      dEt_canopy_hru = dCanopy_depth_hru;
                    }
                  else
                    {
                      if (dDifference_et < -tiny_value)
                        {
                          // more than depth
                          dEt_canopy_hru =
                              dCanopy_depth_hru; // all canopy becomes ET
                          dCanopy_depth_hru = 0.0;
                        }
                      else
                        {
                          // less than depth
                          // that will do
                          dEt_canopy_hru = dEt_canopy_dummy;
                        }
                    }
                  // we then will consider the pet
                  dDifference_et = dPet_hru_in - dEt_canopy_hru;
                  if (abs(dDifference_et) < tiny_value)
                    {
                      dEt_canopy_hru = dPet_hru_in;
                    }
                  else
                    {
                      if (dDifference_et < -tiny_value)
                        {
                          dEt_canopy_hru =
                              dPet_hru_in; // it implies lai can't be 0.0
                          // since there is storage
                          dCanopy_depth_hru = 0.0;
                        }
                      else // more than available
                        {
                          dCanopy_depth_hru =
                              dCanopy_depth_hru - dEt_canopy_hru;
                        }
                    }
                  // then we need to consider the lai adjustment,
                  dEt_canopy = dEt_canopy_hru / dLai;
                  dCanopy_depth = dCanopy_depth_hru / dLai;
                }
            }
          if (dCanopy_depth < tiny_value)
            {
              dCanopy_depth = 0.0;
              dCanopy_depth_hru = 0.0;
            }
          if (dEt_canopy < tiny_value)
            {
              dEt_canopy = 0.0;
              dEt_canopy_hru = 0.0;
            }
        }
    }

  return error_code;
}
// 50==================================================
/*!
  <the mass balanced interception model.
  the status of the canopy interception is defined by the following variables:
  lai: the leaf area index, which defined how much of the land surface is
  covered by vegetation canopy lai can be greater than one when vegetation
  density is rather high current storage depth, which defines how much of the
  canopy storage is already occupied, current storage depth must be always lower
  the maximum storage depth max storage depth, which is constant parameter for
  each plant function type, is the maximum storage depth> \param dArea_in the
  area of the grid cell, unit, sqare meter \param dCanopy_depth_in unit: meter
  \param dCanopy_depth_capacity_in unit: meter \param dLai_in modis product 0-10
  \param dPrecipitation_in unit: mete
  //return value, the net precipitation, unit: same as precipitation
  //the status of the canopy will also be updated
  */
// 50==================================================
std::array<double, 3> interception::calculate_canopy_interception(
    double dArea_in, double dCanopy_depth_in, double dCanopy_depth_capacity_in,
    double dLai_in, double dPrecipitation_in)
{
  // 50==================================================
  // temporal variables
  double dCanopy_depth_available;     // unit: meter
  double dCanopy_storage_capacity;    // unit: cubic meter
  double dCanopy_storage_available;   // volume, unit: cubic meter
  double dDifference_lai;             // 0-10
  double dThroughfall_canopy_storage; // volume, unit: cubic meter
  double dPrecipitation_net = 0.0;
  double dDifference_depth;
  std::array<double, 3> aDummy_out = {}; // return values
  // 50==================================================
  // change in lai
  // 50==================================================
  dDifference_lai = dLai_previous - dLai_in;
  // maximum storage
  dCanopy_storage_capacity =
      dArea_in * dLai_in *
      dCanopy_depth_capacity_in; // unit : square meter * fraction *
  // meter = cubic meter
  // current storage
  dCanopy_storage = dArea_in * dLai_previous * dCanopy_depth_in;
  // available storage
  dCanopy_storage_available = dCanopy_storage_capacity - dCanopy_storage;
  // available depth
  if (abs(dCanopy_storage_available) <
      near_zero) // because of the area, I used a different thrshold
    {
      // no precipitation can be added to the canopy anymore
      dCanopy_depth_available = 0.0;
    }
  else
    {
      if (dCanopy_storage_available <= -near_zero)
        {
          // no precipitation can be added to the canopy anymore
          dCanopy_depth_available = 0.0;
        }
      else
        {
          // in this case the lai must be greater than 0
          // calculate the maximum depth could be added
          dCanopy_depth_available =
              dCanopy_storage_available / (dArea_in * dLai_in);
          // we stil need to check the depth again
          if (dCanopy_depth_available < tiny_value)
            {
              dCanopy_depth_available = 0.0;
            }
        }
    }

  // 50==================================================
  // interception in three catalogs: (1) lai unchange,(2) increase and
  // (3)decrease
  if (abs(dDifference_lai) < small_value)
    {
      // lai does not change, but the available storage can be near zero or
      // positive or negative
      if (abs(dCanopy_storage_available) < near_zero)
        {
          // no throughfall from canopy.

          // precipitation fall as throughfall
          dThroughfall = dPrecipitation_in;
          // at full capacity since available storage is 0.0
          dCanopy_depth_in = dCanopy_depth_capacity_in;
          // net precipitation is based on throughfall,
          // but in this case, all precipitation is throughfall
          dPrecipitation_net = dThroughfall;
        }
      else // when available stoarge is greater than zero
        {

          if (dCanopy_storage_available <= -near_zero)
            {
              // capacity increased ?
              // impossible
            }
          else
            {
              // if rain exceeds the available depth, then throughfall forms
              dDifference_depth = dCanopy_depth_available - dPrecipitation_in;
              // just close enough to be intercepted
              if (abs(dDifference_depth) < tiny_value)
                {
                  dThroughfall = 0.0;
                  dCanopy_depth_in = dCanopy_depth_capacity_in;
                }
              else
                {

                  if (dDifference_depth <= -tiny_value)
                    { // precipitation greater than available
                      // use difference as throughfall
                      dThroughfall = -dDifference_depth;
                      // storage depth reaches maximum value
                      dCanopy_depth_in = dCanopy_depth_capacity_in;
                    }
                  else
                    { // precipitation is much less
                      // all precipitation will be intercepted
                      dThroughfall = 0.0;
                      // depth is re-calcualted due the expandsion of lai
                      // rain is added to the storage depth
                      // old storage is distributed using new LAI
                      dCanopy_depth_in =
                          dCanopy_storage / (dArea_in * dLai_in) +
                          dPrecipitation_in;
                    }
                }
              if (dLai_in < 1.0)
                {
                  dPrecipitation_net = dThroughfall * dLai_in +
                                       dPrecipitation_in * (1.0 - dLai_in);
                }
              else
                {
                  dPrecipitation_net = dThroughfall;
                }
            }
        }
    }
  else
    {
      if (dDifference_lai <= -small_value)
        {
          // increase

          // LAI is increasing, new lai must be greater than 0.0
          // and dCanopy_storage_available must be greater than 0.0

          // even though the lai is increasing,
          // if rain speed exceeds the available depth, the throughfall still
          // forms
          dDifference_depth = dCanopy_depth_available - dPrecipitation_in;
          // same as above
          if (abs(dDifference_depth) < tiny_value) // close to each other
            {
              dThroughfall = 0.0;
              dCanopy_depth_in = dCanopy_depth_capacity_in;
            }
          else
            {
              if (dDifference_depth <= -tiny_value)
                {
                  // precipitation greater than available
                  dThroughfall = dDifference_depth;
                  // storage depth reaches maximum value
                  dCanopy_depth_in = dCanopy_depth_capacity_in;
                }
              else
                { // precipitation less than available
                  // all precipitation will be intercepted
                  dThroughfall = 0.0;
                  // depth is re-calcualted due the expandsion of lai
                  // rain is added to the storage depth
                  // old storage is distributed using new LAI
                  dCanopy_depth_in = dCanopy_storage / (dArea_in * dLai_in) +
                                     dPrecipitation_in;
                }
            }
          // calculate the net precipitation
          // lai could be large than 1.0, important!
          if (dLai_in < 1.0)
            {
              dPrecipitation_net =
                  dThroughfall * dLai_in + dPrecipitation_in * (1.0 - dLai_in);
            }
          else
            {
              dPrecipitation_net = dThroughfall;
            }
        }
      else
        {
          // decrease
          // decrease in lai, lai may be zero
          // even the lai is decreasing, the available storage could be still
          // positive, negative or zero.  storage does not change
          if (abs(dCanopy_storage_available) < tiny_value)
            {
              dThroughfall_canopy_storage = 0.0;
              dThroughfall = dPrecipitation_in;
              dCanopy_depth_in = dCanopy_depth_capacity_in;
              dPrecipitation_net = dThroughfall;
            }
          else
            {
              // when available storage is greater than zero

              if (dCanopy_storage_available <= -tiny_value)
                {
                  dThroughfall_canopy_storage =
                      -1.0 * dCanopy_storage_available;
                  if (dLai_in < small_value)
                    {
                      // all depth fall + precipitation
                      dThroughfall = dPrecipitation_in + dCanopy_depth_in;
                      dCanopy_depth_in = 0.0;
                    }
                  else
                    {
                      dThroughfall =
                          dThroughfall_canopy_storage / (dArea_in * dLai_in) +
                          dPrecipitation_in;
                      dCanopy_depth_in = dCanopy_depth_capacity_in;
                    }
                  if (dLai_in < 1.0)
                    {
                      dPrecipitation_net = dThroughfall * dLai_in +
                                           dPrecipitation_in * (1.0 - dLai_in);
                    }
                  else
                    {
                      dPrecipitation_net = dThroughfall;
                    }
                }
              else
                {
                  // if rain speed exceeds the available depth, then throughfall
                  // forms
                  dDifference_depth =
                      dPrecipitation_in - dCanopy_depth_available;
                  if (abs(dDifference_depth) <
                      tiny_value) // close to each other
                    {
                      dThroughfall = 0.0;
                      dCanopy_depth_in = dCanopy_depth_capacity_in;
                    }
                  else
                    {
                      if (dDifference_depth <=
                          -tiny_value) // precipitation less than available
                        {
                          // all precipitation will be intercepted
                          dThroughfall = 0.0;
                          // depth is re-calcualted due the expandsion of lai
                          // rain is added to the storage depth
                          // old storage is distributed using new LAI
                          dCanopy_depth_in =
                              dCanopy_storage / (dArea_in * dLai_in) +
                              dPrecipitation_in;
                        }
                      else // precipitation greater than available
                        {
                          dThroughfall = dDifference_depth;
                          // storage depth reaches maximum value
                          dCanopy_depth_in = dCanopy_depth_capacity_in;
                        }
                    }
                  if (dLai_in < 1.0)
                    {
                      dPrecipitation_net = dThroughfall * dLai_in +
                                           dPrecipitation_in * (1.0 - dLai_in);
                    }
                  else
                    {
                      dPrecipitation_net = dThroughfall;
                    }
                }
            }
        }
    }

  if (dThroughfall < tiny_value)
    {
      dThroughfall = 0.0;
    }
  if (dPrecipitation_net < tiny_value)
    {
      dPrecipitation_net = 0.0;
    }
  if (dCanopy_depth_in < tiny_value)
    {
      dCanopy_depth_in = 0.0;
    }
  aDummy_out[0] = dThroughfall;
  aDummy_out[1] = dPrecipitation_net;
  aDummy_out[2] = dCanopy_depth_in;
  return aDummy_out;
}
// 50==================================================
/*!
  <calculate storage capacity>
  \param iMonth month of the year
  \param iFlag_rain_or_snow a flag for rain or snow event rain (0) snow (1)
  \return <dCanopy_depth_capacity_in, unit meter>
*/
// 50==================================================
double interception::calculate_canopy_depth_capacity(int iFlag_rain_or_snow,
                                                     int iMonth,
                                                     int iVegetation_type)
{
  double dCanopy_depth_capacity_out;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dCanopy_depth_capacity_out = 0.0;
    }
  else
    {
      if (iFlag_rain_or_snow == 0)
        {
          dCanopy_depth_capacity_out = dCanopy_depth_capacity_rain;
        }
      else
        {
          dCanopy_depth_capacity_out = dCanopy_depth_capacity_snow;
        }
      // seasonal variations
      if (iMonth < 4 || iMonth > 10) // for winter
        {
          if (iVegetation_type >= 3) // if it is tree
            {
              dCanopy_depth_capacity_out = dCanopy_depth_capacity_out * 0.5;
            }
        }
      else
        {
          // summer time
        }
    }
  return dCanopy_depth_capacity_out;
}

///
/// \param iColumn_type
/// \param iVegetation_type
/// \return

int interception::initialize_interception(eColumn_type eCU_type,
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
            // there is no vegetation at all
            iIntercept_type = 4;
            iPrecipitation_type = 4;
            dLai = 0.0;
            dLai_previous = 0.0;
          }
        else
          {
            iIntercept_type = 4;
            iPrecipitation_type = 4;
            dLai_previous = 0.5;
            dLai = 0.5; // leaf area index, range 0-10
          }
        dCanopy_depth = 0.0;
        dCanopy_depth_hru = 0.0;
        dCanopy_depth_rain = 0.0;
        dCanopy_depth_snow = 0.0;
        dCanopy_storage = 0.0;
        dCanopy_storage_rain = 0.0;
        dCanopy_storage_snow = 0.0;
        dEt_canopy_rain = 0.0;
        dEt_canopy_snow = 0.0;
        dEt_canopy = 0.0;
        dEt_canopy_hru = 0.0;
        dRain_net = 0.0;
        dSnow_net = 0.0;
        dThroughfall = 0.0;
        dThroughfall_rain = 0.0;
        dThroughfall_snow = 0.0;
        dPrecipitation_net = 0.0;
      }
      break;
    case eCU_stream:
      break;
    case eCU_swale:
      break;
    default:
      {
      }
      break;
    }

  return error_code;
}

// 50==================================================
/*!
  <long-description>
  \param iMonth mobth of the year
  \param iPrecipitation_type precipitation type
  \param dArea_in area of the gril, unit;:square meter
  \param dLai_in modis product 0-10
  \param dPET_hru potential ET unit meter
  \param dRain_in unit: meter
  \param dSnow_in unit: meter
  \return <ReturnValue>
*/
// 50==================================================
int interception::run_interception_model(int iMonth, int iPrecipitation_type,
                                         int iVegetation_type, double dArea_in,
                                         double dEt_in, double dLai_in,
                                         double dRain_in, double dSnow_in,
                                         double dTemperature_air_in)
{
  int error_code = 1;
  // 50==================================================
  // the difference in canopy density directly change the storage capacity
  double dCanopy_depth_capacity;
  // reset necessary variables
  std::array<double, 3> aDummy = {};

  this->dLai = dLai_in;
  if (dLai < small_value)
    {
      dLai = 0.0;
    }

  switch (iPrecipitation_type)
    {
    case 1: // only rain
      {
        // after each day, only the averaged storage depth is saved
        // often used for summer scenario
        dCanopy_depth_capacity =
            calculate_canopy_depth_capacity(0, iMonth, iVegetation_type);
        aDummy = calculate_canopy_interception(dArea_in, dCanopy_depth_rain,
                                               dCanopy_depth_capacity, dLai,
                                               dRain_in);
        dThroughfall_rain = aDummy[0];
        dRain_net = aDummy[1];
        dCanopy_depth_rain = aDummy[2];
        dSnow_net = 0.0;
        dThroughfall_snow = 0.0;
        dCanopy_depth_snow = 0.0;
        iIntercept_type = 1;
        dEt_canopy = 0.0;
      }
      break;
    case 2: // only snow
      {
        dCanopy_depth_capacity =
            calculate_canopy_depth_capacity(1, iMonth, iVegetation_type);
        aDummy = calculate_canopy_interception(dArea_in, dCanopy_depth_snow,
                                               dCanopy_depth_capacity, dLai,
                                               dSnow_in);
        dRain_net = 0.0;
        dThroughfall_rain = 0.0;
        dCanopy_depth_rain = 0.0;
        dThroughfall_snow = aDummy[0];
        dSnow_net = aDummy[1];
        dCanopy_depth_snow = aDummy[2];
        iIntercept_type = 2;
        dEt_canopy = 0.0;
      }
      break;
    case 3: // mixture of rain and snow
      {
        dCanopy_depth_capacity =
            calculate_canopy_depth_capacity(0, iMonth, iVegetation_type);
        aDummy = calculate_canopy_interception(dArea_in, dCanopy_depth_rain,
                                               dCanopy_depth_capacity, dLai,
                                               dRain_in);
        dThroughfall_rain = aDummy[0];
        dRain_net = aDummy[1];
        dCanopy_depth_rain = aDummy[2];
        dCanopy_depth_capacity =
            calculate_canopy_depth_capacity(1, iMonth, iVegetation_type);
        aDummy = calculate_canopy_interception(dArea_in, dCanopy_depth_snow,
                                               dCanopy_depth_capacity, dLai,
                                               dSnow_in);
        dThroughfall_snow = aDummy[0];
        dSnow_net = aDummy[1];
        dCanopy_depth_snow = aDummy[2];
        iIntercept_type = 3;
        dEt_canopy = 0.0;
      }
      break;
    default: // no rain and no snow
      {
        // calculate canopy based on previous lai
        calculate_canopy_evapotranspiration(iIntercept_type, iMonth, dEt_in);

        dCanopy_depth_capacity =
            calculate_canopy_depth_capacity(0, iMonth, iVegetation_type) * 0.5;
        aDummy = calculate_canopy_interception(
            dArea_in, dCanopy_depth_rain, dCanopy_depth_capacity, dLai, 0.0);
        dThroughfall_rain = aDummy[0];
        dRain_net = aDummy[1];
        dCanopy_depth_rain = aDummy[2];
        dCanopy_depth_capacity =
            calculate_canopy_depth_capacity(1, iMonth, iVegetation_type) * 0.5;
        aDummy = calculate_canopy_interception(
            dArea_in, dCanopy_depth_snow, dCanopy_depth_capacity, dLai, 0.0);
        dThroughfall_snow = aDummy[0];
        dSnow_net = aDummy[1];
        dCanopy_depth_snow = aDummy[2];
        iIntercept_type = 4;
      }
      break;
    }

  // adjust for temperature
  if (dTemperature_air_in > celsius_2_kelvin)
    {
      dRain_net = dRain_net + dSnow_net;
      dSnow_net = 0.0;
      dThroughfall_rain = dThroughfall_rain + dThroughfall_snow;
      dThroughfall_snow = 0.0;

      dCanopy_depth_rain = dCanopy_depth_rain + dCanopy_depth_snow;
      dCanopy_depth_snow = 0.0;

      dCanopy_storage_rain = dCanopy_storage_rain + dCanopy_storage_snow;
      dCanopy_storage_snow = 0.0;
    }
  else
    {
    }

  // 50==================================================
  // summarize
  // 50==================================================
  // flux
  dPrecipitation_net = dRain_net + dSnow_net;
  dThroughfall = dThroughfall_rain + dThroughfall_snow;
  // pool
  dCanopy_depth = dCanopy_depth_rain + dCanopy_depth_snow;
  if (dPrecipitation_net < tiny_value)
    {
      dPrecipitation_net = 0.0;
    }
  if (dThroughfall < tiny_value)
    {
      dThroughfall = 0.0;
    }
  if (dCanopy_depth < tiny_value)
    {
      dCanopy_depth = 0.0;
    }

  dCanopy_storage = dArea_in * dLai * dCanopy_depth;
  dCanopy_depth_hru = dCanopy_depth * dLai;

  if (dRain_net < tiny_value)
    {
      if (dSnow_net < tiny_value)
        {
          // nothing
          this->iPrecipitation_type = 4;
        }
      else
        {
          // only snow
          this->iPrecipitation_type = 2;
        }
    }
  else
    {
      if (dSnow_net < tiny_value)
        {
          // only rain
          this->iPrecipitation_type = 1;
        }
      else
        {
          // mixture
          this->iPrecipitation_type = 3;
        }
    }

  return error_code;
}

// 50==================================================
/*!
  <long-description>
  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
int interception::update_interception_parameter(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {

      dCanopy_depth_capacity_rain =
          aCanopy_depth_capacity_rain[iVegetation_type - 1];
      dCanopy_depth_capacity_snow =
          aCanopy_depth_capacity_snow[iVegetation_type - 1];
    }

  return error_code;
}

int interception::update_interception_status(int iVegetation_type)
{
  int error_code = 1;
  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      dLai_previous = 0.0;

      dCanopy_depth = 0.0;
      dCanopy_depth_hru = 0.0;
      dCanopy_depth_rain = 0.0;
      dCanopy_depth_snow = 0.0;
      dCanopy_storage = 0.0;
      dCanopy_storage_rain = 0.0;
      dCanopy_storage_snow = 0.0;
    }
  else
    {
      if (dLai < small_value)
        {
          dLai = 0.0;
          dCanopy_depth = 0.0;
          dCanopy_depth_hru = 0.0;
          dCanopy_depth_rain = 0.0;
          dCanopy_depth_snow = 0.0;
          dCanopy_storage = 0.0;
          dCanopy_storage_rain = 0.0;
          dCanopy_storage_snow = 0.0;
        }
      else
        {
        }
      dLai_previous = dLai;
    }
  return error_code;
}
} // namespace ecohydrology
