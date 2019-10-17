// 50==================================================
/*!
  \file   ecosystem_initialize.cpp
  \brief
  <long description>
  \author Chang Liao
  \date   2017-07-16
*/
// 50==================================================
#include "eco3d.h"

namespace ecohydrology
  {
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int eco3d::initialize_eco3d()
      {
        int error_code = 1;
        initialize_spatial_domain();
        initialize_time_step();
        assign_time_invariant_data();
        initialize_ecosystem();
        initialize_ecosystem_from_initial_file();
        if (iFlag_run_cascade_model != 1)
        {
          setup_cascade_parameter();
        }
        create_stream_segment_reach_topology();

        // should we save something here?
        save_system_parameter();
        return error_code;
      }

      int eco3d::initialize_time_step()
      {
        int error_code = 1;

        long number = day_2_minute;
        long temp = 1;
        vTime_step.clear();
        while (temp <= number)
        {
          if ((number % temp) == 0)
          {
            vTime_step.push_back(temp);
          }
          temp++;
        }
        return error_code;
      }

      int eco3d::find_nearest_time_step(int iTime_step_in)
      {
        int iTime_step_out;
        int iFlag_found = 0;
        long lTime_step1;
        long lTime_step2;
        std::vector<long>::iterator iIterator;

        for (iIterator = vTime_step.begin(); iIterator != vTime_step.end() - 1;
             iIterator++)
        {
          lTime_step1 = *iIterator;
          lTime_step2 = *(iIterator + 1);
          if (iTime_step_in >= lTime_step1 && iTime_step_in < lTime_step2)
          {
            iTime_step_out = lTime_step1;
            iFlag_found = 1;
            break;
          }
          else
          {
            iFlag_found = 0;
            continue;
          }
        }
        if (iFlag_found == 0)
        {

          iTime_step_out = vTime_step.at(vTime_step.size() - 1);
        }

        return iTime_step_out;
      }
// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int eco3d::initialize_spatial_domain()
      {
        int error_code = 1;
        long lIndex, lColumn_count;
        // 50==================================================
        // the vColumn vector has stored the indices of all active pixels with lIndex
        // starting from 0
        lColumn_count =
                (long) (*std::max_element(vColumn_index.begin(), vColumn_index.end()) + 1);
        for (lIndex = 0; lIndex < lColumn_count; lIndex++)
        {
          // initialize one temporal object
          vColumn.emplace_back(column()); // add the object into vector
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
      int eco3d::assign_time_invariant_data()
      {
        int error_code = 1;
        long lRow, lColumn;
        long lIndex;
        // int iColumn_type;
        int iBoundary, iHru_type;
        double dArea, dAspect, dElevation, dLatitude, dSlope;
        // 50==================================================
        // clean everything before we start
        // setup_eco3d column, allocate memory
        // in order to save memory, we use vector instead of array to control all the
        // columns.
        // because only active columns will be simulated, we only add then into the
        // vector
        // assign all the global data to each column

        dArea = dResolution * dResolution;
        for (lRow = 0; lRow < nrow; lRow++)
        {
          for (lColumn = 0; lColumn < ncolumn; lColumn++)
          {
            lIndex = calculate_index_from_coordinates(lRow, lColumn);
            // each column represent one pixel on earth;
            if (lIndex >= 0) // lIndex starts from 0 for active column
            {

              dAspect = vAspect[lRow * ncolumn + lColumn];
              // convert from double to integer here
              iBoundary = (int) vBoundary[lRow * ncolumn + lColumn];
              dElevation = vElevation[lRow * ncolumn + lColumn];
              dLatitude = vLatitude[lRow * ncolumn + lColumn];
              dSlope = vSlope[lRow * ncolumn + lColumn];
              // the column type and hru type are the same currently
              iHru_type = (int) vHru_type[lRow * ncolumn + lColumn];

              vColumn[lIndex].cGeology.dResolution = dResolution;
              vColumn[lIndex].cGeology.dArea = dArea;
              vColumn[lIndex].cGeology.dAspect = dAspect;
              vColumn[lIndex].cGeology.iBoundary = iBoundary;
              vColumn[lIndex].cGeology.dSurface_elevation = dElevation;
              vColumn[lIndex].cGeology.dLatitude = dLatitude;
              vColumn[lIndex].cGeology.dSlope = dSlope;

              switch (iHru_type)
              {
                case 1: // glacier
                {
                  vColumn[lIndex].eCU_type = eCU_glacier;
                  vColumn[lIndex].cGlacier.cGeology =
                          vColumn[lIndex].cGeology;
                  vColumn[lIndex].cGlacier.cHru.iHru_type = iHru_type;
                }
                  break;
                case 2: // lake
                {
                  vColumn[lIndex].eCU_type = eCU_lake;
                  vColumn[lIndex].cLake.cGeology = vColumn[lIndex].cGeology;
                  vColumn[lIndex].cLake.cHru.iHru_type = iHru_type;
                }
                  break;
                case 3: // land
                {
                  vColumn[lIndex].eCU_type = eCU_land;
                  vColumn[lIndex].cLand.cGeology = vColumn[lIndex].cGeology;
                  vColumn[lIndex].cLand.cHru.iHru_type = iHru_type;
                }
                  break;
                case 4: // stream
                {
                  vColumn[lIndex].eCU_type = eCU_stream;
                  vColumn[lIndex].cReach.cGeology = vColumn[lIndex].cGeology;
                  vColumn[lIndex].cReach.cHru.iHru_type = iHru_type;
                }
                  break;
                case 5: // swale
                {
                  vColumn[lIndex].eCU_type = eCU_swale;
                  // not implemented yet
                }
                  break;
                default: // land
                {
                  vColumn[lIndex].eCU_type = eCU_land;
                  vColumn[lIndex].cLand.cGeology = vColumn[lIndex].cGeology;
                  vColumn[lIndex].cLand.cHru.iHru_type = iHru_type;
                }
                  break;
              }
            }
            else
            {
              // inactive column
            }
          }
        }

        sLog = "===Level 3: Successfully assigned global data and default value ";
        ofs_log << sLog << std::endl;
        ofs_log.flush();
        std::cout << sLog << std::endl;

        return error_code;
      }

      int eco3d::initialize_ecosystem()
      {
        int error_code = 1;
        // the indices to be used during initialization
        long lRow, lColumn, lIndex;
        // we only initialize the time-invariant parameters for time variant data such
        // as temperature, they are directly passed in at assigned to the class
        // members.
        for (lRow = 0; lRow < nrow; lRow++)
        {
          for (lColumn = 0; lColumn < ncolumn; lColumn++)
          {
            // convert from 2D lIndex to 1D lIndex
            lIndex = calculate_index_from_coordinates(lRow, lColumn);

            if (lIndex >= 0)
            {
              vColumn[lIndex].initialize_column();
            }
          }
        }
        return error_code;
      }

// 50==================================================
/*!
  this function is used to prepare some initial ecosystem variables.
  these variables may or may not change throughout the simulation.
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int eco3d::initialize_ecosystem_from_initial_file()
      {
        int error_code = 1;
        int iTime_step;
        int iTime_step_original;
        eColumn_type eCU_type;

        int iLand_cover_type; // land cover from modis, the data is converted from

        int iVegetation_type; // the vegetation is retrieved from land cover type

        long lRow, lColumn, lIndex; // the indices to be used during initialization
        int iStream_order;
        double dWidth;
        double dWetted_perimeter;
        double dSlope;
        double dCross_section_area;
        double dHydraulic_radius;
        double dManning_roughness = 0.02;
        double dVelocity;
        double dTravel_time = 60;
        double dTravel_time_minute = 1;
        std::size_t iLength;
        std::string sYear;
        std::string sFilename_land_cover_type;

        if (iFlag_run_steady_state == 1)
        {
          sYear = convert_integer_to_string(iYear_start_ss, 4);
        }
        else
        {
          //
          sYear = convert_integer_to_string(iYear_start_tr, 4);
        }
        // only one land cover type used per year
        sFilename_land_cover_type = sWorkspace_land_cover_type + slash + sYear +
                slash + sPrefix_land_cover_type + sYear + "001" +
                sExtension_envi;
        // check the existence of this file
        // if it does not exist, find the nearest available file
        if (file_test(sFilename_land_cover_type) != 1)
        {
          sFilename_land_cover_type = miscellaneous::find_alternative_file(
                  1, iYear_start_tr, sExtension_envi, sPrefix_land_cover_type,
                  sWorkspace_land_cover_type);
          // record this
          iLength = sFilename_land_cover_type.length();
          if (iLength < 4)
          {

            sLog = "====Level 4: No land cover data is found for year:  " + sYear;
            ofs_log << sLog << std::endl;
            ofs_log.flush();
            std::cout << sLog << std::endl;

            error_code = 0;
            return error_code;
          }
          else
          {

            sLog = "====Level 4: Land cover data is missing for year:  " + sYear;
            ofs_log << sLog << std::endl;
            ofs_log.flush();
            std::cout << sLog << std::endl;
          }
        }
        // read_eco3d the land cover data from the above filename
        vLand_cover_type_init = data::read_binary_vector(sFilename_land_cover_type);
        // initial state
        if (file_test(sFilename_soil_carbon_initial) == 1)
        {
          iFlag_soil_carbon_initial = 1;
          vSoil_carbon_init =
                  data::read_binary_vector(sFilename_soil_carbon_initial);
        }
        if (file_test(sFilename_vegetation_carbon_initial) == 1)
        {
          iFlag_vegetation_carbon_initial = 1;
          vVegetation_carbon_init =
                  data::read_binary_vector(sFilename_vegetation_carbon_initial);
        }
        if (file_test(sFilename_litter_carbon_initial) == 1)
        {
          iFlag_litter_carbon_initial = 1;
          vLitter_carbon_init =
                  data::read_binary_vector(sFilename_litter_carbon_initial);
        }
        // we only initialize the time-invariant parameters, for time variant data
        // such as temperature, they are directly passed in at assigned to the class
        // members.
        for (lRow = 0; lRow < nrow; lRow++)
        {
          for (lColumn = 0; lColumn < ncolumn; lColumn++)
          {
            dSlope = vSlope[lRow * ncolumn + lColumn];
            if (dSlope < small_value)
            {
              dSlope = small_value;
            }
            lIndex = calculate_index_from_coordinates(
                    lRow, lColumn); // convert from 2D lIndex to 1D lIndex

            if (lIndex >= 0)
            {
              eCU_type = vColumn[lIndex].eCU_type;
              iLand_cover_type =
                      (int) vLand_cover_type_init[lRow * ncolumn + lColumn];
              // convert the land cover type to vegetation type
              iVegetation_type =
                      vegetation::convert_land_cover_to_vegetation_type(
                              iLand_cover_type);
              switch (eCU_type)
              {
                case eCU_glacier: // glacier
                {
                }
                  break;
                case eCU_lake: // lake
                {
                }
                  break;
                case eCU_land: // land
                {

                  if (iVegetation_type >= 1)
                  {
                    if (iFlag_vegetation_carbon_initial == 1)
                    {
                      vColumn[lIndex]
                              .cLand.cVegetation.dVegetation_carbon =
                              vVegetation_carbon_init[lRow * ncolumn +
                                      lColumn];
                      vColumn[lIndex]
                              .cLand.cVegetation.cCanopy.dCanopy_carbon =
                              vColumn[lIndex]
                                      .cLand.cVegetation.dVegetation_carbon *
                                      0.3;
                      vColumn[lIndex]
                              .cLand.cVegetation.cStem.dStem_carbon =
                              vColumn[lIndex]
                                      .cLand.cVegetation.dVegetation_carbon *
                                      0.1;
                      vColumn[lIndex]
                              .cLand.cVegetation.cRoot.dRoot_carbon =
                              vColumn[lIndex]
                                      .cLand.cVegetation.dVegetation_carbon *
                                      0.6;
                    }
                    if (iFlag_litter_carbon_initial == 1)
                    {
                      vColumn[lIndex].cLand.cLitter.dLitter_carbon =
                              vLitter_carbon_init[lRow * ncolumn + lColumn];
                    }
                  }
                  if (iFlag_soil_carbon_initial == 1)
                  {
                    vColumn[lIndex].cLand.cSoil.dSoil_carbon =
                            vSoil_carbon_init[lRow * ncolumn + lColumn];
                  }
                  // 50==================================================
                }
                  break;
                case eCU_stream: // stream
                {
                  iStream_order =
                          int(vStream_order[lRow * ncolumn + lColumn]);

                  // the stream reach propriety depends on stream order in
                  // current setting
                  switch (iStream_order)
                  {
                    case 1:
                      dWidth = 50.0;
                      dCross_section_area = dWidth * 1.0;
                      dWetted_perimeter = dWidth * 1.3;
                      break;
                    case 2:
                      dWidth = 100.0;
                      dCross_section_area = dWidth * 2.0;
                      dWetted_perimeter = dWidth * 1.1;
                      break;
                    case 3:
                      dWidth = 300.0;
                      dCross_section_area = dWidth * 5.0;
                      dWetted_perimeter = dWidth;
                      break;
                    default:
                      dWidth = 100.0;
                      dCross_section_area = dWidth * 2.0;
                      dWetted_perimeter = dWidth * 1.1;
                      break;
                  }
                  dHydraulic_radius = dCross_section_area / dWetted_perimeter;
                  dVelocity = 1.0 * pow(dHydraulic_radius, 2.0 / 3.0) *
                          sqrt(dSlope / 100.0) / dManning_roughness;
                  dTravel_time = dResolution / dVelocity;

                  dTravel_time_minute = round(dTravel_time / 60);
                  if (dTravel_time_minute < 1)
                  {
                    dTravel_time_minute = 1;
                  }

                  // re-organize the time to be a factor of 1440
                  if (dTravel_time_minute > 10.0)
                  {
                    dTravel_time_minute = 10.0;
                  }

                  iTime_step_original =
                          int(dTimestep_eco3d_minute / dTravel_time_minute);

                  iTime_step =
                          int(find_nearest_time_step(iTime_step_original));
                  // cout << iTime_step_original << ", " << iTime_step <<
                  // endl;
                  vColumn[lIndex].cReach.iTime_step = iTime_step;

                  vColumn[lIndex].cReach.dReach_travel_time_minute =
                          int(dTimestep_eco3d_minute / iTime_step);
                  vColumn[lIndex].cReach.dReach_travel_time =
                          vColumn[lIndex].cReach.dReach_travel_time_minute * 60.0;
                }
                  break;
                case eCU_swale: // swale
                {
                }
                  break;
                default:
                {
                }
                  break;
              }
            }
          }
        }
        return error_code;
      }
  } // namespace ecohydrology
