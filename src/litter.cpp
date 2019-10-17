// 50==================================================
/*!
  \file litter.cpp
  \brief
  <long description>
  \author Chang Liao
  \date 2017-01-31
*/
// 50==================================================
#include "litter.h"

namespace ecohydrology
  {
// 50==================================================
/*!
  <long-description>
  \param void
*/
// 50==================================================
      litter::litter()
      {
        iVegetation_type_default = 1;

        dLitter = missing_value;

        dLitter_carbon = missing_value;
        dLitter_carbon_influx = missing_value;
        dLitter_carbon_outflux = missing_value;
        dLitter_carbon_outflux_humus = missing_value;

        dLitter_carbon_undecomposed = missing_value;
        dLitter_carbon_undecomposed_influx = missing_value;
        dLitter_carbon_undecomposed_outflux = missing_value;

        dLitter_carbon_decomposed = missing_value;

        dLitter_carbon_decomposed_influx = missing_value;

        dLitter_carbon_decomposed_outflux = missing_value;

        dLitter_carbon_decomposed_storage_ioc = missing_value;

        dLitter_carbon_decomposed_storage_soc = missing_value;
        dLitter_carbon_decomposed_ioc_2_soc = missing_value;

        dLitter_carbon_decomposed_ioc_2_humus = missing_value;
        dLitter_carbon_decomposed_soc_2_humus = missing_value;
        dLitter_carbon_decomposed_soc_2_doc = missing_value;

        dLitter_heterotrophic_respiration = missing_value;
        dLitter_heterotrophic_respiration_undecomposed = missing_value;
        dLitter_heterotrophic_respiration_decomposed_ioc = missing_value;
        dLitter_heterotrophic_respiration_decomposed_soc = missing_value;

        dCoefficient_carbon_ioc_2_soc = missing_value;
        dCoefficient_carbon_ioc_2_humus = missing_value;
        dLitter_doc_storage = missing_value;
        dLitter_doc_storage_concentration = missing_value;

        dLitter_doc_inflow = missing_value;
        dLitter_doc_outflow = missing_value;
        dLitter_doc_storage_area = missing_value;
        dLitter_doc_outflow_area = missing_value;
        dLitter_doc_inflow_upslope = missing_value;

        dLitter_doc_inflow_external = missing_value;
        dLitter_doc_outflow_concentration = missing_value;
        dLitter_doc_outflow_downslope = missing_value;

        dLitter_doc_outflow_leaching = missing_value;

        dSurface_runoff_inflow = missing_value;
        dSurface_runoff_inflow_upslope = missing_value;
        dSurface_runoff_outflow_downslope = missing_value;

        // 50==================================================
        // unused
        // cascade
        // dLitter_upslope = missing_value;
        // dLitter_downslope = missing_value;
        // dNitrogen_storage = missing_value;
        // dPercentage_nitrogen = missing_value;
        // dNitrogen_decomposed_storage = missing_value;
        // dNitrogen_decomposed_influx = missing_value;
      }

      litter::~litter()
      {
      }

///
/// \param eCU_type
/// \return
      int litter::initialize_litter(eColumn_type eCU_type)
      {
        int error_code = 1;
        iVegetation_type = iVegetation_type_default;
        switch (eCU_type)
        {
          case eCU_glacier: // glacier
          {
            dLitter_doc_inflow_upslope = 0.0;
          }
            break;
          case eCU_lake: // lake
            break;
          case eCU_land: // land
          {

            iFlag_run_carbon_cascade = 1;
            iVegetation_type_default = 1;
            dLitter = 0.0;
            dLitter_carbon = 0.0;
            dLitter_carbon_influx = 0.0;
            dLitter_carbon_outflux = 0.0;
            dLitter_carbon_outflux_humus = 0.0;

            dLitter_carbon_undecomposed = 0.0;
            dLitter_carbon_undecomposed_influx = 0.0;
            dLitter_carbon_undecomposed_outflux = 0.0;
            dLitter_carbon_decomposed = 0.0;

            dLitter_carbon_decomposed_influx = 0.0;

            dLitter_carbon_decomposed_outflux = 0.0;

            dLitter_carbon_decomposed_storage_ioc = 0.0;

            dLitter_carbon_decomposed_storage_soc = 0.0;
            dLitter_carbon_decomposed_ioc_2_soc = 0.0;

            dLitter_carbon_decomposed_ioc_2_humus = 0.0;
            dLitter_carbon_decomposed_soc_2_humus = 0.0;
            dLitter_carbon_decomposed_soc_2_doc = 0.0;

            dLitter_heterotrophic_respiration = 0.0;
            dLitter_heterotrophic_respiration_undecomposed = 0.0;
            dLitter_heterotrophic_respiration_decomposed_ioc = 0.0;
            dLitter_heterotrophic_respiration_decomposed_soc = 0.0;

            // very important parameters
            dCoefficient_carbon_ioc_2_soc = 0.1;
            dCoefficient_carbon_ioc_2_humus = 0.1;
            dCoefficient_carbon_soc_2_humus = 0.1;

            dLitter_doc_storage = 0.0;
            dLitter_doc_storage_concentration = 0.0;

            dLitter_doc_inflow = 0.0;
            dLitter_doc_outflow = 0.0;
            dLitter_doc_storage_area = 0.0;
            dLitter_doc_outflow_area = 0.0;
            dLitter_doc_inflow_upslope = 0.0;

            dLitter_doc_inflow_external = 0.0;
            dLitter_doc_outflow_concentration = 0.0;
            dLitter_doc_outflow_downslope = 0.0;

            dLitter_doc_outflow_leaching = 0.0;

            dSurface_runoff_inflow = 0.0;
            dSurface_runoff_inflow_upslope = 0.0;
            dSurface_runoff_outflow_downslope = 0.0;

            // 50==================================================
            // dNitrogen_storage = 0.0;
            // dPercentage_nitrogen = 0.0;
            // dNitrogen_decomposed_storage = 0.0;
            // dNitrogen_decomposed_influx = 0.0;
          }
            break;
          case eCU_stream: // stream
          {
            dLitter_doc_inflow_upslope = 0.0;
          }
            break;
          case eCU_swale: // swale
          {
          }
            break;
          default:
            break;
        }
        return error_code;
      }

      int litter::calculate_litter_heterotrophic_respiration(
              double dLitter_temperature_in)
      {
        int error_code = 1;

        // undecomposed and solid

        double dLitter_heterotrophic_respiration_rate =
                cMicrobe.calculate_litter_heterotrophic_respiration_rate(
                        dLitter_temperature_in);

        dLitter_heterotrophic_respiration_undecomposed =
                dLitter_carbon_undecomposed * dLitter_heterotrophic_respiration_rate;
        if (dLitter_heterotrophic_respiration_undecomposed < near_zero)
        {
          dLitter_heterotrophic_respiration_undecomposed = 0.0;
        }

        dLitter_heterotrophic_respiration_decomposed_ioc =
                dLitter_carbon_decomposed_storage_ioc *
                        dLitter_heterotrophic_respiration_rate;
        if (dLitter_heterotrophic_respiration_decomposed_ioc < near_zero)
        {
          dLitter_heterotrophic_respiration_decomposed_ioc = 0.0;
        }

        dLitter_heterotrophic_respiration_decomposed_soc =
                dLitter_carbon_decomposed_storage_soc *
                        dLitter_heterotrophic_respiration_rate;
        if (dLitter_heterotrophic_respiration_decomposed_soc < near_zero)
        {
          dLitter_heterotrophic_respiration_decomposed_soc = 0.0;
        }
        dLitter_heterotrophic_respiration =
                dLitter_heterotrophic_respiration_undecomposed +
                        dLitter_heterotrophic_respiration_decomposed_ioc +
                        dLitter_heterotrophic_respiration_decomposed_soc;

        if (dLitter_heterotrophic_respiration < near_zero)
        {
          dLitter_heterotrophic_respiration = 0.0;
        }

        return error_code;
      }

// 50==================================================
/*!
  Calculate the carbon decomposition
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_carbon_decomposition(double dTemperature_in)
      {
        int error_code = 1;

        double dLitter_carbon_decomposition_rate =
                cMicrobe.calculate_litter_carbon_decomposition_rate(dTemperature_in);
        // the total decomposition
        dLitter_carbon_decomposed_influx =
                dLitter_carbon_undecomposed * dLitter_carbon_decomposition_rate;
        // dNitrogen = dNitrogen - dNitrogen_decomposition_flux;
        if (dLitter_carbon_decomposed_influx < near_zero)
        {
          dLitter_carbon_decomposed_influx = 0.0;
        }
        //distribute to ioc and soc
        dLitter_carbon_decomposed_ioc_influx = dLitter_carbon_decomposed_influx * 0.8;
        dLitter_carbon_decomposed_soc_influx = dLitter_carbon_decomposed_influx * 0.2;

        dLitter_carbon_undecomposed_outflux = dLitter_carbon_decomposed_influx;

        return error_code;
      }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_decomposed_carbon_soc_budget()
      {
        int error_code = 1;
        // part of the spsoc comes from direct decomposition
        dLitter_carbon_decomposed_storage_soc =
                dLitter_carbon_decomposed_storage_soc
                        + dLitter_carbon_decomposed_soc_influx
                        + dLitter_carbon_decomposed_ioc_2_soc
                        - dLitter_carbon_decomposed_soc_2_doc
                        - dLitter_carbon_decomposed_soc_2_humus
                        - dLitter_heterotrophic_respiration_decomposed_soc;

        if (dLitter_carbon_decomposed_storage_soc < near_zero)
        {
          dLitter_carbon_decomposed_storage_soc = 0.0;
        }
        return error_code;
      }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_decomposed_carbon_ioc_budget()
      {
        int error_code = 1;
        // the total decomposed solid
        dLitter_carbon_decomposed_storage_ioc =
                dLitter_carbon_decomposed_storage_ioc
                        + dLitter_carbon_decomposed_ioc_influx

                        - dLitter_carbon_decomposed_ioc_2_humus
                        - dLitter_carbon_decomposed_ioc_2_soc
                        - dLitter_heterotrophic_respiration_decomposed_ioc;
        if (dLitter_carbon_decomposed_storage_ioc < tiny_value)
        {
          dLitter_carbon_decomposed_storage_ioc = 0.0;
        }
        return error_code;
      }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_decomposed_ioc_carbon_to_humus()
      {
        int error_code = 1;
        // the total decomposed solid
        dLitter_carbon_decomposed_ioc_2_humus =
                dLitter_carbon_decomposed_storage_ioc *
                        dCoefficient_carbon_ioc_2_humus;
        if (dLitter_carbon_decomposed_ioc_2_humus < near_zero)
        {
          dLitter_carbon_decomposed_ioc_2_humus = 0.0;
        }
        return error_code;
      }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_decomposed_ioc_carbon_to_soc()
      {
        int error_code = 1;
        dLitter_carbon_decomposed_ioc_2_soc =
                dLitter_carbon_decomposed_storage_ioc *
                        dCoefficient_carbon_ioc_2_soc;
        if (dLitter_carbon_decomposed_ioc_2_soc < near_zero)
        {
          dLitter_carbon_decomposed_ioc_2_soc = 0.0;
        }
        return error_code;
      }

      int litter::calculate_litter_decomposed_soc_to_humus()
      {
        int error_code = 1;
        dLitter_carbon_decomposed_soc_2_humus =
                dLitter_carbon_decomposed_storage_soc *
                        dCoefficient_carbon_soc_2_humus;
        if (dLitter_carbon_decomposed_soc_2_humus < near_zero)
        {
          dLitter_carbon_decomposed_soc_2_humus = 0.0;
        }
        return error_code;
      }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_decomposed_soc_to_doc(
              double dSurface_runoff_inflow_in, double dSurface_runoff_storage_in)
      {
        int error_code = 1;

        double dDoc_inflow_area = dLitter_doc_inflow * dArea;
        // the total volume of water
        double dVolume =
                (dSurface_runoff_inflow_in + dSurface_runoff_storage_in) * dArea;
        // the maximum doc can be transferred
        double dDoc_capacity_area = dVolume * dDoc_concentration_maximal;
        double dCarbon_decomposition_spsoc_2_doc_area;
        double dDoc_available_area =
                dDoc_capacity_area - dDoc_inflow_area - dLitter_doc_storage_area;
        double dDoc_available = dDoc_available_area / dArea;
        // current mass in spsoc
        double dDifference_doc =
                dDoc_available - dLitter_carbon_decomposed_storage_soc;
        if (abs(dDifference_doc) < near_zero)
        {
          // all goes out
          dLitter_carbon_decomposed_soc_2_doc = dDoc_available;
        }
        else
        {
          if (dDifference_doc <= -near_zero)
          {
            // not all goes out
            dLitter_carbon_decomposed_soc_2_doc = dDoc_available;
          }
          else
          {
            // all go out
            dLitter_carbon_decomposed_soc_2_doc =
                    dLitter_carbon_decomposed_storage_soc;
          }
        }
        dCarbon_decomposition_spsoc_2_doc_area =
                dLitter_carbon_decomposed_soc_2_doc * dArea;

        dLitter_doc_outflow_area = dDoc_inflow_area + dLitter_doc_storage_area +
                dCarbon_decomposition_spsoc_2_doc_area;

        if (dVolume > small_value)
        {
          dLitter_doc_outflow_concentration = dLitter_doc_outflow_area / dVolume;
          if (dLitter_doc_outflow_concentration > dDoc_concentration_maximal)
          {
            dLitter_doc_outflow_concentration = dDoc_concentration_maximal;
            dLitter_doc_outflow_area = dLitter_doc_outflow_concentration * dVolume;
          }
        }
        else
        {
          dLitter_doc_outflow_area = 0.0;
          dLitter_doc_outflow_concentration = 0.0;
        }

        return error_code;
      }
// 50==================================================
/*!
  Calculate the doc in litter
  \param dDoc_inflow_in, units: gC/m2
  \param dInfiltration_in
  \param dPrecipitation_net_in
  \param dSurface_runoff_upslope_in
  \return <ReturnValue>
*/
// 50==================================================
      int litter::calculate_litter_dissolved_organic_carbon(
              double dSurface_runoff_outflow_in,
              double dSurface_runoff_outflow_infiltration_in)
      {
        int error_code = 1;
        double dPercentage_leaching;
        dLitter_doc_storage_area = dLitter_doc_storage * dArea;

        // partition doc based on water flux
        dLitter_doc_outflow = dLitter_doc_outflow_area / dArea;

        if (iFlag_run_carbon_cascade == 1)
        {
          if (dSurface_runoff_outflow_in < near_zero)
          {
            dPercentage_leaching = 0.0;
          }
          else
          {
            dPercentage_leaching = dSurface_runoff_outflow_infiltration_in /
                    dSurface_runoff_outflow_in;
          }
          dLitter_doc_outflow_leaching = dLitter_doc_outflow * dPercentage_leaching;
          if (dLitter_doc_outflow_leaching < near_zero)
          {
            dLitter_doc_outflow_leaching = 0.0;
          }
          dLitter_doc_outflow_downslope =
                  dLitter_doc_outflow - dLitter_doc_outflow_leaching;
          if (dLitter_doc_outflow_downslope < near_zero)
          {
            dLitter_doc_outflow_downslope = 0.0;
          }


        }
        else
        {
          dLitter_doc_outflow_leaching = dLitter_doc_outflow;
          dLitter_doc_outflow_downslope = 0.0;
        }
        return error_code;
      }

// 50==================================================
/*!
  The litter model
  \param dCarbon_litterfall_in
  \param dNitrogen_litterfall_in
  \return <ReturnValue>
*/
// 50==================================================
      int litter::run_litter_model(double dSurface_runoff_inflow_in,
                                   double dSurface_runoff_outflow_in,
                                   double dSurface_runoff_outflow_infiltration_in,
                                   double dSurface_runoff_storage_in,
                                   double dTemperature_in)
      {
        int error_code = 1;

        dSurface_runoff_inflow = dSurface_runoff_inflow_in;
        dLitter_doc_inflow = dLitter_doc_inflow_external + dLitter_doc_inflow_upslope;

        // respiration

        calculate_litter_heterotrophic_respiration(dTemperature_in);

        // decomposition
        calculate_litter_carbon_decomposition(dTemperature_in);

        // solid to soil humus
        calculate_litter_decomposed_ioc_carbon_to_humus();

        // solid to spsoc
        calculate_litter_decomposed_ioc_carbon_to_soc();

        // spsoc to humus
        calculate_litter_decomposed_soc_to_humus();

        dLitter_carbon_outflux_humus = dLitter_carbon_decomposed_ioc_2_humus +
                dLitter_carbon_decomposed_soc_2_humus;
        // spsoc to doc

        calculate_litter_decomposed_soc_to_doc(dSurface_runoff_inflow,
                                               dSurface_runoff_storage_in);

        calculate_litter_dissolved_organic_carbon(
                dSurface_runoff_outflow_in, dSurface_runoff_outflow_infiltration_in);

        return error_code;
      }

      int litter::update_litter_parameter(int iFlag_run_carbon_cascade,
                                          int iFlag_run_water_cascade,
                                          int iSoil_type, int iVegetation_type)
      {
        int error_code = 1;
        this->iFlag_run_water_cascade = iFlag_run_water_cascade;
        this->iFlag_run_carbon_cascade = iFlag_run_carbon_cascade;
        if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
        {
          // something is wrong?
        }
        else
        {
        }
        cMicrobe.update_microbe_parameter(iSoil_type, iVegetation_type);
        return error_code;
      }

      int litter::update_litter_status(double dCarbon_litterfall_in)
      {
        int error_code = 1;

        dLitter_carbon_influx = dCarbon_litterfall_in;
        dLitter_carbon_undecomposed_influx = dCarbon_litterfall_in;
        // undecomposed
        dLitter_carbon_undecomposed = dLitter_carbon_undecomposed +
                dCarbon_litterfall_in -
                dLitter_carbon_undecomposed_outflux -
                dLitter_heterotrophic_respiration_undecomposed;

        if (dLitter_carbon_undecomposed < near_zero)
        {
          dLitter_carbon_undecomposed = 0.0;
        }

        // decomposed

        calculate_litter_decomposed_carbon_ioc_budget();
        calculate_litter_decomposed_carbon_soc_budget();

        dLitter_carbon_decomposed = dLitter_carbon_decomposed_storage_ioc +
                dLitter_carbon_decomposed_storage_soc;

        if (dLitter_carbon_decomposed < near_zero)
        {
          dLitter_carbon_decomposed = 0.0;
        }

        dLitter_carbon = dLitter_carbon_undecomposed + dLitter_carbon_decomposed;

        if (dLitter_carbon < tiny_value)
        {
          dLitter_carbon = 0.0;
        }
        return error_code;
      }

// 50==================================================
/*!
Calculate the decomposition using rate function
\param
\return <ReturnValue>
*/
// 50==================================================
// int litter::calculate_litter_nitrogen_decomposition()
//{
//  int error_code = 1;
//  // the total decomposition
//  dNitrogen_decomposed_influx = dNitrogen_storage * dDecompose_rate_nitrogen;
//  if (dNitrogen_decomposed_influx < tiny_value)
//    {
//      dNitrogen_decomposed_influx = 0.0;
//    }
//  dNitrogen_decomposed_storage =
//    dNitrogen_decomposed_storage + dNitrogen_decomposed_influx;
//  if (dNitrogen_decomposed_storage < tiny_value)
//    {
//      dNitrogen_decomposed_storage = 0.0;
//    }
//  return error_code;
//}

// int litter::calculate_litter_downslope()
//{
//  int error_code = 1;
//  return error_code;
//}
// 50==================================================
/*!
calculate nitrogen budget
\param dNitrogen_litterfall_in
\param dNitrogen_litter_upslope
\return <ReturnValue>
*/
// 50==================================================
// int litter::calculate_litter_nitrogen_budget(double dNitrogen_litterfall_in)
// {
//   int error_code = 1;
//   dNitrogen_storage = dNitrogen_storage + dNitrogen_litterfall_in;
//   if (dNitrogen_storage < small_value)
//     {
//       dNitrogen_storage = 0.0;
//     }
//   return error_code;
// }

// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
// int litter::calculate_litter_budget(double dCarbon_litterfall_in, double
// dNitrogen_litterfall_in)
// {
//   int error_code = 1;
//   // the total decomposed carbon is partitioned into two parts
//   dLitter = dLitter + dCarbon_litterfall_in
//           + dNitrogen_litterfall_in;
//   if (dLitter < tiny_value)
//   {
//     dLitter = 0.0;
//   }
//   return error_code;
// }

  } // namespace ecohydrology
