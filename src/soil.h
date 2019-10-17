// 50==================================================
/*!
  \file soil.h
  \brief
  The soil module consists of three major child modules.
  A water module
  A thermal module
  A carbon module
  \author Chang Liao
  \date 2016-09-30
*/
// 50==================================================
#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
// 50==================================================
#include "global.h"
#include "mathematics.h"
#include "microbe.h"
#include "miscellaneous.h"

using namespace std;

// 50==================================================
// Currently the soil water model consist of two different models.
// one of them is from the prms model and the other one is from
// item (clm).
// 50==================================================
// The first soil water model parameters for soil water model, established from
// PRMS 50==================================================

const std::array<double, nsoil_type> aCoefficient_fast_linear_base = {
        0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.7};
const std::array<double, nsoil_type> aCoefficient_fast_square_base = {
        0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8};

const std::array<double, nsoil_type> aCoefficient_slow_linear_base = {
        0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01,
        0.01, 0.01, 0.01, 0.01, 0.01, 0.8};
const std::array<double, nsoil_type> aCoefficient_slow_square_base = {
        0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.8};

const std::array<double, nsoil_type> aRate_gravity_to_groundwater_base = {
        0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.5};
const double dGravity_to_groundwater_coef = 1.0;
// 50==================================================
// the second soil water model from iTEM
// 50==================================================
// 50==================================================
// soil thermal module
// 50==================================================
// 50==================================================
// parameter for soil carbon model, established from TEM
// 50==================================================
// meter
const std::array<double, nsoil_type> aSoil_thicknesses = {
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.2}};

const std::array<double, nsoil_type> aSoil_density = {
        100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};


const std::array<double, nsoil_type> aFraction_preferential = {
        0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.3};


// moisture limitation, fraction
const std::array<double, nsoil_type> aMoisture_min = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
const std::array<double, nsoil_type> aMoisture_max = {
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
const std::array<double, nsoil_type> aMoisture_optimization = {
        {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5}};

const std::array<double, nsoil_type> aKd = {
        {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.033}};

const std::array<double, nsoil_type> aCoefficient_desorption = {1.0E-3,
        1.0E-3, 1.0E-3, 1.0E-3, 1.0E-3, 1.0E-3, 1.0E-3, 1.0E-3,
        1.0E-3, 1.0E-3, 1.0E-3, 1.0E-3,
        1.0E-3};

const std::array<double, nsoil_type> aFraction_field_capacity = {0.4,
        0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4,
        0.4, 0.4, 0.4, 0.4,
        0.2};

const std::array<double, nsoil_type> aFraction_wilting_point = {0.1,
        0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1,
        0.1, 0.1, 0.1, 0.1,
        0.01};




// 50==================================================
namespace ecohydrology
{
    class soil
    {
    public:
        soil();

        ~soil();

        int iFlag_run_carbon_cascade;
        int iFlag_run_water_cascade;
        int iSoil_type;
        int iSoil_type_default;
        // the flag to indicate whether soil is under anoxic condistion or not
        int iFlag_anoxic;

        double dSoil_density;

        double dSoil_porosity;

        double dSoil_clay_percentage;

        double dSoil_silt_percentage;

        double dSoil_sand_percentage;

        double dSoil_moisture_volume;

        double dSoil_temperature;

        double dSoil_thickness;
        double dSoil_water_storage;

        // the upper threshold of preferential storage,
        // the maximum water content the soil can hold, unit: meter
        double dThreshold_saturation;
        // the lower threshold of preferential storage, unit: meter
        double dThreshold_preferential;
        double dThreshold_field_capacity;
        // the field capacity of soil, unit: meter
        double dThreshold_wilting_point;
        // 50==================================================
        // the max storage in preferential reservoir, unit: meter
        double dSoil_storage_preferential_max;
        double dSoil_storage_moisture_max;
        // max storage in capillary recharge zone
        double dSoil_storage_recharge_zone_max;
        // the max storage in gravity reservoir, unit: meter
        double dSoil_storage_gravity_max;
        // current storage in preferential reservoir, unit:meter
        double dSoil_water_storage_preferential;
        double dSoil_water_storage_moisture;
        // storage in capillary recharge zone, unit: meter
        double dSoil_water_storage_recharge_zone;
        // current storage in gravity reservoir, unit:  meter
        double dSoil_water_storage_gravity;
        // storage in capillary lower zone, unit: meter
        double dStorage_soil_moisture_lower;

        double dSoil_water_outflow_dunnian_runoff;
        // the total dunnian runoff, which contains two components from gravity and
        // preferential reservoirs, unit: meter double dDunnian_runoff_outflow; double
        // dDunnian_runoff_storage; double dDunnian_runoff_inflow_upslope; dunnian
        // runoff from upslope
        double dSoil_water_dunnian_runoff_gravity;
        // the dunnian runoff from preferential reservoir, the first component, unit:
        // meter
        double dSoil_water_dunnian_runoff_preferential;
        // double dDunnian_runoff_downslope;
        // fraction of pervious surface, unitless
        double dFraction_pervious;
        // the fraction of preferential reservoir in the upper soil zone, unitless
        double dFraction_preferential;
        // inflow to the preferential reservoir, unit: meter
        double dPreferential_inflow;
        // fast interflow from preferential reservoir, unit: meter
        double dSoil_interflow_fast;

        // inflow to the gravity reservoir, unit: meter
        double dGravity_inflow;
        // outflow to preferential reservoir, unit: meter
        double dGravity_to_preferential;
        // discharge to groundwater, unit: meter
        double dSoil_gravity_drainage;
        // interflow from gravity reservoir, unit: meter
        double dSoil_interflow_slow;

        // inflow to the capillary reservoir, unit: meter
        double dCapillary_inflow;

        double dSoil_interflow_fast_downslope;
        double dSoil_interflow_fast_upslope;
        double dSoil_interflow_slow_downslope;
        double dSoil_interflow_slow_upslope;

        double dSoil_water_interflow;
        double dSoil_interflow_inflow;
        // the total interflow, which includes both fast flow and slow flow, unit:
        // meter
        double dSoil_water_interflow_outflow;

        double dSoil_water_interflow_storage;
        // the interflow from upslope
        double dSoil_interflow_inflow_upslope;
        double dSoil_water_interflow_inflow_external;
        // double dSoil_interflow_inflow_groundwater;

        double dSoil_water_outflow_downslope;
        double dSoil_water_outflow_groundwater;
        // soil recharge to groundwater, unit: meter
        double dSoil_moisture_to_groundwater;
        // the et from pervious surface, unit: meter
        double dET_pervious;
        // the ET from impervious surface, unit: meter
        double dET_impervious;
        // linear coefficient for slow interflow, unitless
        double dCoefficient_slow_linear;
        // exponent coefficient for slow interflow, unitless
        double dCoefficient_slow_square;
        // linear coefficient for fast interflow, unitless
        double dCoefficient_fast_linear;
        // exponent coefficient for fast interflow, unitless
        double dCoefficient_fast_square;
        double dRate_gravity_to_groundwater;
        // parameters
        double dSoil_moisture_to_groundwater_max;

        double dCoefficient_slow_linear_base; // linear coefficient for slow
        // interflow, unitless = 0.01
        double dCoefficient_slow_square_base; // exponent coefficient for slow
        // interflow, unitless= 0.1
        double dCoefficient_fast_linear_base; // linear coefficient for fast
        // interflow, unitless= 0.1
        double dCoefficient_fast_square_base; // exponent coefficient for fast
        // interflow, unitless = 0.8
        double dRate_gravity_to_groundwater_base; // = 0.1

        // 50==================================================
        // The second soil water model from iTEM variables
        // 50==================================================
        // The soil thermal module from iTEM
        // 50==================================================
        // soil evaporation (mm h2o /s) [+ = to atm, - = to srf]
        double dET_soil;
        // freezing/thawing temp range for apparent heat capacity
        double dFrozen_range;
        // hydraulic conductivity at saturation (mm h2o /s)
        double dHydraulic_conductivity_saturated;
        // volumetric latent heat of fusion (j/m**3)
        double dLatent_heat_volumetric;
        // infiltration calculated from the surface runoff module, unit: cm
        double dInfiltration;
        // sub-surface runoff (mm h2o /s)
        double dRunoff_subsurface;
        // soil matrix potential at saturation (mm)
        double dSoil_matrix_saturated;
        // unfrozen volumetric specific heat (j/m**3/kelvin)
        double dSpecific_heat_volumetric_unfrozen;
        // frozen volumetric specific heat (j/m**3/kelvin)
        double dSpecific_heat_volumetric_frozen;
        // specific heat capacity soil solids  (j/m**3/kelvin)
        double dSpecific_heat_capacity_solid;
        // thermal conductivity soil solids (w/m/kelvin)
        double dThermal_conductivity_solid;
        // thermal conductivity dry soil (w/m/kelvin)
        double dThermal_conductivity_dry;
        // unfrozen thermal conductivity (w/m/kelvin)
        double dThermal_conductivity_unfrozen;
        // frozen thermal conductivity (w/m/kelvin)
        double dThermal_conductivity_frozen;
        // time step for solving soil    physics
        double dTimestep_soil;
        // soil water content for root zone (up to 1 m depth)
        double dWater_content_root;
        // saturated volumetric soil water content (porosity)
        double dWater_content_saturated;
        // water content when evapotranspiration stops
        double dWater_content_dry;
        // optimal water content for evapotranspiration
        double dWater_content_optimal;
        std::array<double, nsoil_layer> dFraction_root;
        // volumetric specific heat (j/m**3/kelvin)
        std::array<double, nsoil_layer> aSpecific_heat_volumetric;
        // soil layer thickness (m)
        std::array<double, nsoil_layer> aSoil_thickness;
        // soil layer depth (m)
        std::array<double, nsoil_layer> aSoil_depth;
        // thermal conductivity  (w/m/kelvin)
        std::array<double, nsoil_layer> aThermal_conductivity;
        // soil layer temperature (K)
        std::array<double, nsoil_layer> aTemperature_soil;
        // volumetric soil water content
        std::array<double, nsoil_layer> aWater_content_volumetric;
        // 50==================================================
        // member variable for carbon (mainly from TEM) module
        // 50==================================================
        // carbon
        // 50==================================================
        // the total carbon content in the soil, unit: g
        double dSoil_carbon;
        // 50==================================================
        // nitrogen
        // 50==================================================
        // double dNitrogen_available; //
        // double dNitrogen_input;             //
        // double dNitrogen_soil;              //
        // double dNitrogen_output;    //
        // 50==================================================
        // carbon nitrogen interactions
        // 50==================================================
        double dRatio_cn;
        // soil moisture limitation on nitrogen uptake
        double dMoisture_max;
        double dMoisture_min;
        double dMoisture_optimization;
        double dScalar_soil_moisture_decomposition;
        double dScalar_soil_moisture_nitrogen_uptake;

        // solid organic carbon
        double dSoil_soc;

        double dSoil_doc_concentration;
        double dSoil_doc_inflow;
        double dSoil_doc_outflow;
        double dSoil_doc_storage;
        double dSoil_doc_inflow_upslope;
        double dSoil_doc_inflow_external;
        double dSoil_doc_inflow_groundwater;
        double dSoil_doc_outflow_downslope;

        double dSoil_doc_production;     // kg / m2
        double dSoil_doc_mineralization; // kg
        double dSoil_doc_adsorption;
        double dSoil_doc_desorption;

        double dSoil_doc_outflow_groundwater;
        double dSoil_doc_outflow_dunnian_runoff;
        double dSoil_doc_outflow_drainage;
        // 50==================================================
        double dSoil_spsoc; // ratio between mass, units: kg * kg(-1)  < 1.0
        double dSoil_spsoc_mass;
        double dSoil_spsoc_mineralization; // kg
        double dSoil_spsoc_adsorption;
        double dSoil_spsoc_desorption;
        // 50==================================================

        double dSoil_heterotrophic_respiration;
        double dKd;
        double dCoefficient_desorption;
        double dFraction_field_capacity;
        double dFraction_wilting_point;

        //temp
        double dSoil_doc_total;
        double dSoil_water_flow_total;
        // unknown==========================================================
        double bch;     // clapp and hornberger "b"
        double qinfll;  // qinfl for large-scale region (mm h2o /s)
        double qinflcl; // qinfl for convective + large-scale region (mm h2o /s)
        double rwat;    // soil water to maximum depth of 1 m (m)
        double rz;      // thickness of soil layers contributing to rwat (m)
        // 50==================================================
        microbe cMicrobe;

        // step 1
        int calculate_soil_preferential_inflow(double dInfiltration_hru_in,
                                               double dFraction_preferential_in);

        // step 2
        int calculate_preferential_storage();

        // step 3
        int calculate_capillary_inflow(double dInfiltration_hru_in);

        // step 4
        int calculate_soil_recharge_zone();

        // step 5 and 6
        int calculate_soil_moisture();

        // step 7
        int calculate_gravity_reservoir();

        // step 8
        int calculate_preferential_reservoir();

        // step 9
        int calculate_slow_interflow();

        // step 10
        int calculate_gravity_drainage();

        // step 11
        int calculate_fast_interflow();

        // step 12
        int calculate_dunnian_flow();

        // step 13
        int calculate_interflow();

        int calculate_soil_to_groundwater();

        // step 14
        int calculate_soil_evapotranspiration(int iFlag_transpiration,
                                              int iLand_cover_type,
                                              double dET_available_in,
                                              double dSca_in);

        int calculate_soil_dissolved_organic_carbon_adsorption();

        int calculate_soil_dissolved_organic_carbon_desorption();

        int calculate_soil_dissolved_organic_carbon_production(
                double dSoil_temperature_in);

        int calculate_soil_dissolved_organic_carbon_mineralization(
                double dSoil_temperature_in);

        int calculate_soil_dissolved_organic_carbon_concentration();

        int calculate_soil_potential_dissolved_organic_carbon_mineralization(
                double dTemperature_soil_in);

        int calculate_soil_decomposition_moisture_scalar();

        int calculate_soil_heterotrophic_respiration(double dSoil_temperature_in);

        int initialize_soil(eColumn_type eCU_type);

        int run_soil_model(int iFlag_transpiration, int iLand_cover_type,
                           double dDoc_leaching_in, double dEt_available_hru_in,
                           double dFraction_pervious_in, double dInfiltration_hru_in,
                           double dSca_in, double dTemperature_soil_in);

        int run_soil_water_model(int iFlag_transpiration, int iLand_cover_type,
                                 double dET_available_hru_in,
                                 double dFraction_pervious_in,
                                 double dInfiltration_hru_in, double dSCA_in);

        int run_soil_thermal_model();

        int run_soil_carbon_model(double dDoc_leaching_in,
                                  double dTemperature_soil_in);

        int run_soil_dissolved_carbon_model(
                double dSoil_temperature_in);

        int update_soil_parameter(int iFlag_run_carbon_cascade,
                                  int iFlag_run_water_cascade,
                                  int iSoil_type,
                                  int iVegetation_type);

        int update_soil_status(double dLitter_carbon_decomposition_in
        );

        int update_soil_doc_status();

        int update_soil_spsoc_status();

        int update_soil_water_status();

        // soil moisture limitation on nitrogen uptake by microbe and vegetation root
        // int calculate_moisture_scalar_nitrogen_uptake();
        //	int calculate_soil_water_layer(double dEt_surface_in,
        //		double dEt_vegetation_in,
        //		double dTimestep_ecosystem_in);

        //	int calculate_thermal_proprieties();
        //	int calculate_soil_thermal(double dHeat_flux_in,
        //		double dThickness_surface_in);
        // int initialize_thermal_model(int iSoil_type);
    };
} // namespace ecohydrology
