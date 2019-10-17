// 50==================================================
/*!
  \file litter.h
  \brief
  <long description>
  \author Chang Liao
  \date 2017-01-31
*/
// 50==================================================
#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <string>
// 50==================================================
#include "global.h"
#include "microbe.h"

using namespace std;
// 50==================================================
// the max doc concentration based on field measurements
const double dDoc_concentration_maximal = 0.05; // unit kg m(-3)
// 50==================================================
namespace ecohydrology
{
class litter
{
public:
  litter();

  ~litter();

  int iFlag_run_carbon_cascade;
  int iFlag_run_water_cascade;
  int iVegetation_type;
  int iVegetation_type_default;

  double dLitter;

  double dLitter_carbon;
  double dLitter_carbon_influx;
  double dLitter_carbon_outflux;
  double dLitter_carbon_outflux_humus;

  double dLitter_carbon_undecomposed;
  double dLitter_carbon_undecomposed_influx;
  double dLitter_carbon_undecomposed_outflux;

  double dLitter_carbon_decomposed; //total decomposed from undecomposed

  double dLitter_carbon_decomposed_influx;
  double dLitter_carbon_decomposed_ioc_influx;
  double dLitter_carbon_decomposed_soc_influx;

  double dLitter_carbon_decomposed_outflux;

  double dLitter_carbon_decomposed_storage_ioc;

  double dLitter_carbon_decomposed_storage_soc;
  double dLitter_carbon_decomposed_ioc_2_soc;

  double dLitter_carbon_decomposed_ioc_2_humus;
  double dLitter_carbon_decomposed_soc_2_humus;
  double dLitter_carbon_decomposed_soc_2_doc;

  double dLitter_heterotrophic_respiration;
  double dLitter_heterotrophic_respiration_undecomposed;
  double dLitter_heterotrophic_respiration_decomposed_ioc;
  double dLitter_heterotrophic_respiration_decomposed_soc;

  double dCoefficient_carbon_ioc_2_soc;
  double dCoefficient_carbon_ioc_2_humus;
  double dCoefficient_carbon_soc_2_humus;
  double dLitter_doc_storage;
  double dLitter_doc_storage_concentration;

  double dLitter_doc_inflow;
  double dLitter_doc_outflow;
  double dLitter_doc_storage_area;
  double dLitter_doc_outflow_area;
  double dLitter_doc_inflow_upslope;

  double dLitter_doc_inflow_external;
  double dLitter_doc_outflow_concentration;
  double dLitter_doc_outflow_downslope;

  double dLitter_doc_outflow_leaching;

  double dSurface_runoff_inflow;
  double dSurface_runoff_inflow_upslope;
  double dSurface_runoff_outflow_downslope;

  microbe cMicrobe;

  int calculate_litter_heterotrophic_respiration(double dLitter_temperature_in);

  int calculate_litter_carbon_decomposition(double dTemperature_in);

  // int calculate_litter_nitrogen_decomposition();
  int calculate_litter_decomposed_carbon_soc_budget();

  int calculate_litter_decomposed_carbon_ioc_budget();

  int calculate_litter_decomposed_ioc_carbon_to_humus();

  int calculate_litter_decomposed_ioc_carbon_to_soc();

  int calculate_litter_decomposed_soc_to_humus();

  int calculate_litter_decomposed_soc_to_doc(
          double dSurface_runoff_inflow_in, double dSurface_runoff_storage_in);

  int calculate_litter_dissolved_organic_carbon(
      double dSurface_runoff_outflow_in,
      double dSurface_runoff_outflow_infiltration_in);

  int initialize_litter(eColumn_type eCU_type);

  int run_litter_model(double dSurface_runoff_inflow_in,
                       double dSurface_runoff_outflow_in,
                       double dSurface_runoff_outflow_infiltration_in,
                       double dSurface_runoff_storage_in,
                       double dTemperature_in);

  int update_litter_status(double dCarbon_litterfall_in);

  int update_litter_parameter(int iFlag_run_carbon_cascade,
                              int iFlag_run_water_cascade,
                              int iSoil_type,
                              int iVegetation_type);

  // 50==================================================
  // unused
  // 50==================================================
  // double dPercentage_carbon; // the percentage of carbon in litter

  // int calculate_litter_budget(double dLitterfall_carbon_in,
  //                            double dLitterfall_nitrogen_in);
  // the total litter nitrogen mass, units: kgC*m(-2)
  // double dNitrogen_storage;
  // double dDecompose_rate_nitrogen;
  // double dNitrogen_decomposed_storage;
  // the decomposed nitrogen, unit: kg C*m(-2) per day
  // double dNitrogen_decomposed_influx;
  // 50==================================================
  // cascade
  // 50==================================================
  // unused for solid form yet
  // double dCarbon_upslope;
  // double dNitrogen_upslope;
  // int calculate_litter_downslope();
  // int calculate_litter_nitrogen_budget(double dNitrogen_litterfall_in);
};
} // namespace ecohydrology
