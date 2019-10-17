// 50==================================================
/*!
  \file   ecosystem_read.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-16
*/
// 50==================================================

#include "eco3d.h"

namespace ecohydrology
  {
  int eco3d::read_eco3d ()
    {
    int error_code = 1;
    read_configuration_file ();
    retrieve_user_input ();
    read_global_data ();
    if (iFlag_run_cascade_model != 1)
    {
      read_cascade_parameter ();
    }

    return error_code;
    }
// 50==================================================
/*!
//read the configuration file
\param, nothing most of time, since the configuration file was already assigned.
\return <ReturnValue>
*/
// 50==================================================
  int eco3d::read_configuration_file ()
    {
    int error_code = 1;
    std::size_t iVector_size;
    std::string sLine;  // used to store each sLine
    std::string sKey;   // used to store the sKey
    std::string sValue; // used to store the sValue
    std::ifstream ifs;  // fstream object to read_eco3d the file
    std::vector<std::string> vTokens;
    // 50==================================================
    // the existence of the configuration file is checked already
    // 50==================================================
    ifs.open (sFilename_configuration.c_str (), ios::in);
    if (ifs.good ())
    {
      std::cout << "===Level 3: Start to read_eco3d the configuration file"
                << std::endl;
      while (!ifs.eof ()) // read_eco3d all the content
      {
        // reset sKey and sValue to null for each sLine
        sKey = "";
        sValue = "";
        // 50==================================================
        // we should read_eco3d one sLine at a time and then break the sLine
        // into substrings.
        // 50==================================================
        std::getline (ifs, sLine); // read_eco3d the sLine
        std::size_t iLength0 = sLine.length ();
        if (iLength0 <= 3)
        {
          continue;
        }
        // split the sLine
        vTokens = split_string_by_space (sLine);
        // test the size of the vector
        iVector_size = vTokens.size ();
        if (iVector_size == 2)
        {
          sKey = vTokens[0];   // sKey holds the sKey(address)!
          sValue = vTokens[1]; // the real sValue of the sKey.
        }
        else
        {
          sKey = vTokens[0]; // sKey holds the sKey(address)!
          sValue = sKey;
        }
        // 50==================================================
        // check the completeness of the sLine
        // 50==================================================
        std::size_t iLength1 = sKey.length ();
        std::size_t iLength2 = sValue.length ();
        if (iLength1 >= 1 && iLength2 >= 1) // both of them are not NULL
        {
          auto search = this->mParameter.find (sKey); // search the
          // dictionary
          if (search !=
              this->mParameter
                  .end ()) // if found, add the sValue to the sKey/address
          {
            search->second = sValue;
          }
          else
          {
            // if not, insert this sKey and sValue pair into user map
            // this map should be used to output control
            mVariable_user.insert (
                std::pair<std::string, std::string> (sKey, sValue));
          }
        }
        else
        {
          // this may be a empty sLine
          continue;
        }
      }
      // close the file after reading
    }
    else
    {
      error_code = 0;
    }
    ifs.close ();

    return error_code;
    }

// 50==================================================
/*!
  <long-description>

  \param
  \return <ReturnValue>
*/
// 50==================================================
  int eco3d::retrieve_user_input ()
    {
    int error_code = 1;
    // 50==================================================
    // assign the class members using the key and value dictionary
    // 50==================================================
    std::string sKey = "workspace_data";
    auto search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_data = search->second;
    }
    sKey = "workspace_scratch";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_scratch = search->second;
      if (sWorkspace_scratch.length () < 5)
      {
        std::string sRcac_scratch = std::getenv ("RCAC_SCRATCH");
        // std::size_t iLength_head = sHead.length();
        // std::size_t iPosition_user = sRcac_scratch.find("/l/liao46");
        // std::size_t iLength_cluster = iPosition_user - iLength_head;
        std::string sCluster = "rice";
        sWorkspace_scratch = sRcac_scratch;// + slash + sCluster;
      }
    }
    sKey = "workspace_out";
    search = mParameter.find (sKey);
    if ( search != mParameter.end () )
    {
      this->sWorkspace_out = sWorkspace_scratch + slash + search->second;
    }
    if( path_test(sWorkspace_out) != 1)
    {
      make_directory(sWorkspace_out);
    }

    sKey = "workspace_dewpoint";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_dewpoint = sWorkspace_scratch + slash + search->second;
    }

    sKey = "workspace_fpar";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_fpar = sWorkspace_scratch + slash + search->second;
    }

    sKey = "workspace_lai";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_lai = sWorkspace_scratch + slash + search->second;
    }

    sKey = "workspace_prec";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_prec = sWorkspace_scratch + slash + search->second;
    }

    sKey = "workspace_tmax";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_tmax = sWorkspace_scratch + slash + search->second;
    }
    sKey = "workspace_tmin";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_tmin = sWorkspace_scratch + slash + search->second;
    }
    sKey = "workspace_land_cover";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sWorkspace_land_cover_type =
          sWorkspace_scratch + slash + search->second;
    }
    //this one has been moved from git to a different location because inflow is too large for git
    //this->sWorkspace_reach_inflow = sWorkspace_data + slash + "inflow";
      sKey = "workspace_reach_inflow";
      search = mParameter.find (sKey);
      if (search != mParameter.end ())
      {
        this->sWorkspace_reach_inflow =
                sWorkspace_scratch + slash + search->second;
      }


    sKey = "aspect";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_aspect = sWorkspace_data + slash + search->second;
    }
    sKey = "boundary";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_boundary = sWorkspace_data + slash + search->second;
    }

    sKey = "column_index";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_column_index = sWorkspace_data + slash + search->second;
    }

    sKey = "dem";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_elevation = sWorkspace_data + slash + search->second;
    }
    sKey = "header";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_header = sWorkspace_data + slash + search->second;
    }
    sKey = "hru_type";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_hru_type = sWorkspace_data + slash + search->second;
    }

    sKey = "latitude";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_latitude = sWorkspace_data + slash + search->second;
    }

    sKey = "initial_vegetation_carbon";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_vegetation_carbon_initial =
          sWorkspace_data + slash + search->second;
      if (file_test (sFilename_vegetation_carbon_initial) == 1)
      {
        iFlag_vegetation_carbon_initial = 1;
      }
      else
      {
        iFlag_vegetation_carbon_initial = 0;
      }
    }
    sKey = "initial_litter_carbon";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_litter_carbon_initial =
          sWorkspace_data + slash + search->second;
      if (file_test (sFilename_litter_carbon_initial) == 1)
      {
        iFlag_litter_carbon_initial = 1;
      }
      else
      {
        iFlag_litter_carbon_initial = 0;
      }
    }
    sKey = "initial_soil_carbon";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_soil_carbon_initial =
          sWorkspace_data + slash + search->second;
      if (file_test (sFilename_soil_carbon_initial) == 1)
      {
        iFlag_soil_carbon_initial = 1;
      }
      else
      {
        iFlag_soil_carbon_initial = 0;
      }
    }
    if (1 == iFlag_litter_carbon_initial &&
        1 == iFlag_vegetation_carbon_initial && 1 == iFlag_soil_carbon_initial)
    {
      iFlag_initial_state = 1;
    }
    else
    {
      iFlag_initial_state = 0;
    }
    sKey = "starlog";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_log = search->second;
    }
    else
    {
      // the default log file
      this->sFilename_log = "star_log.txt";
    }
    this->ofs_log.open (this->sFilename_log.c_str (), ios::out);

    sLog = "=Level 1: Simulation log will be save to log file: " +
           this->sFilename_log;
    ofs_log << sLog << std::endl;

    sKey = "slope";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_slope = sWorkspace_data + slash + search->second;
    }
    sKey = "stream_reach";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_stream_reach = sWorkspace_data + slash + search->second;
    }
    sKey = "stream_segment";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_stream_segment = sWorkspace_data + slash + search->second;
    }
    sKey = "stream_order";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_stream_order = sWorkspace_data + slash + search->second;
    }
    sKey = "stream_segment_topology";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_stream_segment_topology =
          sWorkspace_data + slash + search->second;
    }
    sKey = "soil_type";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->sFilename_soil_type = sWorkspace_data + slash + search->second;
    }

    // define flags

    sKey = "steady_state_flag";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_run_steady_state = std::stoi (search->second);
    }
    sKey = "transient_flag";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_run_transient = std::stoi (search->second);
    }
    sKey = "cascade_flag";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_run_cascade_model = std::stoi (search->second);
    }

    sKey = "run_cascade_flag";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_run_cascade = std::stoi (search->second);
    }

    sKey = "carbon_cascade_flag";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_run_carbon_cascade = std::stoi (search->second);
    }
    sKey = "debug_flag";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_debug = std::stoi (search->second);
    }

    sKey = "debug_flag_land";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_debug_land = std::stoi (search->second);
    }

    sKey = "debug_flag_reach";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_debug_reach = std::stoi (search->second);
    }
    sKey = "debug_flag_cascade";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iFlag_debug_cascade = std::stoi (search->second);
      this->sFilename_debug_table = "debug_table.csv";
    }
    if (iFlag_debug_cascade == 1)
    {
      iFlag_run_cascade = 1;
    }

    sKey = "debug_cu_index";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->lCU_index_debug = std::stol (search->second);
    }
    // 50==================================================
    // time variables of model simulation
    // 50==================================================

    sKey = "save_start_year";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iYear_start_save = std::stoi (search->second);
    }
    sKey = "save_end_year";
    search = this->mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iYear_end_save = std::stoi (search->second);
    }

    sKey = "save_start_month";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iMonth_start_save = std::stoi (search->second);
    }
    sKey = "save_end_month";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iMonth_end_save = std::stoi (search->second);
    }
    sKey = "save_start_day";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iDay_start_save = std::stoi (search->second);
    }
    sKey = "save_end_day";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iDay_end_save = std::stoi (search->second);
    }
    sKey = "data_year_start";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iYear_available_start = std::stoi (search->second);
    }
    sKey = "data_year_end";
    search = mParameter.find (sKey);
    if (search != mParameter.end ())
    {
      this->iYear_available_end = std::stoi (search->second);
    }
    // check the time definition
    if (iYear_available_start > iYear_available_end)
    {
      // error
      std::cout << "Error in configuration" << std::endl;
      return -1;
    }

    if (1 == iFlag_initial_state)
    {
      // if all initial state are provided, do not run steady state
      iFlag_run_steady_state = 0;
    }
    else
    {
      iFlag_run_steady_state = 1;
    }

    if (iYear_end_save < iYear_start_save)
    {
      iYear_end_save = iYear_start_save;
    }

    sFilename_budget_table = "budget_table.csv";
    iMonth_start_ss = 1;
    iDay_start_ss = 1;
    iMonth_end_ss = 12;
    iDay_end_ss = 31;

    iMonth_start_tr = 1;
    iDay_start_tr = 1;
    iMonth_end_tr = 12;
    iDay_end_tr = 31;
    if (1 == iFlag_run_steady_state)
    {

      // this is the same with default setting
      if (iFlag_debug != 1)
      {
        // the most expensive simulation
        iYear_start_ss = 1700;
        iYear_end_ss = 1840;
        // tr
        iYear_start_tr = iYear_end_ss + 1;
        iYear_end_tr = iYear_end_save;
      }
      else
      {

        // for debug purpose
        if (iYear_available_start < iYear_start_save)
        {
          iYear_start_ss = iYear_available_start - 50;
          iYear_end_ss = iYear_available_start - 10 - 1;
        }
        else
        {
          iYear_start_ss = iYear_start_save - 50;
          iYear_end_ss = iYear_start_save - 10 - 1;
        }
      }
    }
    else
    {
      // if initial data is available, we do not run ss
      if (iYear_available_start < iYear_start_save)
      {
        iYear_start_tr = iYear_available_start - 1;
      }
      else
      {
        iYear_start_tr = iYear_start_save - 1;
      }
      iYear_end_tr = iYear_end_save;
    }
    // get the start julian day
    this->lJulianDay_available_start = date::YmdToJd (iYear_available_start, 1, 1);

    // 50==================================================
    // stream segment topology file
    // 50==================================================


    if (iFlag_run_cascade == 1)
    {
      iFlag_run_water_cascade = 1;
      if (iFlag_run_carbon_cascade == 1)
      {
        sWorkspace_out_lateral_carbon = sWorkspace_out + slash + "lateral_carbon";
        sWorkspace_out = sWorkspace_out_lateral_carbon;
      }
      else
      {
        sWorkspace_out_no_lateral_carbon = sWorkspace_out + slash + "no_lateral_carbon";
        sWorkspace_out = sWorkspace_out_no_lateral_carbon;
      }
    }
    else
    {
      iFlag_run_water_cascade = 0;
      iFlag_run_carbon_cascade = 0;
      sWorkspace_out_no_lateral_water = sWorkspace_out + slash + "no_lateral_water";
      sWorkspace_out = sWorkspace_out_no_lateral_water;

    }


    // workspace output
    if (path_test (sWorkspace_out) != 1)
    {
      sLog = "===Level 3: Workspace does not exists, it will be created!";
      make_directory (sWorkspace_out);
      this->ofs_log << sLog << std::endl;
    }
    else
    {
      sLog = "===Level 3: Workspace already exists!";
      this->ofs_log << sLog << std::endl;
    }

    sLog = "==Level 2: Finished reading the configuration file";
    ofs_log << sLog << std::endl;
    this->ofs_log.flush ();
    std::cout << sLog << std::endl;

    return error_code;
    }

// 50==================================================
/*!
//After the configuration is read, the first step is to read the global data.
//Global data are time-invariant data, such as dem.
\param
\return <ReturnValue>
*/
// 50==================================================
  int eco3d::read_global_data ()
    {
    int error_code = 1;
    std::string sError_file_missing = "The following file does not exist: ";
    std::string sFilename_dummy;
    // 50==================================================
    // start to write program info into the log file
    // 50==================================================

    sLog = "===Level 3: Start to read global data:";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;

    // check the existence of global data
    sFilename_dummy = sFilename_aspect;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();

      return 0;
    }
    sFilename_dummy = sFilename_boundary;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }
    sFilename_dummy = sFilename_column_index;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }
    sFilename_dummy = sFilename_elevation;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }
    sFilename_dummy = sFilename_hru_type;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }
    sFilename_dummy = sFilename_latitude;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }
    sFilename_dummy = sFilename_slope;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }
    sFilename_dummy = sFilename_soil_type;
    if (file_test (sFilename_dummy) != 1)
    {
      sLog = "===Level 4: " + sError_file_missing + " : " + sFilename_dummy;
      ofs_log << sLog << std::endl;
      ofs_log.flush ();
      return 0;
    }

    sLog = "===Level 3: Global data are ready for read";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;
    // 50==================================================
    // read_eco3d all the global file here, all the global files are
    // raster file, and they all have the same format (envi standard data)
    // 50==================================================
    vAspect = data::read_binary_vector (sFilename_aspect);
    vBoundary = data::read_binary_vector (sFilename_boundary);
    vColumn_index = data::read_binary_vector (sFilename_column_index);
    vElevation = data::read_binary_vector (sFilename_elevation);
    vHru_type = data::read_binary_vector (sFilename_hru_type);
    vLatitude = data::read_binary_vector (sFilename_latitude);
    vSlope = data::read_binary_vector (sFilename_slope);
    vSoil_type = data::read_binary_vector (sFilename_soil_type);
    vStream_reach = data::read_binary_vector (sFilename_stream_reach);
    vStream_segment = data::read_binary_vector (sFilename_stream_segment);
    vStream_order = data::read_binary_vector (sFilename_stream_order);

    // read_eco3d the stream topology file
    read_stream_segment_topology ();

    // 50==================================================
    // check data quality here because all matrix must have the same dimension
    // 50==================================================

    sLog = "===Level 3: Successfully read global data";
    ofs_log << sLog << std::endl;
    ofs_log.flush ();
    std::cout << sLog << std::endl;

    return error_code;
    }

  } // namespace ecohydrology
