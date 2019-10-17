// 50==================================================
/*!
  \file ecosystem.cpp
  \brief
  This the major entrance of the ecosystem model
  \author Chang Liao
  \date 2016-09-28
*/
// 50==================================================
#include "eco3d.h"

namespace ecohydrology
{
// 50==================================================
/*!
  The default constructor
  \param void
*/
// 50==================================================
    eco3d::eco3d() = default;
// 50==================================================
/*!
  \param void
*/
// 50==================================================
    eco3d::~eco3d() = default;

// 50==================================================
/*!
  Initialize the ecosystem class with the configuration file string
  \param sFilename_configuration
*/
// 50==================================================
    eco3d::eco3d(std::string sFilename_configuration_in)
    {
      iFlag_run_cascade = 1;
      iFlag_run_carbon_cascade = 1;
      iFlag_debug = 0;
      iFlag_debug_cascade = 0;
      iFlag_debug_land = 0;
      iFlag_debug_reach = 0;
      lCU_index_debug = -1;
      // check the length of the configuration file
      std::size_t iLength = sFilename_configuration_in.length();
      if (iLength < 5)
      {
        this->iFlag_configuration_file = 0;
      }
      else
      {
        std::cout << "====Level 3: The configuration file is:" << sFilename_configuration_in
                  << std::endl;
        // check the existence of the configuration file
        if (file_test(sFilename_configuration_in) !=
                1) // the file does not even exist
        {
          this->iFlag_configuration_file = 0;
        }
        else
        {
          this->sFilename_configuration = sFilename_configuration_in;
          this->iFlag_configuration_file = 1;
        }
      }

      sExtension_header = ".hdr";
      sExtension_envi = ".dat";
      sExtension_text = ".txt";
      sPrefix_dewpoint = "dewpoint";
      sPrefix_fpar = "fpar";
      sPrefix_lai = "lai";
      sPrefix_land_cover_type = "landcover";
      sPrefix_prec = "prec";
      sPrefix_tmax = "tmax";
      sPrefix_tmin = "tmin";
      sModule_cascade = "cascade";
      sModule_snow = "snow";
    }

// 50==================================================
/*!
//check whether the system is steady state or not
//there are many criteria can be used
//the criteria used here is the total carbon budget of the entire wateshed
\param
\return <ReturnValue>
*/
// 50==================================================
    int eco3d::check_steady_state()
    {
      int error_code = 1;
      double dThreshold_carbon = small_value;
      // calculate the absolute changes in total carbon storage
      double dDifference1, dDifference2, dDifference3, dDifference4;

      if (dWatershed_carbon_previous > small_value &&
              dWatershed_carbon_vegetation_previous > small_value &&
              dWatershed_carbon_soil_previous > small_value &&
              dWatershed_carbon_litter_previous > small_value)
      {
        dDifference1 = abs(dWatershed_carbon - dWatershed_carbon_previous) /
                dWatershed_carbon_previous;
        dDifference2 = abs(dWatershed_vegetation_carbon -
                                   dWatershed_carbon_vegetation_previous) /
                dWatershed_carbon_vegetation_previous;
        dDifference3 =
                abs(dWatershed_soil_carbon - dWatershed_carbon_soil_previous) /
                        dWatershed_carbon_soil_previous;
        dDifference4 =
                abs(dWatershed_litter_carbon - dWatershed_carbon_litter_previous) /
                        dWatershed_carbon_litter_previous;

        if (dDifference1 <= dThreshold_carbon &&
                dDifference2 <= dThreshold_carbon &&
                dDifference3 <= dThreshold_carbon &&
                dDifference4 <= dThreshold_carbon)
        {
          // update the system steady state.
          iFlag_steady_state = 1;
        }
        else
        {
          iFlag_steady_state = 0;
        }
      }
      else
      {
        iFlag_steady_state = 0;
      }
      return error_code;
    }

// 50==================================================
/*!
//clean up unused variables and close starlog file
\param
\return <ReturnValue>
*/
// 50==================================================
    int eco3d::cleanup_eco3d()
    {
      int error_code = 1;
      // 50==================================================
      if (this->ofs_log.good())
      {
        ofs_log.close();
        ofs_budget.close();
        if (1 == iFlag_debug_cascade)
        {
          ofs_debug.close();
        }
      }
      return error_code;
    }

// 50==================================================
/*!
//summarize the whole budget for the whole watershed
//currently only the carbon is considered.
\param
\return <ReturnValue>
*/
// 50==================================================
    int eco3d::summarize(int iYear, int iDay)
    {
      int error_code = 1;
      int iBoundary;
      int iHru_type;
      long lRow, lColumn;
      long lIndex;

      double dDummy;
      eColumn_type eCU_type;
      std::string sYear = convert_integer_to_string(iYear, 4);
      std::string sDay = convert_integer_to_string(iDay, 3);
      // before sum up, save current as previous
      dWatershed_carbon_previous = dWatershed_carbon;
      dWatershed_carbon_vegetation_previous = dWatershed_vegetation_carbon;
      dWatershed_carbon_soil_previous = dWatershed_soil_carbon;
      dWatershed_carbon_litter_previous = dWatershed_litter_carbon;
      // 50==================================================
      // reset everything to zero
      dWatershed_vegetation_carbon = 0.0;
      dWatershed_soil_carbon = 0.0;
      dWatershed_litter_carbon = 0.0;
      double dWatershed_litter_carbon_undecomposed = 0.0;
      double dWatershed_litter_carbon_decomposed = 0.0;
      double dWatershed_litter_carbon_decomposed_storage_ioc = 0.0;
      double dWatershed_litter_carbon_decomposed_storage_soc = 0.0;
      dWatershed_carbon = 0.0;

      dWatershed_heterotrophic_respiration = 0.0;
      dWatershed_soil_doc = 0.0;
      dWatershed_soil_doc_production = 0.0;
      dWatershed_soil_doc_mineralization = 0.0;
      dWatershed_litter_doc_leaching = 0.0;
      dWatershed_reach_doc_lateral = 0.0;

      dWatershed_reach_doc_lateral_litter = 0.0;
      dWatershed_reach_doc_lateral_soil = 0.0;
      // 50==================================================
      for (lRow = 0; lRow < nrow; lRow++)
      {

        for (lColumn = 0; lColumn < ncolumn; lColumn++)
        {
          // boundary control
          iBoundary = (int) vBoundary[lRow * ncolumn + lColumn];
          lIndex = calculate_index_from_coordinates(
                  lRow, lColumn); // convert from 2D index to 1D index

          if (iBoundary != missing_value && lIndex >= 0)
          {
            eCU_type = vColumn[lIndex].eCU_type;

            switch (eCU_type)
            {
              case eCU_glacier:
                break;
              case eCU_lake:
                break;
              case eCU_land:
              {
                // vegetation
                dDummy = vColumn[lIndex].cLand.cVegetation.dVegetation_carbon;
                if (dDummy > 0)
                {
                  dWatershed_vegetation_carbon =
                          dWatershed_vegetation_carbon + dDummy;
                }
                // litter
                dDummy = vColumn[lIndex].cLand.cLitter.dLitter_carbon;
                if (dDummy > 0)
                {
                  dWatershed_litter_carbon =
                          dWatershed_litter_carbon + dDummy;
                }

                dDummy =
                        vColumn[lIndex].cLand.cLitter.dLitter_carbon_undecomposed;
                if (dDummy > 0)
                {
                  dWatershed_litter_carbon_undecomposed =
                          dWatershed_litter_carbon_undecomposed + dDummy;
                }
                dDummy =
                        vColumn[lIndex].cLand.cLitter.dLitter_carbon_decomposed;
                if (dDummy > 0)
                {
                  dWatershed_litter_carbon_decomposed =
                          dWatershed_litter_carbon_decomposed + dDummy;
                }
                dDummy = vColumn[lIndex]
                        .cLand.cLitter
                        .dLitter_carbon_decomposed_storage_ioc;
                if (dDummy > 0)
                {
                  dWatershed_litter_carbon_decomposed_storage_ioc =
                          dWatershed_litter_carbon_decomposed_storage_ioc +
                                  dDummy;
                }
                dDummy = vColumn[lIndex]
                        .cLand.cLitter
                        .dLitter_carbon_decomposed_storage_soc;
                if (dDummy > 0)
                {
                  dWatershed_litter_carbon_decomposed_storage_soc =
                          dWatershed_litter_carbon_decomposed_storage_soc +
                                  dDummy;
                }
                // soil

                dDummy = vColumn[lIndex].cLand.cSoil.dSoil_carbon;
                if (dDummy > 0)
                {
                  dWatershed_soil_carbon = dWatershed_soil_carbon + dDummy;
                }
                dDummy = vColumn[lIndex]
                        .cLand.cSoil.dSoil_heterotrophic_respiration;
                if (dDummy > 0)
                {
                  dWatershed_heterotrophic_respiration =
                          dWatershed_heterotrophic_respiration + dDummy;
                }
                // 50==================================================
                dDummy = vColumn[lIndex].cLand.cSoil.dSoil_doc_storage;
                if (dDummy > 0)
                {
                  dWatershed_soil_doc = dWatershed_soil_doc + dDummy;
                }

                dDummy = vColumn[lIndex].cLand.cSoil.dSoil_doc_production;
                if (dDummy > 0)
                {
                  dWatershed_soil_doc_production =
                          dWatershed_soil_doc_production + dDummy;
                }
                dDummy = vColumn[lIndex].cLand.cSoil.dSoil_doc_mineralization;
                if (dDummy > 0)
                {
                  dWatershed_soil_doc_mineralization =
                          dWatershed_soil_doc_mineralization + dDummy;
                }
                dDummy = vColumn[lIndex]
                        .cLand.cLitter.dLitter_doc_outflow_leaching;
                if (dDummy > 0)
                {
                  dWatershed_litter_doc_leaching =
                          dWatershed_litter_doc_leaching + dDummy;
                }
              }
                break;
              case eCU_stream:
              {
                dDummy = vColumn[lIndex].cReach.dReach_doc_inflow_lateral_daily;
                if (dDummy > 0)
                {
                  dWatershed_reach_doc_lateral =
                          dWatershed_reach_doc_lateral + dDummy;
                }
                dDummy = vColumn[lIndex].cReach.dReach_doc_inflow_lateral_litter_daily;
                if (dDummy > 0)
                {
                  dWatershed_reach_doc_lateral_litter =
                          dWatershed_reach_doc_lateral_litter + dDummy;
                }
                dDummy = vColumn[lIndex].cReach.dReach_doc_inflow_lateral_soil_daily;
                if (dDummy > 0)
                {
                  dWatershed_reach_doc_lateral_soil =
                          dWatershed_reach_doc_lateral_soil + dDummy;
                }
              }
                break;
              case eCU_swale:
                break;
              default:
                break;
            }


          }     // avtive

        } // column
      }     // row

      // 50==================================================
      // add them together
      dWatershed_carbon = dWatershed_vegetation_carbon + dWatershed_litter_carbon +
              dWatershed_soil_carbon;

      sLog = "======Level 6: Summary for year: " + sYear + ", " + sDay + " :"

              + convert_double_to_string(2, 10, dWatershed_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_vegetation_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_soil_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_carbon_undecomposed) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_carbon_decomposed) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_carbon_decomposed_storage_ioc) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_carbon_decomposed_storage_soc);

      ofs_log << sLog << std::endl;
      ofs_log.flush();
      //std::cout << sLog << std::endl;

      sLog = sYear + ", " + sDay + ", "

              + convert_double_to_string(2, 10, dWatershed_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_vegetation_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_soil_carbon) + ", "
              + convert_double_to_string(2, 10, dWatershed_heterotrophic_respiration) + ", "
              + convert_double_to_string(2, 10, dWatershed_soil_doc_production) + ", "
              + convert_double_to_string(2, 10, dWatershed_soil_doc_mineralization) + ", "
              + convert_double_to_string(2, 10, dWatershed_litter_doc_leaching) + ", "
              + convert_double_to_string(2, 10, dWatershed_reach_doc_lateral) + ", "
              + convert_double_to_string(2, 10, dWatershed_reach_doc_lateral_litter) + ", "
              + convert_double_to_string(2, 10, dWatershed_reach_doc_lateral_soil);

      this->ofs_budget << sLog << std::endl;
      ofs_budget.flush();
      return error_code;
    }
} // namespace ecohydrology
