// 50==================================================
/*!
  \file   microbe.h
  \brief

  <long description>

  \author Chang Liao
  \date   2017-02-06
*/
// 50==================================================
#pragma once

#include <array>
#include <cmath>
#include <iostream>
// 50==================================================
#include "dimension.h"
#include "global.h"

using namespace std;

// 50==================================================
// parameters
// 50==================================================
// q10 from TEM

// 50==================================================
// kd
// respiration rate
// 50==================================================

const std::array<double, nvegetation_type> aKda = {{
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
    0.00046835,
}};
const std::array<double, nvegetation_type> aKdb = {
    {0.00300743, 0.00300743, 0.00300743, 0.00300743, 0.00300743, 0.00300743,
     0.00300743, 0.00300743, 0.00300743, 0.00300743, 0.00300743, 0.00300743}};
// 50==================================================
const std::array<double, nsoil_type> aKn2 = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nupa = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nupb = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
// soil decay parameter
const std::array<double, nvegetation_type> aPropftos = {
    {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2}};

// 50==================================================
const std::array<double, nvegetation_type>
    aLitter_heterotrophic_respiration_rate_base = {
        {1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4,
         1.0E-4, 1.0E-4, 1.0E-4}};
const std::array<double, nvegetation_type> aLitter_decomposition_rate_base = {
    {1.0E-2, 1.0E-2, 1.0E-2, 1.0E-2, 1.0E-2, 1.0E-2, 1.0E-2, 1.0E-2, 1.0E-2,
     1.0E-2, 1.0E-2, 1.0E-2}};

// the Pbasal parameter mg/g /day
const std::array<double, nsoil_type> aSoil_doc_production_rate_base = {
    {1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4,
     1.0E-4, 1.0E-4, 1.0E-4, 1.0E-4}};
// the mu parameter
const std::array<double, nsoil_type> aSoil_doc_mineralization_rate_base = {
    {2.0E-4, 2.0E-4, 2.0E-4, 2.0E-4, 2.0E-4, 2.0E-4, 2.0E-4, 2.0E-4, 2.0E-4,
     2.0E-4, 2.0E-4, 2.0E-4, 6.0E-4}};

const std::array<double, nsoil_type> aSoil_heterotrophic_respiration_Q10 = {
    {1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5}};

const std::array<double, nsoil_type> aSoil_decomposition_Q10 = {
    {1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7}};
const std::array<double, nsoil_type> aSoil_doc_production_Q10 = {
    {1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7}};

// temperature parameter, obtained from xiaoliang's paper
const std::array<double, nsoil_type> aSoil_temperature_base = {
    {20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
     20.0}};

// the kanp parameter
const std::array<double, nsoil_type> aScalar_anaerobic_production = {
    {0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07,
     0.07}};
// the kanmu parameter
const std::array<double, nsoil_type> aScalar_anaerobic_mineralization = {
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};

const std::array<double, nsoil_type> aKs = {
    {0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8}};

// 50==================================================
namespace ecohydrology
{
class microbe
{
public:
  microbe();

  ~microbe();
  // litter

  double dLitter_carbon_decomposition_rate;
  double dLitter_heterotrophic_respiration_rate;
  // base rate
  double dLitter_heterotrophic_respiration_rate_base;
  double dLitter_carbon_decomposition_rate_base;
  // temperature  base

  double dLitter_carbon_decomposition_temperature_base;
  double dLitter_heterotrophic_respiration_temperature_base;

  // q10
  double dLitter_carbon_decomposition_Q10;
  double dLitter_heterotrophic_respiration_Q10;
  // soil
  double dSoil_heterotrophic_respiration_rate;

  double dSoil_doc_production_rate;
  double dSoil_doc_mineralization_rate;
  double dSoil_spsoc_mineralization_rate;

  // temperature base
  double dSoil_heterotrophic_respiration_temperature_base;
  double dSoil_doc_production_temperature_base;
  double dSoil_doc_mineralization_temperature_base;

  // base rate
  double dSoil_doc_production_rate_base;

  double dSoil_doc_mineralization_rate_base;

  // q10
  double dSoil_doc_production_Q10;
  double dSoil_doc_mineralization_Q10;
  double dSoil_heterotrophic_respiration_Q10;

  double dScalar_anaerobic_production;
  double dScalar_anaerobic_mineralization;

  // unknown
  double dKda;
  double dKdb;

  double dKs;
  double dPropftos; // from tem, need to be checked

  // litter
  double
  calculate_litter_carbon_decomposition_rate(double dLitter_temperature_in);

  double calculate_litter_heterotrophic_respiration_rate(
      double dLitter_temperature_in);

  // soil
  double calculate_soil_dissolved_organic_carbon_production_rate(
      int iFlag_anoxic, double dTemperature_soil_in);

  double calculate_soil_dissolved_organic_carbon_mineralization_rate(
      int iFlag_anoxic, double dTemperature_soil_in);

  double
  calculate_soil_sorbed_potentially_soluble_organic_carbon_mineralization_rate();

  double
  calculate_soil_heterotrophic_respiration_rate(double dClay_percentage_in,
                                                double dSilt_percentage_in,
                                                double dSoil_temperature_in);

  int initialize_microbe();

  int update_microbe_parameter(int iSoil_type, int iVegetation_type);

  // from tem, unknown unit yet
  // double dLcclnc;

  // double dKn2;
  // double dNupa, dNupb;
  // q10 parameter for doc/dic decomposition, unitless
  // soil decay

  // respiration
  // the doc production rate, unit: unitless, it is a fraction of soil carbon?

  // the doc mineralization rate, unit: unitless, it is a fraction of soil
  // carbon?  double dSoil_heterotrophic_respiration_rate;  double
  // dMicrobe_decay_coefficient; //the soil decay  double
  // dNitrogen_uptake_coefficient;  double dNitrogen_mineralization;  double
  // dNitrogen_uptake;  double calculate_decay_coefficient();  double
  // calculate_nitrogen_uptake_coefficient(double dPercent_clay_in, 	double
  // dPercent_silt_in);

  //	int calculate_nitrogen_mineralization(double dNitrogen_available_in,
  //		double dpercent_clay_in,
  //		double dPercent_silt_in,
  //		double dRatio_carbon_2_nitrogen_in,
  //		double dScalar_soil_moisture_decomposition_in,
  //		double dSoil_organic_carbon_in,
  //		double dSoil_organic_nitrogen_in);
};
} // namespace ecohydrology
