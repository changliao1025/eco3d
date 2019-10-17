#include "land.h"

namespace ecohydrology
  {
  land::land ()
    {
    dLand_carbon = missing_value;
    dLand_nep = missing_value;
    dLand_pet = missing_value;
    }

  land::~land ()
    { }
// 50==================================================
/*!
  <long-description>

  \param
  \return <ReturnValue>
*/
// 50==================================================
  int land::initialize_land (eColumn_type eCU_type)
    {
    int error_code = 1;

    this->eCU_type = eCU_type;

    if (eCU_type != eCU_land)
    {
      // something is wrong
      error_code = 0;
    }
    else
    {
      // member variables
      dLand_carbon = 0.0;
      dLand_nep = 0.0;
      dLand_pet = 0.0;
      // components
      cEvapotranspiration.initialize_evapotranspiration ();
      cVegetation.initialize_vegetation (eCU_type);
      cHru.initialize_hru (eCU_type);
      cLandcover.initialize_landcover ();

      cLitter.initialize_litter (eCU_type);
      cSnow.initialize (eCU_type);
      cSurface_runoff.initialize_surface_runoff (eCU_type);
      cSoil.initialize_soil (eCU_type);
      cGroundwater.initialize_groundwater ();
    }
    return error_code;
    }

  int land::run_land_model (
      int iDay_doy, int iFlag_transpiration, int iMonth_ymd,
      int iPrecipitation_type, int iStorm_type, double dFpar_in, double dLai_in,
      double dPar_in, double dPrecipitation_in, double dPrecipitation_basin_in,
      double dRain_in, double dShortwave_radiation_in, double dSnow_in,
      double dTemperature_basin_max_in, double dTemperature_dewpoint_in,
      double dTemperature_max_in, double dTemperature_mean_in,
      double dTemperature_min_in, double dTemperature_rain_in,
      double dTemperature_snow_in, double dVapor_pressure_deficit_in)
    {
    int error_code = 1;
    int iFlag_old_snow;
    int iFlag_has_snow;
    double dRain;
    double dSnow;
    double dEt;
    double dSca;
    double dSnowmelt;
    double dSoil_moisture;
    double dSoil_moisture_max;
    double dStorage_recharge_zone;
    double dStorage_recharge_zone_max;

    double dInfiltration;
    double dSurface_runoff;
    double dSurface_runoff_inflow;
    double dSurface_runoff_outflow;
    double dSurface_runoff_storage;

    double dGpp;
    double dSoil_to_groundwater;
    double dGravity_drainage;
    double dLitter_carbon_deposit;
    double dLitter_doc_leaching;
    double dFraction_pervious;
    double dElevation = cGeology.dSurface_elevation;
    double dEt_dummy = 0.0;

    cEvapotranspiration.run_evapotranspiration_model (
        iMonth_ymd, dElevation,
        dShortwave_radiation_in, // radiation module
        dTemperature_mean_in, dVapor_pressure_deficit_in);
    this->dLand_pet = cEvapotranspiration.dPet;
    // 50==================================================
    // initialize all the necessary modules
    // 50==================================================

    // we need to fix the differences between vegetation type

    // 50==================================================
    // canopy level
    // update the vegetation type here
    int iVegetation_type = cVegetation.iVegetation_type;
    int iLand_cover_type = cLandcover.iLand_cover_type;

    cVegetation.run_vegetation_model (
        iMonth_ymd, iPrecipitation_type, dArea, dLand_pet,
        dLai_in, // climate input
        dRain_in, dSnow_in, dFpar_in, dPar_in, dTemperature_mean_in,
        dVapor_pressure_deficit_in); // estimated from dewpoint temperature
    dEt_dummy = cVegetation.cCanopy.cInterception.dEt_canopy_hru;
    dEt = dLand_pet - dEt_dummy;

    // precipitation type must be updated again due to canopy
    iPrecipitation_type = cVegetation.cCanopy.cInterception.iPrecipitation_type;
    // rain and snow must be updated again due to canopy
    dRain = cVegetation.cCanopy.cInterception.dRain_net;
    dSnow = cVegetation.cCanopy.cInterception.dSnow_net;

    cSnow.run_snow_model (iDay_doy, iFlag_transpiration, iLand_cover_type,
                          iMonth_ymd, iPrecipitation_type, iStorm_type, dElevation,
                          dEt, dLai_in, dPrecipitation_basin_in, dRain, dSnow,
                          dShortwave_radiation_in, dTemperature_max_in,
                          dTemperature_mean_in, dTemperature_min_in,
                          dTemperature_rain_in, dTemperature_snow_in);
    // 50==================================================
    dEt_dummy = cSnow.dSnow_et_hru;
    dEt = dEt - dEt_dummy;

    // 50==================================================
    iFlag_old_snow = cSnow.iFlag_old_snow;
    iFlag_has_snow = cSnow.iFlag_has_snow;
    dSca = cSnow.dSca;
    dSnowmelt = cSnow.dSnowmelt;
    dSoil_moisture = cSoil.dSoil_water_storage_moisture;
    dSoil_moisture_max = cSoil.dSoil_storage_moisture_max;
    dStorage_recharge_zone = cSoil.dSoil_water_storage_recharge_zone;
    dStorage_recharge_zone_max = cSoil.dSoil_storage_recharge_zone_max;

    cSurface_runoff.run_surface_runoff_model (
        iFlag_old_snow, iFlag_has_snow, eCU_type, iLand_cover_type,
        iPrecipitation_type, dEt, dRain_in, dSca, dSnowmelt, dSnow_in,
        dSoil_moisture,     // system variable
        dSoil_moisture_max, // soil proprities
        dTemperature_mean_in, dStorage_recharge_zone, dStorage_recharge_zone_max);

    dInfiltration = cSurface_runoff.cInfiltration.dInfiltration_hru;

    // litter model
    dSurface_runoff_inflow = cSurface_runoff.dSurface_runoff_inflow;
    dSurface_runoff_outflow = cSurface_runoff.dSurface_runoff_outflow;
    dSurface_runoff_storage = cSurface_runoff.dSurface_runoff_storage;
    cLitter.run_litter_model (dSurface_runoff_inflow, dSurface_runoff_outflow,
                              dInfiltration, dSurface_runoff_storage,
                              dTemperature_mean_in);
    dEt_dummy = cSurface_runoff.dSurface_et_hru;
    dEt = dEt - dEt_dummy;

    dLitter_doc_leaching = cLitter.dLitter_doc_outflow_leaching;

    dFraction_pervious = cSurface_runoff.dPervious_surface_percentage;


    cSoil.run_soil_model (iFlag_transpiration, iLand_cover_type,
                          dLitter_doc_leaching, dEt, dFraction_pervious,
                          dInfiltration, dSca, dTemperature_mean_in);
    
    // add dunnian runoff back before cascade
    cSurface_runoff.dSurface_runoff_outflow_downslope += +cSoil.dSoil_water_outflow_dunnian_runoff;
    // 50==================================================
    // step 9 groundwater
    // 50==================================================
    dSoil_to_groundwater = cSoil.dSoil_moisture_to_groundwater;
    dGravity_drainage = cSoil.dSoil_gravity_drainage;
    cGroundwater.run_groundwater_model (dSoil_to_groundwater, dGravity_drainage);
    return error_code;
    }
// 50==================================================
/*!
  <long-description>

  \param iFlag_run_carbon_cascade
  \param iLand_cover_type
  \param iSoil_type
  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
  int land::update_land_parameter (int iFlag_run_carbon_cascade,
                                   int iFlag_run_water_cascade,
                                   int iLand_cover_type, int iSoil_type,
                                   int iVegetation_type,
                                   double dImpervious_surface_percentage_in,
                                   double dPervious_surface_percentage_in,
                                   double dSurface_depression_percentage_in)
    {
    int error_code = 1;
    this->iFlag_run_carbon_cascade = iFlag_run_carbon_cascade;
    cHru.update_hru_parameter (dImpervious_surface_percentage_in,
                               dPervious_surface_percentage_in,
                               dSurface_depression_percentage_in);

    cEvapotranspiration.update_evapotranspiration_parameter ();
    cLandcover.update_landcover_parameter (iLand_cover_type);
    cVegetation.update_vegetation_parameter (iVegetation_type);
    cLitter.update_litter_parameter (iFlag_run_carbon_cascade,iFlag_run_water_cascade,
                                     iSoil_type,
                                     iVegetation_type);
    cSnow.update_snow_parameter ();
    cSurface_runoff.update_surface_runoff_parameter (iLand_cover_type, iSoil_type);
    cSoil.update_soil_parameter (iFlag_run_carbon_cascade,iFlag_run_water_cascade,
                                 iSoil_type,
                                 iVegetation_type);
    cGroundwater.update_groundwater_parameter ();
    return error_code;
    }

///
/// \return
  int land::update_land_status ()
    {
    int error_code = 1;
    double dLitter_carbon_deposition = 0.0;
    //double dLitter_doc_leaching = 0.0;
    double dLitterfall_carbon;
    double dNpp;
    double dHeterotrophic_respiration;
    double dDoc_upslope;
    double dDoc_downslope;

    cVegetation.update_vegetation_status ();

    dLitter_carbon_deposition = cLitter.dLitter_carbon_decomposed_ioc_2_humus +
                                cLitter.dLitter_carbon_decomposed_soc_2_humus +
                                cVegetation.cRoot.dRoot_carbon_decomposed;
    //dLitter_doc_leaching = cLitter.dLitter_doc_outflow_leaching;
    dNpp = cVegetation.dVegetation_npp;

    dLitterfall_carbon = cVegetation.cCanopy.cLitterfall.dLitterfall_carbon;
    // update litter
    cLitter.update_litter_status (dLitterfall_carbon);
    // 50==================================================
    // update soil system variable
    // 50==================================================
    dHeterotrophic_respiration = cSoil.dSoil_heterotrophic_respiration
                                 + cLitter.dLitter_heterotrophic_respiration;
    cSoil.update_soil_status (dLitter_carbon_deposition);

    // 50==================================================
    // column update
    // 50==================================================
    if (iFlag_run_carbon_cascade == 1)
    {
      dDoc_upslope =
          cLitter.dLitter_doc_inflow_upslope + cSoil.dSoil_doc_inflow_upslope;
      dDoc_downslope = cLitter.dLitter_doc_outflow_downslope +
                       cSoil.dSoil_doc_outflow_downslope;
    }
    else
    {
      dDoc_upslope = 0.0;
      dDoc_downslope = 0.0;
    }
    // 50==================================================
    // update carbon
    // 50==================================================
    dLand_carbon = dLand_carbon + dNpp - dHeterotrophic_respiration +
                   dDoc_upslope - dDoc_downslope;
    if (dLand_carbon < near_zero)
    {
      dLand_carbon = 0;
    }
    // 50==================================================
    // update nep
    // 50==================================================
    dLand_nep = dNpp - dHeterotrophic_respiration + dDoc_upslope - dDoc_downslope;

    if (abs (dLand_nep) < near_zero)
    {
      iFlag_source_or_sink = -1; // it is steady state
    }
    else
    {
      if (dLand_nep <= -near_zero)
      {
        iFlag_source_or_sink = 0;
      }
      else
      {
        iFlag_source_or_sink = 1;
      }
    }
    return error_code;
    }

  } // namespace ecohydrology
