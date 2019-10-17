// 50==================================================
/*!
  \file ecosystem_run.cpp
  \brief
  <long description>
  \author Chang Liao
  \date 2017-07-28
*/
// 50==================================================
#include "eco3d.h"

namespace ecohydrology
  {
// 50==================================================
/*!
//run the ecosystem system
\param
\return <ReturnValue>
*/
// 50==================================================
  int eco3d::run_and_save_eco3d ()
    {
    int error_code = 1;
    // this->ofs_log << "Start to run the ecosystem simulation!" << std::endl;
    // we must prepare the cascade parameter first
    if (iFlag_run_cascade_model == 1)
    {
      error_code = call_cascade_model ();
    }

    this->ofs_budget.open (this->sFilename_budget_table.c_str (), ios::out);

    if (1 == this->iFlag_debug)
    {
      this->ofs_debug.open (this->sFilename_debug_table.c_str (), ios::out);
    }

    sLog = "===Level 3: Start to run and save model simulation ";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;
    if (1 == error_code)
    {
      // run steady state simulation
      if (iFlag_run_steady_state == 1)
      {
        // this->ofs_log << "Start to run the ecosystem simulation!" <<
        // std::endl;
        sLog = "====Level 4: Start to run steady state simulation ";
        ofs_log << sLog << std::endl;
        ofs_log.flush ();
        std::cout << sLog << std::endl;
        error_code = run_steady_state_simulation ();
        if (1 == error_code)
        {
          sLog = "====Level 4: Finished running steady state simulation ";
          ofs_log << sLog << std::endl;
          ofs_log.flush ();
          std::cout << sLog << std::endl;
        }
        else
        {
        }

        sLog = "===Level 3: Finished running and save model simulation ";
        ofs_log << sLog << std::endl;
        ofs_log.flush ();
        std::cout << sLog << std::endl;
      }
      else
      {
      }

      if (iFlag_run_transient == 1)
      {
        sLog = "====Level 4: Start to run transient simulation ";
        ofs_log << sLog << std::endl;
        ofs_log.flush ();
        std::cout << sLog << std::endl;
        error_code = run_transient_simulation ();
        if (1 == error_code)
        {
          sLog = "====Level 4: Finished running transient simulation ";
          ofs_log << sLog << std::endl;
          ofs_log.flush ();
          std::cout << sLog << std::endl;
        }
        else
        {
        }
      }
      else
      {
        // failed
        error_code = 0;
      }
    }
    else
    {
      // failed
    }
    return error_code;
    }
// 50==================================================
/*!
//steady state simulation
\param
\return <ReturnValue>
*/
// 50==================================================
  int eco3d::run_steady_state_simulation ()
    {
    int error_code;
    sLog = "==Level 2: Start to run transient simulation ";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;
    // 50==================================================
    // the default startand end time
    // 50==================================================
    // ss simulation
    int iFlag_mode = 0; // this is used to flag ss or tr
    error_code =
        run_modules (iFlag_mode, iDay_end_ss, iDay_start_ss, iMonth_end_ss,
                     iMonth_start_ss, iYear_end_ss, iYear_start_ss);
    this->ofs_log << "Steady state simulation is finished!" << std::endl;
    return error_code;
    }
// 50==================================================
/*!
//Transient simulation
\param
\return <ReturnValue>
*/
// 50==================================================
  int eco3d::run_transient_simulation ()
    {
    int error_code;

    sLog = "==Level 2: Start to run transient simulation ";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;
    // ss simulation
    int iFlag_mode = 1;
    error_code =
        run_modules (iFlag_mode, iDay_end_tr, iDay_start_tr, iMonth_end_tr,
                     iMonth_start_tr, iYear_end_tr, iYear_start_tr);
    // this->ofs_log << "Transient simulation is finished!" << std::endl;
    return error_code;
    }
// 50==================================================
/*!
//This is the core function to run grid-based ecosystem algrothms
//The Philosophy of the sequence in which each algorithm is excuted is based on
the physical
//location instead of classification. In general, the direction of this location
movements is from
//top to bottom, which means we generally first calculate atmosphere components,
and calculate groundwater
//flow in the last.
//after each time step, the system status variables will be updated.
\param iDay_end
\param iDay_start
\param iFlag_mode
\param iMonth_end
\param iMonth_start
\param iYear_end
\param iYear_start
\return <ReturnValue>
*/
// 50==================================================
  int eco3d::run_modules (int iFlag_mode, int iDay_end, int iDay_start,
                          int iMonth_end, int iMonth_start, int iYear_end,
                          int iYear_start)
    {
    int error_code = 1;
    // 50==================================================
    int iDay_doy, iYear; // The time variables used in actual loop
    int iDay_doy_start1;
    int iDay_doy_end1; // The time variables used for each year simulation
    int iDay_in_year;  // the total number of days in a year, when it's a
    // leap, it will be changed to 366
    int iYear_ymd, iMonth_ymd, iDay_ymd; // the variables used to convert between
    int iDay_doy_dummy;
    // day_of_year to month and day
    // 50==================================================
    int iBoundary; // boundary flag which indicates whether this column/grid is
    // active or not

    int iSoil_type;       // the soil type of the column
    int iLand_cover_type; // land cover from modis, the data is converted from
    // double to integer
    int iVegetation_type; // the vegetation is retrieved from land cover type
    // 50==================================================
    int iFlag_precipitation = 0;
    int iFlag_run_carbon_cascade = 1;
    int iFlag_data_availability = 0; // when all data are available
    // 50==================================================
    long lRow, lColumn, lIndex; // the two dimensioal column, row and one
    // dimensional index of the column/grid
    long lJuiday; // the juiday
    // 50==================================================
    double dTemperature_max;
    double dTemperature_min;
    double dTemperature_dewpoint;
    double dTemperature_basin_max;
    double dPrecipitation;
    double dPrecipitation_basin; // atmospheric inputs
    double dFpar;
    double dFpar_basin;      // fpar from modis data
    double dLai, dLai_basin; // lai from modis
    double dSurface_depression_percentage = 0.0;
    double dImpervious_surface_percentage = 0.0;
    double dPervious_surface_percentage = 1.0;
    eColumn_type eCU_type;
    // 50==================================================
    std::string sYear, sDay_doy, sDay1,
        sDay_doy_dummy;                      // strings used to locate files
    std::vector<double>::iterator vIterator; // interator used to ???
    // the filename of input to be read_eco3d for each day
    std::string sFilename_dewpoint;
    std::string sFilename_tmax;
    std::string sFilename_tmin;
    std::string sFilename_prec;
    std::string sFilename_fpar;
    std::string sFilename_lai;
    std::string sFilename_land_cover;
    // the vector to store the actual data
    std::vector<double> vDewpoint;
    std::vector<double> vTmax;
    std::vector<double> vTmin;
    std::vector<double> vPrec;
    std::vector<double> vFpar;
    std::vector<double> vLai;
    // a newland cover type
    std::vector<double> vLand_cover_type;
    std::stringstream ss;

    sLog = "===Level 3: Start to run module simulation ";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;

    // start time-variant loop
    for (iYear = iYear_start; iYear <= iYear_end; iYear++)
    {

      if (iYear >= iYear_available_start && iYear <= iYear_available_end)
      {
        iFlag_data_availability = 1;
      }
      else
      {
        iFlag_data_availability = 0;
      }
      if (iYear > iYear_end_save)
      {
        return error_code;
      }
      sYear = convert_integer_to_string (iYear, 4);

      sLog = "====Level 4: Start to run module simulation for year: " + sYear;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      std::cout << sLog << std::endl;
      if (date::IsLeapYear (iYear) != 1)
      {
        iDay_in_year = 365;
      }
      else
      {
        iDay_in_year = 366;
      }
      if (iYear == iYear_start)
      {
        date da (iDay_start, iMonth_start, iYear_start);
        iDay_doy_start1 = da.DayOfYear ();
      }
      else
      {
        iDay_doy_start1 = 1;
      }
      if (iYear == iYear_end)
      {
        date da (iDay_end, iMonth_end, iYear_end);
        iDay_doy_end1 = da.DayOfYear ();
      }
      else
      {
        iDay_doy_end1 = iDay_in_year;
      }
      // read_eco3d land cover data, it only has one file per year
      ss.str ("");
      ss << sWorkspace_land_cover_type << slash << sYear << slash
         << sPrefix_land_cover_type << sYear << "001" << sExtension_envi;
      sFilename_land_cover = ss.str ();
      // deal with missing file
      // if some climate data file is missing, we use the same day_of_year file
      // from the nearest year.
      // for example, if the precipitation file prec2001001.dat is missing and
      // prec2000001.dat is available,
      // then the latter one will be used.
      if (file_test (sFilename_land_cover) != 1)
      {
        sFilename_land_cover = miscellaneous::find_alternative_file (
            1, iYear, sExtension_envi, sPrefix_land_cover_type,
            sWorkspace_land_cover_type);
      }
      // read_eco3d the binary data
      vLand_cover_type = data::read_binary_vector (sFilename_land_cover);
      // other than land cover data, other climate data have daily or 8-day
      // temporal resolution
      // start the daily simulation using the real day index within each year.
      for (iDay_doy = iDay_doy_start1; iDay_doy <= iDay_doy_end1; iDay_doy++)
      {
        // 50==================================================
        // step one: read all data for one day
        // 50==================================================
        // calculate the calendar
        lJuiday = date::YmdToJd (iYear, 1, 1) + iDay_doy - 1;
        date::JdToYmd (lJuiday, &iYear_ymd, &iMonth_ymd, &iDay_ymd);
        sDay_doy = convert_integer_to_string (iDay_doy, 3);
        sLog = "=====Level 5: Start to run module simulation for year: " +
               sYear + " day: " + sDay_doy;
        ofs_log << sLog << std::endl;
        ofs_log.flush ();
        //std::cout << sLog << std::endl;

        ss.str ("");
        ss << sWorkspace_dewpoint << slash << sYear << slash
           << sPrefix_dewpoint << sYear << sDay_doy << sExtension_envi;
        sFilename_dewpoint = ss.str ();
        if (file_test (sFilename_dewpoint) != 1)
        {
          sFilename_dewpoint = miscellaneous::find_alternative_file (
              iDay_doy, iYear, sExtension_envi, sPrefix_dewpoint,
              sWorkspace_dewpoint);
        }
        ss.str ("");
        ss << sWorkspace_tmax << slash << sYear << slash << sPrefix_tmax
           << sYear << sDay_doy << sExtension_envi;
        sFilename_tmax = ss.str ();
        if (file_test (sFilename_tmax) != 1)
        {
          sFilename_tmax = miscellaneous::find_alternative_file (
              iDay_doy, iYear, sExtension_envi, sPrefix_tmax,
              sWorkspace_tmax);
        }
        ss.str ("");
        ss << sWorkspace_tmin << slash << sYear << slash << sPrefix_tmin
           << sYear << sDay_doy << sExtension_envi;
        sFilename_tmin = ss.str ();
        if (file_test (sFilename_tmin) != 1)
        {
          sFilename_tmin = miscellaneous::find_alternative_file (
              iDay_doy, iYear, sExtension_envi, sPrefix_tmin,
              sWorkspace_tmin);
        }
        ss.str ("");
        ss << sWorkspace_prec << slash << sYear << slash << sPrefix_prec
           << sYear << sDay_doy << sExtension_envi;
        sFilename_prec = ss.str ();
        if (file_test (sFilename_prec) != 1)
        {
          sFilename_prec = miscellaneous::find_alternative_file (
              iDay_doy, iYear, sExtension_envi, sPrefix_prec,
              sWorkspace_prec);
        }
        // climate data not at a daily resolution
        iDay_doy_dummy = (iDay_doy / 4) * 4 + 1;
        sDay_doy_dummy = convert_integer_to_string (iDay_doy_dummy, 3);
        ss.str ("");
        ss << sWorkspace_fpar << slash << sYear << slash << sPrefix_fpar
           << sYear << sDay_doy_dummy << sExtension_envi;
        sFilename_fpar = ss.str ();
        if (file_test (sFilename_fpar) != 1)
        {
          sFilename_fpar = miscellaneous::find_alternative_file (
              iDay_doy_dummy, iYear, sExtension_envi, sPrefix_fpar,
              sWorkspace_fpar);
        }
        ss.str ("");
        ss << sWorkspace_lai << slash << sYear << slash << sPrefix_lai
           << sYear << sDay_doy_dummy << sExtension_envi;
        sFilename_lai = ss.str ();
        if (file_test (sFilename_lai) != 1)
        {
          sFilename_lai = miscellaneous::find_alternative_file (
              iDay_doy_dummy, iYear, sExtension_envi, sPrefix_lai,
              sWorkspace_lai);
        }
        // read_eco3d all binary data
        vDewpoint = data::read_binary_vector (sFilename_dewpoint);
        vTmax = data::read_binary_vector (sFilename_tmax);
        vTmin = data::read_binary_vector (sFilename_tmin);
        vPrec = data::read_binary_vector (sFilename_prec);
        // 4 day resolution
        vFpar = data::read_binary_vector (sFilename_fpar);
        vLai = data::read_binary_vector (sFilename_lai);
        // 50==================================================
        // statictics
        vIterator = std::max_element (vTmax.begin (), vTmax.end ());
        dTemperature_basin_max = *vIterator; // the maximum air temperature
        dTemperature_basin_max =
            dTemperature_basin_max + celsius_2_kelvin; // convert the unit
        dPrecipitation_basin = calculate_mean (
            vPrec, 750.0, 0.0); // the mean precipitation, unit millimeter
        dPrecipitation_basin = dPrecipitation_basin * millimeter_2_meter;
        // if basin mean precipitation is too low we assume it is 0.0
        if (dPrecipitation_basin <
            small_value) // 0.1mm is the minimam requirement
        {
          dPrecipitation_basin = 0.0;
          iFlag_precipitation = 0;
        }
        else
        {
          iFlag_precipitation = 1;
        }
        dLai_basin = calculate_mean (vLai, 10.0, 0.0); // the mean lai
        if (dLai_basin < small_value)
        {
          dLai_basin = 0.2;
        }
        dFpar_basin = calculate_mean (vFpar, 1.0, 0.0);
        if (dFpar_basin < small_value)
        {
          dFpar_basin = 0.5;
        }
        // 50==================================================
        // similar to other input data, the stream reach data are now
        // read_eco3d in  at daily time step!
        // 50==================================================
        if (iFlag_mode == 1 && 1 == iFlag_data_availability)
        {
          
          vReach_inflow_data_daily =
              read_stream_reach_inflow (iYear_ymd, iMonth_ymd, iDay_ymd);
          // we should check the stream data here
          error_code = check_stream_segment_reach_daily_data (
              vReach_inflow_data_daily);
          if (error_code != 1)
          {
            // stream data has error
            // this->ofs_log << "Stream data error " << std::endl;
            return error_code;
          }
        }
        // 50==================================================
        // step 2: update, run
        // start loop for each active column/grid
        // 50==================================================
        // c++ is row major language
        for (lRow = 0; lRow < nrow; lRow++)
        {
          // 50==================================================
          // use openmp for parallel computing
          // 50==================================================

#pragma omp parallel for private(                                              \
    lColumn, lIndex, iBoundary, iSoil_type, iLand_cover_type, \
    iVegetation_type, dTemperature_max, dTemperature_min, \
    dTemperature_dewpoint, dPrecipitation, dLai, dFpar, \
    dSurface_depression_percentage, dImpervious_surface_percentage, \
    dPervious_surface_percentage)

          // 50=================================================
          for (lColumn = 0; lColumn < ncolumn; lColumn++)
          {
            // convert from 2D index to 1D index
            lIndex = calculate_index_from_coordinates (lRow, lColumn);
            if (lIndex >= 0)
            {
              if (iFlag_debug == 1)
              {
                if (iFlag_run_cascade == 1)
                {
                }
                else
                {
                  if (lIndex != lCU_index_debug)
                  {
                    continue;
                  }
                }
              }

              // quality control of data

              dTemperature_max =
                  vTmax[lRow * ncolumn + lColumn] + celsius_2_kelvin;
              if (dTemperature_max < 0.0 || dTemperature_max > 363.15)
              {
                dTemperature_max = 20.0 + 273.15;
              }
              dTemperature_min =
                  vTmin[lRow * ncolumn + lColumn] + celsius_2_kelvin;
              if (dTemperature_min < 0.0 || dTemperature_min > 363.15)
              {
                dTemperature_min = 273.15;
              }
              // dewpoint
              dTemperature_dewpoint =
                  vDewpoint[lRow * ncolumn + lColumn] +
                  celsius_2_kelvin;
              dPrecipitation =
                  vPrec[lRow * ncolumn + lColumn] *
                  millimeter_2_meter; // convert from mm to meter
              // double check precipitation again
              if (iFlag_precipitation == 0)
              {
                // there is no basin scale precipitation
                if (dPrecipitation >= small_value) // minimam 0.1mm
                {
                  // this pixel still have precipitation even though
                  // the mean is small
                }
                else
                {
                  dPrecipitation = 0.0;
                }
              }
              else
              {
                // there is a basin scale precipitation going on
                if (dPrecipitation < small_value)
                {
                  dPrecipitation =
                      0.0; // if the precipitation is too small
                }
                if (dPrecipitation > 0.3)
                {
                  dPrecipitation = 0.3;
                }
              }
              dLai = vLai[lRow * ncolumn + lColumn];
              if (dLai > 10.0 || dLai < small_value)
              {
                dLai = dLai_basin;
              }
              dFpar = vFpar[lRow * ncolumn + lColumn];
              if (dFpar > 1.0)
              {
                dFpar = 1.0;
              }
              if (dFpar < small_value)
              {
                dFpar = dFpar_basin;
              }
              // we are supposed to update soil type, but currently this
              // time is unavailable
              iSoil_type = int (vSoil_type[lRow * ncolumn + lColumn]);
              iLand_cover_type =
                  int (vLand_cover_type[lRow * ncolumn + lColumn]);
              if (iLand_cover_type == missing_value)
              {
                iLand_cover_type = -1;
              }
              iVegetation_type =
                  vegetation::convert_land_cover_to_vegetation_type (
                      iLand_cover_type);
              // we need to update some parameters
              vColumn[lIndex].update_column_parameter (
                  iFlag_run_carbon_cascade,
                  iFlag_run_water_cascade,
                  iLand_cover_type,
                  iSoil_type, iVegetation_type,
                  dImpervious_surface_percentage,
                  dPervious_surface_percentage,
                  dSurface_depression_percentage);
              // run

              vColumn[lIndex].run_column_model (
                  iDay_doy, iMonth_ymd, dFpar, dLai, dPrecipitation,
                  dPrecipitation_basin, dTemperature_basin_max,
                  dTemperature_dewpoint, dTemperature_max,
                  dTemperature_min);

              // output the system variables for debug here.
              if (iFlag_debug == 1)
              {
                // we can only debug for single pixel if cascade is
                // not required.

                if (iFlag_debug_land == 1)
                {
                  if (lIndex == lCU_index_debug)
                  {

                    std::cout
                        << iYear << " " << iDay_doy
                        << " doc upslope: "
                        << vColumn[lIndex]
                            .cLand.cSoil.dSoil_doc_inflow_upslope
                        << " dos downslope: "
                        << vColumn[lIndex]
                            .cLand.cSoil.dSoil_doc_outflow_downslope


                        << "  doc concentration: "
                        << vColumn[lIndex]
                            .cLand.cSoil.dSoil_doc_concentration
                       


                        << std::endl;

                    this->ofs_debug
                        << vColumn[lIndex]
                            .cAtmosphere.cPrecipitation
                            .dTemperature_rain
                        << ","

                        << std::endl;
                  }
                }
              }

            } // within boundary
            else
            {
              // set variable to missing value since no calculation will
              // be conducted
              // continue;
            }
          } // column loop
        }     // row loop
        // 50==================================================
        // step 3: reset cascade
        // Important! because upslope is calculated every time, so we must set
        // everything to 0.0 before cascade
        // 50==================================================

        if (iFlag_run_cascade == 1)
        {
          for (lRow = 0; lRow < nrow; lRow++)
          {
#pragma omp parallel for private(lColumn, lIndex, iBoundary, eCU_type)
            for (lColumn = 0; lColumn < ncolumn; lColumn++)
            {
              iBoundary = (int) vBoundary[lRow * ncolumn + lColumn];

              lIndex = calculate_index_from_coordinates (lRow, lColumn);

              if (iBoundary != missing_value && lIndex >= 0)
              {
                // only land and glacier cascade, lake and stream is
                // handled separately

                eCU_type = vColumn[lIndex].eCU_type;
                switch (eCU_type)
                {
                  case eCU_glacier: // glacier
                  {
                    vColumn[lIndex]
                        .cGlacier.cSurface_runoff
                        .dSurface_runoff_inflow_upslope = 0.0;
                    vColumn[lIndex]
                        .cLand.cSurface_runoff
                        .dSurface_runoff_inflow = 0.0;
                  }
                    break;
                  case eCU_lake: // lake
                    break;
                  case eCU_land: // land
                  {

                    // litter
                    vColumn[lIndex]
                        .cLand.cSurface_runoff
                        .dSurface_runoff_inflow = 0.0;

                    vColumn[lIndex]
                        .cLand.cSurface_runoff
                        .dSurface_runoff_inflow_upslope = 0.0;

                    // soil interflow
                    vColumn[lIndex]
                        .cLand.cSoil.dSoil_interflow_fast_upslope =
                        0.0;
                    vColumn[lIndex]
                        .cLand.cSoil.dSoil_interflow_slow_upslope =
                        0.0;

                    // groundwater flow
                    vColumn[lIndex]
                        .cLand.cGroundwater
                        .dGroundwater_inflow_upslope = 0.0;
                    // doc flux
                    vColumn[lIndex]
                        .cLand.cLitter.dLitter_doc_inflow_upslope =
                        0.0;
                    vColumn[lIndex]
                        .cLand.cSoil.dSoil_doc_inflow_upslope = 0.0;
                  }
                    break;
                  case eCU_stream: // stream
                  {
                    vColumn[lIndex]
                        .cReach
                        .dReach_discharge_inflow_lateral_daily =
                        0.0;
                    vColumn[lIndex]
                        .cReach.dReach_doc_inflow_lateral_daily =
                        0.0;
                    vColumn[lIndex]
                            .cReach.dReach_doc_inflow_lateral_litter_daily =
                            0.0;
                    vColumn[lIndex]
                            .cReach.dReach_doc_inflow_lateral_soil_daily =
                            0.0;


                  }
                    break;
                  case eCU_swale: // swale
                    break;
                  default:break;
                }
              }
              else
              {
                // nothing
              }
            }
          }
        }
        // 50==================================================
        // step 4: cascade
        // 50==================================================
        // we only run cascade during transient and data is available
        if (iFlag_run_cascade == 1)
        {
          if (iFlag_mode == 1 && 1 == iFlag_data_availability)
          {
            // start
            // cascade==============================================================================
            // this->ofs_log << "====Starting cascade simulation!" <<
            // endl;
            for (lRow = 0; lRow < nrow; lRow++)
            {
#pragma omp parallel for private(lColumn, lIndex, iBoundary, eCU_type)
              for (lColumn = 0; lColumn < ncolumn; lColumn++)
              {
                iBoundary = (int) vBoundary[lRow * ncolumn + lColumn];
                lIndex =
                    calculate_index_from_coordinates (lRow, lColumn);
                if (iBoundary != missing_value && lIndex >= 0)
                {
                  // only land and glacier cascade, lake and stream
                  // is handled separately

                  eCU_type = vColumn[lIndex].eCU_type;
                  switch (eCU_type)
                  {
                    case eCU_glacier: // glacier
                    {
                      all_cascade (lRow, lColumn);
                    }
                      break;
                    case eCU_lake: // lake
                      break;
                    case eCU_land: // land
                    {
                      all_cascade (lRow, lColumn);
                    }
                      break;
                    case eCU_stream: // stream
                      break;
                    case eCU_swale: // swale
                      break;
                    default:break;
                  }
                }
                else
                {
                  // nothing
                }
              }
            }
            // this->ofs_log << "====Finishing cascade simulation!" <<
            // endl;
          }
        }
        // 50==================================================
        // step 5: stream routing
        // simulate stream reach interactions
        // 50==================================================
        if (iFlag_run_cascade == 1)
        {
          if (iFlag_mode == 1 && 1 == iFlag_data_availability)
          {
            run_stream_reach ();
          }
        }
        // 50==================================================
        // step 6 : update
        // 50==================================================

        for (lRow = 0; lRow < nrow; lRow++)
        {
#pragma omp parallel for private(lColumn, lIndex, iBoundary)
          for (lColumn = 0; lColumn < ncolumn; lColumn++)
          {
            iBoundary = (int) vBoundary[lRow * ncolumn + lColumn];

            lIndex = calculate_index_from_coordinates (lRow, lColumn);
            if (iBoundary != missing_value && lIndex >= 0)
            {
              if (iFlag_debug == 1)
              {
                if (iFlag_run_cascade == 1)
                {
                  vColumn[lIndex].update_column_status ();
                }
                else
                {
                  if (lIndex != lCU_index_debug)
                  {
                    continue;
                  }
                  else
                  {
                    vColumn[lIndex].update_column_status ();
                  }
                }
              }
              else
              {
                vColumn[lIndex].update_column_status ();
              }

            }
          }
        }
        // only print in debug model during TR simulation
        if (iFlag_debug == 1 && iFlag_mode == 1 &&
            1 == iFlag_data_availability)
        {

          if (iFlag_debug_land == 1)
          {
            //std::cout << "  NEP: " << vColumn[lCU_index_debug].dColumn_nep
            //        << std::endl;
          }

          if (iFlag_debug_reach == 1)
          {

            /*std::array<double, 18> dummy_index{
             36241, 36240, 36239, 35946, 35362, 34496,
             32722, 32126, 30613, 30300, 30299, 30298,
             30297, 30296, 30295, 30294, 29980, 30293};*/
            std::vector<double> dummy_index{3624, 3803, 3983, 4162,
                4340, 4519, 4699, 4883};
            for (int i = 0; i < dummy_index.size (); i++)
            {
              std::cout
                  << (i + 1) << ": "
                  << vColumn[dummy_index[i]].cReach.iTime_step << ", "

                  << vColumn[dummy_index[i]]
                      .cReach.dReach_discharge_inflow_daily
                  << ", "
                  << vColumn[dummy_index[i]]
                      .cReach.dReach_discharge_outflow_daily
                  << ", "
                  << vColumn[dummy_index[i]]
                      .cReach.dReach_discharge_storage_daily
                  << ", "

                  << vColumn[dummy_index[i]]
                      .cReach.dReach_doc_inflow_daily
                  << ", "

                  << vColumn[dummy_index[i]]
                      .cReach.dReach_doc_outflow_daily
                  << ", "

                  << vColumn[dummy_index[i]]
                      .cReach.dReach_doc_storage_daily
                  << ", "

                  << vColumn[dummy_index[i]]
                      .cReach.dReach_doc_concentration_time_step
                  << std::endl;
            }
            std::cout
                << "  Inflow discharge: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_daily
                << std::endl
                << "  Inflow discharge upstream: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_upstream_daily
                << std::endl
                << "  Inflow discharge lateral: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_lateral_daily
                << std::endl
                << "  Inflow discharge external: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_external_daily
                << std::endl
                << "  Outflow discharge downstream: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_outflow_daily
                << std::endl

                << "  Discharge storage: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_storage_time_step
                << std::endl

                << "  Inflow doc: "
                << vColumn[lCU_index_debug].cReach.dReach_doc_inflow_daily
                << std::endl
                << "  Inflow doc upstream: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_inflow_upstream_daily
                << std::endl
                << "  Inflow doc lateral: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_inflow_lateral_daily
                << std::endl
                << "  Inflow doc external: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_inflow_external_daily
                << std::endl
                << "  Outflow doc downstream: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_outflow_daily
                << std::endl

                << "  DOC concentration: "
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_concentration_time_step
                << std::endl;
            // save to debug file
            this->ofs_debug
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_upstream_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_lateral_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_inflow_external_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_outflow_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_discharge_storage_time_step
                << ","

                << vColumn[lCU_index_debug].cReach.dReach_doc_inflow_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_inflow_upstream_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_inflow_lateral_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_inflow_external_daily
                << ","
                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_outflow_daily
                << ","

                << vColumn[lCU_index_debug]
                    .cReach.dReach_doc_concentration_time_step
                << std::endl;
          }
        }

        if (iFlag_debug == 1)
        {
          // we do not save result during debug
        }
        else
        {
          // 50==================================================
          // step 7: summarize and save result
          // 50==================================================
          summarize (iYear, iDay_doy);
          if (iFlag_mode == 1)
          {

            if (iYear >= iYear_start_save && iYear <= iYear_end_save)
            {
              error_code = save_system_status (iDay_doy, iYear);
            }
          }
          else
          {
            if (1 == iFlag_debug)
            {
              error_code = save_system_status (iDay_doy, iYear);
            }
          }
        }

      } // day loop
      // 50==================================================
      // step 8: check steady state after the end of one year
      // 50==================================================

      if (iFlag_mode == 0) // steady state simulation
      {
        if (iYear >= (iYear_start + 100))
        {
          check_steady_state ();
          if (iFlag_steady_state == 1)
          {
            // if the model reach ss, then stop ss immediately
            // this->ofs_log << "====Steady state is reached!" <<
            // endl;
            error_code = save_system_status (iDay_doy, iYear);
            return error_code;
          }
        }
      }

      // compare system dynamics
    } // year loop
    return error_code;
    }
  } // namespace ecohydrology
