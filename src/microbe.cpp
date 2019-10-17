// 50==================================================
/*!
  \file microbe.cpp
  \brief

  <long description>

  \author
  \date 2016-10-03
*/
// 50==================================================
#include "microbe.h"

namespace ecohydrology
{
microbe::microbe()
{

  dLitter_carbon_decomposition_rate = missing_value;
  dLitter_heterotrophic_respiration_rate = missing_value;

  dSoil_heterotrophic_respiration_rate = missing_value;

  dSoil_doc_production_rate = missing_value;
  dSoil_doc_mineralization_rate = missing_value;
  dSoil_spsoc_mineralization_rate = missing_value;
}

microbe::~microbe() {}

double microbe::calculate_litter_carbon_decomposition_rate(
    double dLitter_temperature_in)
{
  double dLitter_carbon_decomposition_rate_out = 0.0;

  double dTemperature_celsius = dLitter_temperature_in + kelvin_2_celsius;
  double dQ10;
  double dummy;
  if (dTemperature_celsius < 0.0)
    {
      dLitter_carbon_decomposition_rate_out = 0.0;
    }
  else
    {
      dummy = (dTemperature_celsius -
               dLitter_carbon_decomposition_temperature_base) /
              10.0;
      dQ10 = pow(dLitter_carbon_decomposition_Q10, dummy);
      if (dQ10 > 8.0)
        {
          dQ10 = 8.0;
        }
      dLitter_carbon_decomposition_rate_out =
          dLitter_carbon_decomposition_rate_base * dQ10;

      if (dLitter_carbon_decomposition_rate_out < near_zero)
        {
          dLitter_carbon_decomposition_rate_out = 0.0;
        }
    }
  dLitter_carbon_decomposition_rate = dLitter_carbon_decomposition_rate_out;

  return dLitter_carbon_decomposition_rate_out;
}

double microbe::calculate_litter_heterotrophic_respiration_rate(
    double dLitter_temperature_in)
{
  double dLitter_heterotrophic_respiration_rate_out = 0.0;

  double dSoil_temperature_celsius = dLitter_temperature_in + kelvin_2_celsius;
  double dummy;
  double dQ10;

  if (dSoil_temperature_celsius < -10.0)
    {
      dLitter_heterotrophic_respiration_rate_out = 0.0;
    }
  else
    {
      dummy = (dSoil_temperature_celsius -
               dLitter_heterotrophic_respiration_temperature_base) /
              10.0;
      dQ10 = pow(dLitter_heterotrophic_respiration_Q10, dummy);
      if (dQ10 > 8.0)
        {
          dQ10 = 8.0;
        }

      dLitter_heterotrophic_respiration_rate_out =
          dLitter_heterotrophic_respiration_rate_base * dQ10;
      if (dLitter_heterotrophic_respiration_rate_out < near_zero)
        {
          dLitter_heterotrophic_respiration_rate_out = 0.0;
        }
    }
  dLitter_heterotrophic_respiration_rate =
      dLitter_heterotrophic_respiration_rate_out;

  return dLitter_heterotrophic_respiration_rate_out;
}

// 50==================================================
/*!
  calculate the doc production based on Xiaoliang's paper

  \param iFlag_anoxic   a flag whether the soil is under anosic condition or not
  \param dTemperature_soil_in   the soil temperature, unit, kelvin
  \return <ReturnValue>
*/
// 50==================================================
double microbe::calculate_soil_dissolved_organic_carbon_production_rate(
    int iFlag_anoxic, double dSoil_temperature_in)
{

  double dSoil_doc_production_rate_out = 0.0;
  double dSoil_temperature_celsius = dSoil_temperature_in + kelvin_2_celsius;
  double dummy;
  double dQ10;
  if (dSoil_temperature_celsius < -10.0)
    {
      dSoil_doc_production_rate_out = 0.0;
    }
  else
    {
      dummy =
          (dSoil_temperature_celsius - dSoil_doc_production_temperature_base) /
          10.0;
      // units: mg*g(-1)*day(-1)

      dQ10 = pow(dSoil_doc_production_Q10, dummy);
      if (dQ10 > 8.0)
        {
          dQ10 = 8.0;
        }
      dSoil_doc_production_rate_out = dSoil_doc_production_rate_base * dQ10;

      if (iFlag_anoxic == 1)
        {
          // dScalar_anaerobic_production between 0 and 1
          dSoil_doc_production_rate_out =
              dSoil_doc_production_rate_out * dScalar_anaerobic_production;
        }
      if (dSoil_doc_production_rate_out < near_zero)
        {
          dSoil_doc_production_rate_out = 0.0;
        }
    }

  dSoil_doc_production_rate = dSoil_doc_production_rate_out;
  return dSoil_doc_production_rate_out;
}

// 50==================================================
/*!
  calculate the doc mineralization based on Xiaoliang's paper

  \param iFlag_anoxic   a flag whether the soil is under anosic condition or not
  \param dTemperature_soil_in     the soil temperature, unit: kelvin
  \return <ReturnValue>
*/
// 50==================================================
double microbe::calculate_soil_dissolved_organic_carbon_mineralization_rate(
    int iFlag_anoxic, double dTemperature_soil_in)
{
  double dSoil_doc_mineralization_rate_out;
  double dSoil_temperature_celsius = dTemperature_soil_in + kelvin_2_celsius;

  double dQ10;
  double dummy;
  if (dSoil_temperature_celsius < -20.0)
    {
      dSoil_doc_mineralization_rate_out = 0.0;
    }
  else
    {
      dummy = (dSoil_temperature_celsius -
               dSoil_doc_mineralization_temperature_base) /
              10.0;

      dQ10 =  pow(dSoil_doc_mineralization_Q10, dummy);
      dSoil_doc_mineralization_rate_out =
          dSoil_doc_mineralization_rate_base * dQ10;

      if (iFlag_anoxic == 1)
        {
          dSoil_doc_mineralization_rate_out =
              dSoil_doc_mineralization_rate_out *
              dScalar_anaerobic_mineralization;
        }
      if (dSoil_doc_mineralization_rate_out < near_zero)
        {
          dSoil_doc_mineralization_rate_out = 0.0;
        }
    }

  dSoil_doc_mineralization_rate = dSoil_doc_mineralization_rate_out;
  return dSoil_doc_mineralization_rate_out;
}

// 50==================================================
/*!
  calculate the spsoc mineralization rate

  \param iFlag_anoxic
  \param dTemperature_soil_in
  \return <ReturnValue>
*/
// 50==================================================
double microbe::
    calculate_soil_sorbed_potentially_soluble_organic_carbon_mineralization_rate()
{

  dSoil_spsoc_mineralization_rate = dSoil_doc_mineralization_rate * dKs;

  return dSoil_spsoc_mineralization_rate;
}

// 50==================================================
/*!
//calculate kd
//microbe.kdc = (microbe.kda[dcmnt] / psiplusc) + microbe.kdb[dcmnt];
//dPercent_clay_in, the percentage of clay from soil module, unitless, between 0
and 1
//dPercent_silt_in, the percentage of clay from soil module, unitless, between 0
and 1

\param dClay_percentage_in
\param dSilt_percentage_in
\return <ReturnValue>
*/
// 50==================================================
double microbe::calculate_soil_heterotrophic_respiration_rate(
    double dClay_percentage_in, double dSilt_percentage_in,
    double dSoil_temperature_in)
{
  double dSoil_heterotrophic_respiration_rate_out;

  double dSoil_temperature_celsius = dSoil_temperature_in + kelvin_2_celsius;
  double dummy;
  double dQ10;
  double dRate_base;
  if (dSoil_temperature_celsius < -20.0)
    {
      dSoil_heterotrophic_respiration_rate_out = 0.0;
    }
  else
    {
      dummy = (dSoil_temperature_celsius -
               dSoil_heterotrophic_respiration_temperature_base) /
              10.0;
      dQ10 = pow(dSoil_heterotrophic_respiration_Q10, dummy);
      if (dQ10 > 8.0)
        {
          dQ10 = 8.0;
        }
      dRate_base = (dKda / (dClay_percentage_in + dSilt_percentage_in)) + dKdb;
      dSoil_heterotrophic_respiration_rate_out = dRate_base * dQ10;
      if (dSoil_heterotrophic_respiration_rate_out < near_zero)
        {
          dSoil_heterotrophic_respiration_rate_out = 0.0;
        }
    }

  dSoil_heterotrophic_respiration_rate =
      dSoil_heterotrophic_respiration_rate_out;

  return dSoil_heterotrophic_respiration_rate_out;
}

int microbe::initialize_microbe()
{
  int error_code = 1;

  dSoil_doc_mineralization_rate = 0.0;

  dSoil_spsoc_mineralization_rate = 0.0;

  return error_code;
}

// 50==================================================
/*!
  <long-description>

  \param iSoil_type
  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
int microbe::update_microbe_parameter(int iSoil_type, int iVegetation_type)
{
  int error_code = 1;
  if (iSoil_type < 1 || iSoil_type > nsoil_type)
    {
      // if there is no soil, there is no microbe activities
    }
  else
    {
      // soil
      dSoil_doc_production_rate_base =
          aSoil_doc_production_rate_base[iSoil_type - 1] * 24.0 * 1.0E-3;
      dSoil_doc_mineralization_rate_base =
          aSoil_doc_mineralization_rate_base[iSoil_type - 1] * 24.0;

      dSoil_heterotrophic_respiration_temperature_base =
          aSoil_temperature_base[iSoil_type - 1];
      dSoil_doc_production_temperature_base =
          aSoil_temperature_base[iSoil_type - 1];
      dSoil_doc_mineralization_temperature_base =
          aSoil_temperature_base[iSoil_type - 1];

      dSoil_doc_production_Q10 = aSoil_doc_production_Q10[iSoil_type - 1];
      dSoil_doc_mineralization_Q10 = aSoil_decomposition_Q10[iSoil_type - 1];
      dSoil_heterotrophic_respiration_Q10 =
          aSoil_heterotrophic_respiration_Q10[iSoil_type - 1];

      dScalar_anaerobic_production =
          aScalar_anaerobic_production[iSoil_type - 1];
      dScalar_anaerobic_mineralization =
          aScalar_anaerobic_mineralization[iSoil_type - 1];

      dKs = aKs[iSoil_type - 1];

      // nitrogen
      // dKn2 = aKn2[iSoil_type - 1];
      // decomposition doc dic
    }

  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      // if there is no vegetation, there is still microbe activities
      dPropftos = aPropftos[0];
      dKda = aKda[0];
      dKdb = aKdb[0];
      dLitter_heterotrophic_respiration_rate_base =
          aLitter_heterotrophic_respiration_rate_base[0];
      dLitter_carbon_decomposition_rate_base =
          aLitter_decomposition_rate_base[0];
    }
  else
    {
      dPropftos = aPropftos[iVegetation_type - 1];
      dKda = aKda[iVegetation_type - 1];
      dKdb = aKdb[iVegetation_type - 1];

      // litter
      dLitter_heterotrophic_respiration_rate_base =
          aLitter_heterotrophic_respiration_rate_base[iVegetation_type - 1];
      dLitter_carbon_decomposition_rate_base =
          aLitter_decomposition_rate_base[iVegetation_type - 1];
      // kd
      // dSoil_heterotrophic_respiration_out =
      // aRate_heterotrophic_respiration[iSoil_type - 1]; dLcclnc =
      // aLcclnc[iVegetation_type-1];
    }
  dLitter_carbon_decomposition_temperature_base = 0.0;
  dLitter_heterotrophic_respiration_temperature_base = -10.0;
  dLitter_carbon_decomposition_Q10 = 1.5;
  dLitter_heterotrophic_respiration_Q10 = 1.5;
  return error_code;
}

// 50==================================================
/*!
  calculate the nitrogen mineralization
  //microbe.netnmin[mon][day] = microbe.nminxclm(veg.cmnt, mon, day,
  pstate[I_SM], pstate[I_SOLC], pstate[I_SOLN],
  //pstate[I_AVLN], microbe.decay, microbe.rh[mon][day],
  dScalar_soil_moisture_in);

  \param dNitrogen_available_in
  \param inorganic
  \param dPercent_silt_in
  \param dRatio_carbon_2_nitrogen_in
  \param dScalar_soil_moisture_in
  \param module
  \param module
  \return <ReturnValue>
*/
// 50==================================================
//   int microbe::calculate_nitrogen_mineralization(double
//   dNitrogen_available_in, //inorganic
//                                                  double dPercent_clay_in,
//                                                  double dPercent_silt_in,
//                                                  double
//                                                  dRatio_carbon_2_nitrogen_in,
//                                                  double
//                                                  dScalar_soil_moisture_in,
//                                                  //calcuated from soil module
//                                                  double dSoil_carbon_in,
//                                                  //from soil module double
//                                                  dSoil_nitrogen_in //from
//                                                  soil module
//   )
//   {
//     int error_code = 1;
//     double dummy0;
//     double dummy1;
//     double dummy2;
//     double dummy3;
//     double dummy4;
//
//     if (dSoil_carbon_in < near_zero || dSoil_nitrogen_in < near_zero)
//     {
//       dNitrogen_mineralization = 0.0;
//     }
//     else
//     {
//       dummy0 = dNitrogen_available_in * dScalar_soil_moisture_in;
//       dummy1 = dummy0 / (dummy0 + dKn2);
//       dummy2 = dSoil_nitrogen_in / dSoil_carbon_in;
//       dummy3 = calculate_nitrogen_uptake_coefficient(
//               dPercent_clay_in,
//               dPercent_silt_in);
//       dummy4 = calculate_decay_coefficient();
//       //double check!!!
//       dNitrogen_mineralization = (dummy3 * dummy1 * dummy4 + dummy2) *
//       dSoil_heterotrophic_respiration; if (dNitrogen_mineralization >= 0.0)
//       {
//         dNitrogen_mineralization = dNitrogen_mineralization * dummy2 *
//         dRatio_carbon_2_nitrogen_in;
//       } // organic N -> inorganic N: adjust flux, e.g. reduce flux if organic
//       N < mean state else
//       {
//         dNitrogen_mineralization = dNitrogen_mineralization / dummy2 /
//         dRatio_carbon_2_nitrogen_in;
//       } // inorganic N -> organic N: adjust fluxe, e.g. increase flux if
//       organic N < mean state
//
//     }
//     return error_code;
//   }

// 50==================================================
/*!
//nup from tem
//inputs:
//dPercent_clay_in, the percentage of clay from soil module, unitless, between 0
and 1
//dPercent_silt_in, the percentage of clay from soil module, unitless, between 0
and 1

\param dPercent_clay_in
\param dPercent_silt_in
\return <ReturnValue>
*/
// 50==================================================
//    double microbe::calculate_nitrogen_uptake_coefficient(double
//    dPercent_clay_in,
//                                                          double
//                                                          dPercent_silt_in)
//    {
//      double dCoefficient_nitrogen_uptake_out = (dNupa / (dPercent_clay_in +
//      dPercent_silt_in))
//              + dNupb;
//      return dCoefficient_nitrogen_uptake_out;
//    }

// 50==================================================
/*!
  calculate the soil organic matter decay parameter
  the algorithm should be based on vegetation type/soil type?

  \param
  \return <ReturnValue>
*/
// 50==================================================
//  double microbe::calculate_decay_coefficient()
//  {
//    double dCoefficient_Decay = 0.26299 + (1.14757 * dPropftos)
//            - (0.42956 * pow(dPropftos, 2.0)); // McGuire1995JBd
//    return dCoefficient_Decay;
//  }
} // namespace ecohydrology