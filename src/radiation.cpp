// 50==================================================
/*!
  <long-description>

  \param void
*/
// 50==================================================
#include "radiation.h"
namespace ecohydrology
{
// 50==================================================
/*!
      <long-description>

      \param void
*/
// 50==================================================

radiation::radiation()
{
  dObliquity = 1.0;
  dShortwave_radiation_potential = 0.0;
  dShortwave_radiation_actual = 0.0;
  dPar = 0.0;
  dSolar_declination = 0.0;
  dSun_hours = 0.0;
  dTime_sunrise = 0.0;
  dTime_sunset = 0.0;
}
radiation::~radiation() {}

// 50==================================================
/*!
      <calculate actual shortwave>

      \param iMonth month of the year
      \param dSlope_in slope of the grid, unit degree
      \param dTemperature_max_in maximum temperature unit: kelvin
      \return <actual shortwave radiation, units: joule per square meter per
   day>
*/
// 50==================================================

double
radiation::calculate_actual_shortwave_radiation(int iMonth, double dSlope_in,
                                                double dTemperature_max_in)
{
  int kp, kp1;
  // convert temperature unit to be the same with original equation
  double dTemperature_max_fahrenheit =
      convert_from_kelvin_to_fahrenheit(dTemperature_max_in);
  double dday = dday_slope[iMonth - 1] * dTemperature_max_fahrenheit +
                dday_intcp[iMonth - 1] + 1.0;
  double dAdjustment_temperature;
  double dAdjustment_precipitation = 1.0;
  double dSlope_radian = atan(dSlope_in / 180.0 * pi);
  double ddayi;
  double dShortwave_radiation_actual_out;
  if (dday < 1.0)
    {
      dday = 1.0;
    }
  if (dday < 26.0)
    {
      kp = int(dday);
      ddayi = double(kp);
      kp1 = kp + 1;
      // calculate adjustment
      dAdjustment_temperature =
          solf[kp - 1] + ((solf[kp1 - 1] - solf[kp - 1]) * (dday - ddayi));
      if (dAdjustment_temperature > dAdjustment_temperature_max)
        {
          dAdjustment_temperature = dAdjustment_temperature_max;
        }
    }
  else
    {
      dAdjustment_temperature = dAdjustment_temperature_max;
    }
  // precipitation effects not considered yet
  // total effects
  dShortwave_radiation_actual_out =
      dAdjustment_precipitation * dAdjustment_temperature *
      dShortwave_radiation_potential / (cos(atan(dSlope_radian)));

  // 50==================================================
  // quality control
  // 50==================================================

  if (dShortwave_radiation_actual_out < near_zero)
    {
      dShortwave_radiation_actual_out = 0.0;
    }

  this->dShortwave_radiation_actual = dShortwave_radiation_actual_out;
  return dShortwave_radiation_actual_out;
}

// 50==================================================
/*!
      <calculate photosyntheis active radiation>

      \param
      \return <dPAR, units: joule per square meter per day>
*/
// 50==================================================

double radiation::calculate_photosyntheis_active_radiation()
{
  double dPar_out;
  if (dShortwave_radiation_actual < near_zero)
    {
      dPar_out = 0.0;
    }
  else
    {
      dPar_out = dShortwave_radiation_actual * 0.45; // a simplfied method
    }
  this->dPar = dPar_out;
  return dPar_out;
}

// 50==================================================
/*!
      <calculate the potential solar radiation>

      \param iDay_doy unit: day of the year
      \param dAspect_in unit: degree
      \param dLatitude_in unit: degree
      \param dSlope_in unit: degree
      \return <potential radiation, units: joule per square meter per day>
*/
// 50==================================================

double radiation::calculate_potential_shortwave_radiation(int iDay_doy,
                                                          double dAspect_in,
                                                          double dLatitude_in,
                                                          double dSlope_in)
{
  double dShortwave_radiation_potential_out;
  // input quality control
  // settings
  double dThreshold_slope = 0.0001;
  double dThreshold_hour = 0.001;
  // temperal variables
  double dAspect_radian;               // unit radian
  double dDifference_longitude_radian; // unit radian
  double dLatitude_equivalent_radian;  // unit radian
  double dLatitude_radian;             // unit radian
  double numerator, denominator;
  double dRadiation_hourly; // units: joule per square meter per hour
  double dSlope_radian;     // unit radian

  double dSolar_declination_radian; // unit radian
  double dSolar_constant_hourly;    // units: joule per square meter per hour
  double dTime_sunrise_horizontal;
  double dTime_sunset_horizontal;
  double dTime_sunrise_equivalent;
  double dTime_sunset_equivalent;
  std::array<double, 2> aDummy = {};
  // declination
  dSolar_declination = calculate_solar_declination(iDay_doy);
  dSolar_declination_radian = dSolar_declination / 180.0 * pi;
  // obliquity
  dObliquity = calculate_sun_obliquity(iDay_doy);
  // if the slope is less than the threshold, it is not considered.
  dAspect_radian = dAspect_in / 180.0 * pi;
  dLatitude_radian = dLatitude_in / 180.0 * pi;
  dSlope_radian = atan(dSlope_in / 180.0 * pi);
  // equivalent latitude of equivalent slope
  dLatitude_equivalent_radian =
      asin(sin(dSlope_radian) * cos(dAspect_radian) * cos(dLatitude_radian) +
           cos(dSlope_radian) * sin(dLatitude_radian));
  numerator = sin(dSlope_radian) * sin(dAspect_radian);
  denominator =
      cos(dSlope_radian) * cos(dLatitude_radian) -
      cos(dAspect_radian) * sin(dSlope_radian) * sin(dLatitude_radian);
  if (denominator < tiny_value)
    {
      denominator = tiny_value;
    }
  dDifference_longitude_radian = atan(numerator / denominator);
  /*if (denominator < 0.0)
  {
          dDifference_longitude_radian = dDifference_longitude_radian + pi;
  }*/
  // This is adjusted to express the variability of available insolation as
  // a function of the earth-sun distance. Lee, 1963, p 16.
  // Obliquity is the obliquity of the ellipse of the earth's orbit around the
  // sun. E is also called the radius vector of the sun (or earth) and is the
  // ratio of the earth-sun distance on a day to the mean earth-sun distance.
  // from second to hour, units: joule per square meter per hour
  dSolar_constant_hourly = dSolar_constant * 3600.0;
  dRadiation_hourly = dSolar_constant_hourly / (dObliquity * dObliquity);
  aDummy = calculate_sunrise_sunset_time(dLatitude_equivalent_radian,
                                         dSolar_declination_radian);
  dTime_sunrise_horizontal = aDummy[0];
  dTime_sunset_horizontal = aDummy[1];
  aDummy = calculate_sunrise_sunset_time(dLatitude_equivalent_radian,
                                         dSolar_declination_radian);
  dTime_sunrise_equivalent = aDummy[0] - dDifference_longitude_radian;
  dTime_sunset_equivalent = aDummy[1] - dDifference_longitude_radian;
  // adjust the sunrise time
  if (dTime_sunrise_equivalent < dTime_sunrise_horizontal)
    {
      dTime_sunrise = dTime_sunrise_horizontal;
    }
  else
    {
      dTime_sunrise = dTime_sunrise_equivalent;
    }
  // adjust sunset time
  if (dTime_sunset_equivalent > dTime_sunset_horizontal)
    {
      dTime_sunset = dTime_sunset_equivalent;
    }
  else
    {
      dTime_sunset = dTime_sunset_horizontal;
    }
  // when slope is small enough to be ignored.
  if (abs(dSlope_radian) < dThreshold_slope)
    {
      dDifference_longitude_radian = 0.0;
      // Swift, 1976, equation 6
      dShortwave_radiation_potential_out =
          calculate_potential_solar_radiation_on_surface(
              dLatitude_radian, dDifference_longitude_radian, dRadiation_hourly,
              dSolar_declination_radian, dTime_sunrise_horizontal,
              dTime_sunset_horizontal);
      dSun_hours =
          (dTime_sunset_horizontal - dTime_sunrise_horizontal) / pi * 12.0;
    }
  else // when slope cannot be ignored
    {
      if (dTime_sunset < dTime_sunrise)
        {
          dTime_sunrise = 0.0;
          dTime_sunset = 0.0;
        }
      dTime_sunrise_equivalent = dTime_sunrise_equivalent + pi * 2.0;
      if (dTime_sunrise_equivalent < dTime_sunset_horizontal)
        {
          dShortwave_radiation_potential_out =
              calculate_potential_solar_radiation_on_surface(
                  dLatitude_equivalent_radian, dDifference_longitude_radian,
                  dRadiation_hourly, dSolar_declination, dTime_sunrise,
                  dTime_sunset) +
              calculate_potential_solar_radiation_on_surface(
                  dLatitude_radian, dDifference_longitude_radian,
                  dRadiation_hourly, dSolar_declination_radian,
                  dTime_sunrise_equivalent, dTime_sunset_horizontal);
          dSun_hours = (dTime_sunset - dTime_sunrise + dTime_sunset_horizontal -
                        dTime_sunrise_equivalent) /
                       pi * 12.0;
        }
      else
        {
          dTime_sunset_equivalent = dTime_sunset_equivalent - pi * 2.0;
          if (dTime_sunset_equivalent > dTime_sunrise_horizontal)
            {
              dShortwave_radiation_potential_out =
                  calculate_potential_solar_radiation_on_surface(
                      dLatitude_equivalent_radian, dDifference_longitude_radian,
                      dRadiation_hourly, dSolar_declination_radian,
                      dTime_sunrise, dTime_sunset) +
                  calculate_potential_solar_radiation_on_surface(
                      dLatitude_radian, dDifference_longitude_radian,
                      dRadiation_hourly, dSolar_declination_radian,
                      dTime_sunrise_horizontal, dTime_sunset_equivalent);
              dSun_hours =
                  (dTime_sunset - dTime_sunrise + dTime_sunset_equivalent -
                   dTime_sunrise_horizontal) /
                  pi * 12.0;
            }
          else
            {
              dShortwave_radiation_potential_out =
                  calculate_potential_solar_radiation_on_surface(
                      dLatitude_equivalent_radian, dDifference_longitude_radian,
                      dRadiation_hourly, dSolar_declination_radian,
                      dTime_sunrise, dTime_sunset);
              dSun_hours = (dTime_sunset - dTime_sunrise) / pi * 12.0;
            }
        }
    }
  if (dShortwave_radiation_potential_out < near_zero)
    {
      dShortwave_radiation_potential_out = 0.0;
    }

  if (dSun_hours < dThreshold_hour)
    {
      dSun_hours = 0.0;
    }

  this->dShortwave_radiation_potential = dShortwave_radiation_potential_out;
  return dShortwave_radiation_potential_out;
}

// 50==================================================
/*!
      <long-description>

      \param dLatitude_radian_in unit radiaon
      \param dLatitude_difference_radian_in unit radiaon
      \param dRadiation_hourly_in hourly solar radiaon, units: joulr per square
   meter per hour \param dSolar_declination_radian_in unit radian \param
   dSunrise_time_in unit radian \param dSunset_time_in unit radian \return
   <daily potential solar radiation units: joule per square meter per day>
*/
// 50==================================================

double radiation::calculate_potential_solar_radiation_on_surface(
    double dLatitude_radian_in, double dLatitude_difference_radian_in,
    double dRadiation_hourly_in, double dSolar_declination_radian_in,
    double dSunrise_time_in, double dSunset_time_in)
{
  // 50==================================================
  // This is the radian angle version of FUNC3 (eqn 6) from Swift, 1976
  // or Lee, 1963 equation 5.
  double dRadiation_potential_out;
  dRadiation_potential_out =
      dRadiation_hourly_in * 12.0 / pi *
      (sin(dSolar_declination_radian_in) * sin(dLatitude_radian_in) *
           (dSunset_time_in - dSunrise_time_in) +
       cos(dSolar_declination_radian_in) * cos(dLatitude_radian_in) *
           (sin(dSunset_time_in + dLatitude_difference_radian_in) -
            sin(dSunrise_time_in + dLatitude_difference_radian_in)));

  // 50==================================================
  // quality control
  // 50==================================================
  if (dRadiation_potential_out < near_zero)
    {
      dRadiation_potential_out = 0.0;
    }
  return dRadiation_potential_out;
}

// 50==================================================
/*!
      <calculate the solar declination>

      \param iDay day of the year
      \return <solar declination unit: degree>
*/
// 50==================================================
double radiation::calculate_solar_declination(int iDay)
{
  double dSolar_declination_out;
  double Ert = radian * (iDay - 1.0);
  dSolar_declination_out = 0.006918 - 0.399912 * cos(Ert) +
                           0.070257 * sin(Ert) - 0.006758 * cos(Ert) +
                           0.000907 * sin(2 * Ert) - 0.002697 * cos(3 * Ert) +
                           0.00148 * sin(3 * Ert);
  // convert unit from radian to degree
  dSolar_declination_out = dSolar_declination_out / pi * 180.0;

  if (dSolar_declination_out >= -30.0 && dSolar_declination_out <= 30.0)
    {
    }
  else
    {
      dSolar_declination_out = 0.0;
      std::cout << "declination angle error" << std::endl;
    }
  this->dSolar_declination = dSolar_declination_out;
  return dSolar_declination_out;
}

// 50==================================================
/*!
      < This is the sunrise equation
      inputs dLatitude_radian_in unit radian
      dSolor_declination_radian unti radian
      Solar_declination is the declination of the sun on a day
      T is the angle hour from the local meridian (local solar noon) to the
      sunrise (negative) or sunset (positive). The Earth rotates at the angular
      speed of 15 degree per hour (360 degree in 24 hour ) and, therefore, T/15
      gives the time of sunrise as the number of hours before the local
      noon, or the time of sunset as the number of hours after the local noon.
      Here the term local noon indicates the local time when the sun is exactly
   to the south or north or exactly overhead.>

      \param dLatitude_radian_in latitde of the grid, unit: radian
      \param dSolar_declination_radian_in solar declination, unit: radian
*/
// 50==================================================
std::array<double, 2>
radiation::calculate_sunrise_sunset_time(double dLatitude_radian_in,
                                         double dSolar_declination_radian_in)
{
  std::array<double, 2> aSunrise_sunset_time_out = {{-9999.0, -9999.0}};
  double time;
  double dummy =
      -1.0 * atan(dLatitude_radian_in) * tan(dSolar_declination_radian_in);
  if (dummy < -1.0)
    {
      time = pi;
    }
  else
    {
      if (dummy > 1.0)
        {
          time = 0.0;
        }
      else
        {
          time = acos(dummy);
        }
    }
  aSunrise_sunset_time_out.at(0) = -1.0 * time;
  aSunrise_sunset_time_out.at(1) = time;
  return aSunrise_sunset_time_out;
}

// 50==================================================
/*!
      <estimate the obliquity>

      \param iDay day of year
      \return <obliquity of the sun's ecliptic, unit: fraction between 0 and 1>
*/
// 50==================================================

double radiation::calculate_sun_obliquity(int iDay)
{
  double dObliquity_out;
  dObliquity_out = 1.0 - dEccentricy * cos((double)iDay - 3.0) * radian;

  if (dObliquity_out < 0.0 || dObliquity_out > 1.0)
    {
      // something is wrong
      dObliquity_out = 1.0;
    }

  this->dObliquity = dObliquity_out;
  return dObliquity_out;
}

// 50==================================================
/*!
      <long-description>
      \param iDay_doy the day of the year,
      \param iMonth_ymd the month of the year
      \param dAspect_in aspect of the grid, unit: degree
      \param dLatitude_in latitude of the grid, unit: degree
      \param dSlope_in slope of the grid, unit: degree
      \param dTemperature_max_in the maximum temperature of the grid, unit:
   kelvin \return <ReturnValue>
*/
// 50==================================================

int radiation::run_radiation_model(int iDay_doy, int iMonth_ymd,
                                   double dAspect_in, double dLatitude_in,
                                   double dSlope_in, double dTemperature_max_in)
{
  int error_code = 1;
  calculate_potential_shortwave_radiation(iDay_doy, dAspect_in, dLatitude_in,
                                          dSlope_in);
  calculate_actual_shortwave_radiation(iMonth_ymd, dSlope_in,
                                       dTemperature_max_in);
  calculate_photosyntheis_active_radiation();
  return error_code;
} // end of
  // run========================================================================
} // namespace ecohydrology