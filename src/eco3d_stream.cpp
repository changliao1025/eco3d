// 50==================================================
/*!
  \file ecosystem_stream.cpp
  \brief
  <long description>
  \author Chang Liao
  \date 2017-07-05
*/
// 50==================================================
#include "eco3d.h"

namespace ecohydrology
  {

      int eco3d::check_stream_segment_reach_daily_data(
              std::vector<std::vector<std::vector<double>>> vDaily_data_in)
      {
        int error_code = 1;
        int iSegment, nSegment_dummy;
        long iReach;
        long nReach_segment, nReach_dummy;

        std::vector<long> vReach_dummy;
        std::vector<std::vector<double>> vReach_dummy2; // stores all the reach
        // indices
        string sSegment;

        nSegment_dummy = vDaily_data_in.size();

        if (nSegment_dummy != nsegment)
        {
          error_code = 0;
        }
        else
        {

          for (iSegment = 1; iSegment <= nsegment; iSegment++)
          {
            // 50==================================================
            // locate the inflow input file
            // 50==================================================
            sSegment = convert_integer_to_string(iSegment,
                                                 2); // prepare filename stråing
            vReach_dummy = vStream_segment_reach.at(iSegment - 1);
            nReach_segment = vReach_dummy.size();

            // get it from data
            vReach_dummy2 = vDaily_data_in.at(iSegment - 1);
            nReach_dummy = vReach_dummy2.size();

            if (nReach_segment != nReach_dummy)
            {
              error_code = 0;
              std::cout << iSegment << ": "
                        << "Expected: " << nReach_segment << ", "
                        << "Actual: " << nReach_dummy << std::endl;

              return error_code;
            }
          }
        }

        return error_code;
      }
// 50==================================================
/*!
  Create the stream segment reach topology vector.
  After this function, the vStream_segment_reach will be updated.
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int eco3d::create_stream_segment_reach_topology()
      {
        int error_code = 1;
        int iFlag_found = 0; // a flag used to indicate search result
        int iSegment;
        int iReach;
        long lColumn, lRow;
        std::ptrdiff_t nReach_segment; // the total number of reach within a segment
        long lIndex_reach; // the column index for one single reach (optimized for
        // memory )
        long lPosition;
        std::ptrdiff_t lPosition_reach; // the matrix location of the single reach, it
        // will be used to locate actual index
        std::vector<double>::iterator iIterator_segment;
        std::vector<double>::iterator iIterator_reach;
        std::vector<long>
                vReach_dummy; // the temporal vector to store reach information
        // 50==================================================
        // vStream_reach stores the reach information for each segment
        // this global variable is what needs to be generated
        // 50==================================================
        vStream_segment_reach.clear();
        vReach_dummy.clear();
        // 50==================================================
        // start segment loop
        for (iSegment = 1; iSegment <= nsegment; iSegment++)
        {
          // https://stackoverflow.com/questions/2275076/is-stdvector-copying-the-objects-with-a-push-back
          // reset the vector for each segment
          vReach_dummy.clear();
          // get the total count of reach in this segment
          nReach_segment =
                  std::count(vStream_segment.begin(), vStream_segment.end(), iSegment);
          // find the segment, which is actually unnecessary because it must exist
          iIterator_segment =
                  std::find(vStream_segment.begin(), vStream_segment.end(), iSegment);
          // 50==================================================
          // start reach loop
          // 50==================================================
          for (iReach = 1; iReach <= nReach_segment; iReach++)
          {
            // find the all the reach starting with the iReach index,
            // since there is limited segment, the result cannot exceed the
            // nsegment
            iFlag_found = 0; // set to not found at the beginning
            iIterator_reach =
                    std::find(vStream_reach.begin(), vStream_reach.end(), iReach);
            // loop through all found reaches
            while (iIterator_reach != vStream_reach.end())
            {
              if (iFlag_found == 0)
              {
                // get the position of this reach
                lPosition_reach =
                        std::distance(vStream_reach.begin(), iIterator_reach);
                lPosition = long(lPosition_reach);
                // whether this reach is what we are looking for depends on
                // its segment index
                if (vStream_segment.at(lPosition) == iSegment)
                {
                  // this is the reach we are looking for
                  // get the index of reach
                  lRow = (long) (lPosition_reach / ncolumn);
                  lColumn = lPosition_reach % ncolumn;
                  // get the index within the index again
                  lIndex_reach =
                          calculate_index_from_coordinates(lRow, lColumn);
                  // add reach to lookuptable
                  vReach_dummy.push_back(lIndex_reach);
                  // find the next reach
                  iFlag_found = 1;
                }
                else
                {
                  // this is not the reach we are looking for
                  // because all segments have the same reach index pattern
                  iIterator_reach = std::find(++iIterator_reach,
                                              vStream_reach.end(), iReach);
                  // iFlag_found = 0; // unchanged
                }
              }
              else
              {
                // stop search since it is found already
                break;
              }
            }
          } // end reach loop
          // add all reaches to segment vector
          vStream_segment_reach.push_back(vReach_dummy);
        } // end segment loop
        // success
        return error_code;
      }

// 50==================================================
/*!
  Read the stream topology from the input file
  \param
  \return <ReturnValue>
*/
// 50==================================================
      int eco3d::read_stream_segment_topology()
      {
        int error_code = 1;
        int iDummy;
        char cDummy;
        std::ifstream ifs;
        std::string sLine;
        // test the file exist or not
        if (file_test(sFilename_stream_segment_topology) == 1)
        {
          ifs.open(sFilename_stream_segment_topology.c_str(), ios::in);
          if (ifs.good())
          {
            // read_eco3d the first line
            std::getline(ifs, sLine);
            // read_eco3d the rest lines
            // loop through all stream segments
            for (int iSegment = 0; iSegment < nsegment; iSegment++)
            {
              std::getline(ifs, sLine);
              // now we will use istringstream approach to extract information
              // from the line string object
              std::istringstream ss(sLine);
              ss >> cDummy >> aStream_segment_topology[iSegment][0] >> cDummy >>
                 iDummy >> cDummy >> aStream_segment_topology[iSegment][1];
            }

            // success
          }
          else
          {
            // could not open the file


            sLog = "===Level 3: " + sError_open_failed + sFilename_stream_segment_topology;
            ofs_log << sLog << std::endl;
            ofs_log.flush();
            std::cout << sLog << std::endl;
            error_code = 0;
          }
          ifs.close();
        }
        else
        {
          // the file does not exist


          sLog = "===Level 3: " + sError_file_missing + sFilename_stream_segment_topology;
          ofs_log << sLog << std::endl;
          ofs_log.flush();
          std::cout << sLog << std::endl;
          error_code = 0;
        }

        return error_code;
      }

// 50==================================================
/*!
  Read the external information for each stream segment
  Note that actual data is reach-based instead of segment-based.
  Because I only want to read in data at each time step, the files are openned
  and closed at each time step, too.
  \param iYear_ymd: the year of time step
  \param iMonth_ymd: the month of time step
  \param iDay_ymd: the day of time step
  \return value: a vector which contains data for each segment
*/
// 50==================================================
      std::vector<std::vector<std::vector<double>>>
      eco3d::read_stream_reach_inflow(int iYear_ymd, int iMonth_ymd, int iDay_ymd)
      {
        int iSegment;
        std::ptrdiff_t nReach_segment; // the total number of reach within a segment
        long lJulian_day; // we will use the julian day to find the start and end line
        // in input file
        long iStress_skip; // the total time step of data to be skipped
        std::string sFilename_reach_inflow; // from where we will read_eco3d the data
        std::string sSegment; // from integer to string (for filename search)
        std::string sLine;
        std::vector<std::vector<std::vector<double>>>
                vAll_data_out; // stores all the data for all segments
        std::vector<std::vector<double>>
                vSegment_data_out; // stores all the data for all reaches within a segment
        std::vector<double> vReach_data_out; // stores all thd data for single reach
        std::vector<long> vReach_dummy;      // stores all the reach indices
        std::ifstream
                ifs; // used to read_eco3d text file, it will be used multiple times
        std::vector<std::string> vTokens; // used to extract lines from text file
        // 50==================================================
        // we will use the time to find the index range
        // 50==================================================
        lJulian_day =
                date::YmdToJd(iYear_ymd, iMonth_ymd,
                              iDay_ymd); // the current julian day at this time step
        iStress_skip =
                lJulian_day - this->lJulianDay_available_start; // days to be skiped.
        // 50==================================================
        // loop through each segment because I store them separately to reduce file
        // size  50==================================================
        for (iSegment = 1; iSegment <= nsegment; iSegment++)
        {
          // 50==================================================
          // locate the inflow input file
          // 50==================================================
          sSegment =
                  convert_integer_to_string(iSegment, 2); // prepare filename stråing

          std::stringstream ss;
          ss << sWorkspace_reach_inflow << slash << "reach_inflow_" << sSegment
             << sExtension_text;
          sFilename_reach_inflow = ss.str();

          // 50==================================================
          // reset reach inflow for each segment
          // 50==================================================
          vSegment_data_out.clear();
          // 50==================================================
          // we need to get the reach count from the global vector
          // 50==================================================
          vReach_dummy = vStream_segment_reach.at(iSegment - 1);
          nReach_segment = vReach_dummy.size();
          // 50==================================================
          // now we need to make sure the inflow file exists
          // 50==================================================
          if (file_test(sFilename_reach_inflow) == 1)
          {
            ifs.open(sFilename_reach_inflow.c_str(), ios::in);
            if (ifs.good()) // the file is opened successfully
            {
              // skip the previous day data
              for (long day = 0; day < iStress_skip; day++)
              {
                // in each day, there are nReach_segment line
                for (long l = 0; l < nReach_segment; l++)
                {
                  std::getline(ifs, sLine); // read_eco3d the line
                }
              }
              // read_eco3d the actual data needed
              for (long l = 0; l < nReach_segment; l++) // skipped line
              {
                std::getline(ifs, sLine); // read_eco3d the line
                size_t iLength0 = sLine.length();
                if (iLength0 <= 3)
                {
                  // something is wrong here
                  std::cout << sError_data_missing << sFilename_reach_inflow
                            << std::endl;
                }
                else
                {
                  // split the line by comma
                  vTokens = split_string_by_delimiter(sLine, ',');
                  vReach_data_out.clear();
                  // save to the reach inflow container
                  vReach_data_out.push_back(std::stof(vTokens.at(5)));
                  vReach_data_out.push_back(std::stof(vTokens.at(6)));
                  vSegment_data_out.push_back(vReach_data_out);
                }
              } // reach loop
              // the rest data are ignored
            }
            else
            {
              std::cout << sError_open_failed << sFilename_reach_inflow
                        << std::endl;
            }
            // 50==================================================
            // close the file, this is very important because we will open
            // multiple files 50==================================================
            ifs.close();
          }
          else
          {
            // something is wrong
            std::cout << sError_file_missing << sFilename_reach_inflow
                      << std::endl;
          }
          vAll_data_out.push_back(vSegment_data_out);
        } // segment loop
        return vAll_data_out;
      }
//==============================================================
/*!
  Run the reach-based stream routing model
  \param
  \return <ReturnValue>
*/
//==============================================================
      int eco3d::run_stream_reach()
      {
        int error_code = 1;

        int iSegment_downstream;
        int iSegment;
        int iReach;
        int iMod;
        int iFlag_routed;
        long iMinute;
        long lIndex_reach0, lIndex_reach1, lIndex_reach2, lIndex_reach3;
        double dDummy0, dDummy1;

        std::ptrdiff_t nReach_segment;
        std::vector<long> vReaches;
        std::vector<std::vector<double>> vSegment_data_daily_in;
        std::vector<double> vReach_data_daily_in;
        std::vector<long> vReach_dummy;
        // minute-based reach route model



        for (iSegment = 1; iSegment <= nsegment; iSegment++)
        {
          // get the total count of reach in this segment
          vReach_dummy = vStream_segment_reach.at(iSegment - 1);
          nReach_segment = vReach_dummy.size();
          // 50==================================================
          // get the current segment vecter, the unit is daily based
          // 50==================================================
          vSegment_data_daily_in =
                  this->vReach_inflow_data_daily.at(iSegment - 1);

          for (iReach = 1; iReach <= nReach_segment; iReach++)
          {
            vReach_data_daily_in = vSegment_data_daily_in.at(iReach - 1);

            lIndex_reach0 = vReach_dummy.at(iReach - 1);

            //discharge
            vColumn[lIndex_reach0].cReach.dReach_discharge_inflow_daily_cumulative = 0.0;
            vColumn[lIndex_reach0].cReach.dReach_discharge_inflow_upstream_daily_cumulative = 0.0;

            vColumn[lIndex_reach0].cReach.dReach_discharge_outflow_daily_cumulative = 0.0;
            vColumn[lIndex_reach0].cReach.dReach_discharge_outflow_downstream_daily_cumulative = 0.0;

            //doc
            vColumn[lIndex_reach0].cReach.dReach_doc_inflow_daily_cumulative = 0.0;
            vColumn[lIndex_reach0].cReach.dReach_doc_inflow_upstream_daily_cumulative = 0.0;

            vColumn[lIndex_reach0].cReach.dReach_doc_outflow_daily_cumulative = 0.0;
            vColumn[lIndex_reach0].cReach.dReach_doc_outflow_downstream_daily_cumulative = 0.0;
          }
        }

//start minute-based simulation
        for (iMinute = 1; iMinute <= day_2_minute; iMinute++)
        {
          // 50==================================================
          // at each time step, we need to reset the upstream inflow of first reach
          // of each segment because only the first reach may receive multiple
          // upstream inflow all other reaches should be updated by its upstream
          // reach directly
          // see more details from document and
          // https://plus.google.com/u/0/+changliao1025/posts/XoN7faCiv8T
          // I think I need to assign the external inflow before the simulation
          // here.  50==================================================
          for (iSegment = 1; iSegment <= nsegment; iSegment++)
          {
            // get the total count of reach in this segment
            vReach_dummy = vStream_segment_reach.at(iSegment - 1);
            nReach_segment = vReach_dummy.size();
            // 50==================================================
            // get the current segment vecter, the unit is daily based
            // 50==================================================
            vSegment_data_daily_in =
                    this->vReach_inflow_data_daily.at(iSegment - 1);

            for (iReach = 1; iReach <= nReach_segment; iReach++)
            {
              // unit daily
              vReach_data_daily_in = vSegment_data_daily_in.at(iReach - 1);

              lIndex_reach0 = vReach_dummy.at(iReach - 1);

              // unit cms to cm per minute
              dDummy0 = vReach_data_daily_in.at(0);
              if (dDummy0 < 0.0)
              {
                dDummy0 = 0.0;
              }
              dDummy0 = dDummy0 * minute_2_second;
              if(iSegment==1 && iReach==1)
              {
                if (dDummy0 < 10.0)
                {
                  dDummy0 = 10.0;
                }
              }

              vColumn[lIndex_reach0]
                      .cReach.dReach_discharge_inflow_external_time_step = dDummy0;

              vColumn[lIndex_reach0]
                      .cReach.dReach_discharge_inflow_external_daily =
                      dDummy0 * vColumn[lIndex_reach0].cReach.iTime_step;

              // doc
              dDummy1 = vReach_data_daily_in.at(1);
              // micromolar	μM	10−6 mol/L	10−3 mol/m3
              // convert it mass
              dDummy1 = dDummy1 * 1.0E-3 * (12.0 * 1.0E-3) * dDummy0;

              if (dDummy1 < 0.0)
              {
                dDummy1 = 0.0;
              }
              vColumn[lIndex_reach0]
                      .cReach.dReach_doc_inflow_external_time_step = dDummy1;

              vColumn[lIndex_reach0].cReach.dReach_doc_inflow_external_daily =
                      dDummy1 * vColumn[lIndex_reach0].cReach.iTime_step;
            }
          }
          // 50==================================================
          // simulate each reach
          // segment loop
          // 50==================================================
          for (iSegment = 1; iSegment <= nsegment; iSegment++)
          {
            // get the total count of reach in this segment
            vReach_dummy = vStream_segment_reach.at(iSegment - 1);
            nReach_segment = vReach_dummy.size();
            // 50==================================================
            // start reach loop
            // 50==================================================
            for (iReach = 1; iReach <= nReach_segment; iReach++)
            {
              // get the index within the index
              lIndex_reach1 = vReach_dummy.at(iReach - 1);
              // 50==================================================
              // run simulation for this reach, important step
              // 50==================================================
              vColumn[lIndex_reach1].cReach.run_reach_model(iMinute);

              iMod = iMinute % int(vColumn[lIndex_reach1].cReach.dReach_travel_time_minute);

             /*
              if (iMod == 0 && lIndex_reach1 == 8828)
              {
                cout << vColumn[lIndex_reach1].cReach.iTime_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_discharge_inflow_previous_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_discharge_outflow_previous_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_discharge_storage_previous_time_step << ", "

                     << vColumn[lIndex_reach1].cReach.dReach_discharge_inflow_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_discharge_outflow_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_discharge_storage_time_step << ", "
                     << endl;
                cout << vColumn[lIndex_reach1].cReach.dReach_doc_inflow_previous_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_doc_outflow_previous_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_doc_storage_previous_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_doc_concentration_previous_time_step << ", "

                     << vColumn[lIndex_reach1].cReach.dReach_doc_inflow_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_doc_outflow_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_doc_storage_time_step << ", "
                     << vColumn[lIndex_reach1].cReach.dReach_doc_concentration_time_step << ", "

                     << endl;

                cout<<"=================="<<endl;

              }
              */

              //add up

              if (iMod == 0)
              {
                vColumn[lIndex_reach1].cReach.dReach_discharge_inflow_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_discharge_inflow_time_step;

                vColumn[lIndex_reach1].cReach.dReach_discharge_inflow_upstream_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_discharge_inflow_upstream_time_step;

                vColumn[lIndex_reach1].cReach.dReach_discharge_outflow_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_discharge_outflow_time_step;

                vColumn[lIndex_reach1].cReach.dReach_discharge_outflow_downstream_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_discharge_outflow_downstream_time_step;

                //doc
                vColumn[lIndex_reach1].cReach.dReach_doc_inflow_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_doc_inflow_time_step;

                vColumn[lIndex_reach1].cReach.dReach_doc_inflow_upstream_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_doc_inflow_upstream_time_step;

                vColumn[lIndex_reach1].cReach.dReach_doc_outflow_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_doc_outflow_time_step;

                vColumn[lIndex_reach1].cReach.dReach_doc_outflow_downstream_daily_cumulative +=
                        vColumn[lIndex_reach1].cReach.dReach_doc_outflow_downstream_time_step;
                //update for this time step
                vColumn[lIndex_reach1].cReach.update_reach_time_step_status();
              }

            } // reach loop
          }     // segment loop
          // 50==================================================
          // reset
          // 50==================================================
          for (iSegment = 1; iSegment <= nsegment; iSegment++)
          {
            // get the total count of reach in this segment
            vReach_dummy = vStream_segment_reach.at(iSegment - 1);
            nReach_segment = vReach_dummy.size();
            // 50==================================================
            // start reach loop
            // 50==================================================
            for (iReach = 1; iReach <= nReach_segment; iReach++)
            {
              // get the index within the index
              lIndex_reach1 = vReach_dummy.at(iReach - 1);
              iFlag_routed = vColumn[lIndex_reach1].cReach.iFlag_just_routed;
              if (iFlag_routed == 1)
              {
                vColumn[lIndex_reach1]
                        .cReach.dReach_discharge_inflow_upstream_time_step = 0.0;

                vColumn[lIndex_reach1]
                        .cReach.dReach_doc_inflow_upstream_time_step = 0.0;
              }
              else
              {
                //still accumulating
              }

            } // reach loop
          }     // segment loop
          // 50==================================================
          // routing
          // 50==================================================
          for (iSegment = 1; iSegment <= nsegment; iSegment++)
          {
            // get the total count of reach in this segment
            vReach_dummy = vStream_segment_reach.at(iSegment - 1);
            nReach_segment = vReach_dummy.size();

            for (iReach = 1; iReach <= nReach_segment; iReach++)
            {
              lIndex_reach1 = vReach_dummy.at(iReach - 1);
              // 50==================================================
              // update the system based on topography relation
              // 50==================================================
              // whether this reach is what we are looking for depends on its
              // segment index
              // 50==================================================
              if (iReach < nReach_segment) // this is not the outlet
              {
                // you find it, but that is not the end, now you need to find
                // its downstream reach_index2 is the downstream reach of
                // current reach
                lIndex_reach2 = vReach_dummy.at(iReach);
                // we can only set the upstream inflow for the downstream
                // reach


                vColumn[lIndex_reach2]
                        .cReach.dReach_discharge_inflow_upstream_time_step +=
                        vColumn[lIndex_reach1]
                                .cReach.dReach_discharge_outflow_time_step;

                vColumn[lIndex_reach2]
                        .cReach.dReach_doc_inflow_upstream_time_step +=
                        vColumn[lIndex_reach1]
                                .cReach.dReach_doc_outflow_time_step;
              }
              else
              {
                // this is the last reach, we have to go to the downstream
                // segment
                iSegment_downstream =
                        aStream_segment_topology[iSegment - 1][1];

                if (iSegment_downstream > 0 &&
                        iSegment_downstream <= nsegment)
                {
                  lIndex_reach3 =
                          (vStream_segment_reach.at(iSegment_downstream - 1))
                                  .at(0);


                  // one segment may receive multiple upstream inflow
                  // so the operator is +=
                  vColumn[lIndex_reach3]
                          .cReach.dReach_discharge_inflow_upstream_time_step +=
                          vColumn[lIndex_reach1]
                                  .cReach.dReach_discharge_outflow_time_step;

                  vColumn[lIndex_reach3]
                          .cReach.dReach_doc_inflow_upstream_time_step +=
                          vColumn[lIndex_reach1]
                                  .cReach.dReach_doc_outflow_time_step;

                  // external inflow?
                }
                else
                {
                  // this is the outlet of the watershed
                  // do nothing here
                }
              }
            }
          }

        } // time loop
        // do we need to update the daily flow here???
        return error_code;
      }
  } // namespace ecohydrology
