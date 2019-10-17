// 50==================================================
/*!
  \file   ecosystem_save.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-05
*/
// 50==================================================

#include "eco3d.h"

namespace ecohydrology
{
// 50==================================================
    //this function is used to save some time invariant parameters
    // 50==================================================
    int eco3d::save_system_parameter()
    {
      int error_code = 1;
      long lRow, lColumn, lIndex;
      std::string sValue;
      std::string sPath_out;
      std::string sFilename_out;
      std::string sHeader_out;
      std::vector<double> vData_out;
      std::stringstream ss;
      std::ifstream src;
      std::ofstream dst;
      sValue = "reach_time_step";
      for (lRow = 0; lRow < nrow; lRow++)
      {
        for (lColumn = 0; lColumn < ncolumn; lColumn++)
        {
          lIndex =
                  calculate_index_from_coordinates(lRow, lColumn);
          if (lIndex >= 0)
          {
            // put pixel value into the matrix
            vData_out.push_back(
                    vColumn[lIndex]
                            .cReach.iTime_step);
            //
          }
          else
          {
            // for inactive grid, set missing value
            vData_out.push_back(missing_value);
          }
        }
      }

      ss.str("");
      ss << sWorkspace_out << slash ;

      sPath_out = ss.str();

      if (path_test(sPath_out) == 0)
      {
        make_directory(sPath_out);
      }
      ss.str("");
      ss << sPath_out << slash << sValue
         << sExtension_envi;

      sFilename_out = ss.str();

      error_code = data::write_binary_vector(sFilename_out, vData_out);
      // copy the header file
      if (error_code == 1)
      {
        ss.str("");
        ss << sPath_out << slash << sValue
           << sExtension_header;
        sHeader_out = ss.str();
        src.open(this->sFilename_header.c_str());
        dst.open(sHeader_out.c_str());
        dst << src.rdbuf();
        src.close();
        dst.close();
        // this->ofs_log << "Successfully saved : " << iYear << " " <<
        // iDay << " " << iIndex << endl;
      }
      else
      {
        // writing data out failed, usually it is because of bad
        // data
      }


      return error_code;
    }

// 50==================================================
/*!
//save the out variable specified in the configuration file
\param iDay
\param iYear
\return <ReturnValue>
*/
// 50==================================================
    int eco3d::save_system_status(int iDay, int iYear)
    {
      int error_code = 1;
      int iFlag_output = 0;
      long lRow, lColumn, lIndex;
      std::string sVariable_name;
      std::string sYear;
      std::string sDay;
      std::string sPath_out;
      std::string sFilename_out;
      std::string sHeader_out;
      std::string sKey, sValue;
      std::ifstream src;
      std::ofstream dst;
      std::vector<double> vData_out;
      std::stringstream ss;
      eVariable ev;
      std::map<std::string, std::string>::iterator
              iIterator; // the iterator to search the key
      // convert year day to string
      sYear = convert_integer_to_string(iYear, 4);
      sDay = convert_integer_to_string(iDay, 3);
      try
      {
        for (iIterator = mVariable_user.begin();
             iIterator != mVariable_user.end(); iIterator++)
        {
          vData_out.clear();
          sKey = (*iIterator).first;
          sValue = (*iIterator).second;
          auto search = mVariable_system.find(sKey);
          if (search != mVariable_system.end())
          {
            iFlag_output = 1;
            ev = search->second;

            switch (ev)
            {
              case eV_max_temperature:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cTemperature
                                      .dTemperature_max);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_min_temperature:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cTemperature
                                      .dTemperature_min);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_mean_temperature:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cTemperature
                                      .dTemperature_mean);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_dewpoint_temperature:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cTemperature
                                      .dTemperature_dewpoint);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_temperature_cumulative:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cTemperature
                                      .dTemperature_cumulative);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_vapor_pressure_deficit:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cTemperature
                                      .dVapor_pressure_deficit);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_precipitation_type:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cPrecipitation
                                      .iPrecipitation_type);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_rain:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cPrecipitation.dRain);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_snow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      // put pixel value into the matrix
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cPrecipitation.dSnow);
                      //
                    }
                    else
                    {
                      // for inactive grid, set missing value
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_photosynthetically_active_radiation:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cRadiation.dPar);
                      //
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_shortwave_radiation:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cAtmosphere.cRadiation
                                      .dShortwave_radiation_actual);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_potential_evapotranspiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cEvapotranspiration.dPet_hru);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }

                // std::cout<<*std::max_element(vData_out.begin(),vData_out.end())<<std::endl;
              }
                break;
              case eV_canopy_precipitation_type:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .iPrecipitation_type);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_canopy_water_depth:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .dCanopy_depth);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_canopy_water_storage:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .dCanopy_storage);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_canopy_evapotranspiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .dEt_canopy);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_canopy_rain:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .dRain_net);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_canopy_snow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .dSnow_net);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_canopy_net_precipitation:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cInterception
                                      .dPrecipitation_net);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_gross_primary_production:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy
                                      .cPhotosynthesis.dPhotosynthesis_gpp);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_litterfall_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy.cLitterfall
                                      .dLitterfall_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
                // case eV_litterfall_nitrogen:
                //{
                //	for (lRow = 0; lRow < nrow; lRow++)
                //	{
                //		for (lColumn = 0; lColumn < ncolumn; lColumn++)
                //		{
                //			lIndex =
                // calculate_index_from_coordinates(lRow,  lColumn);
                // if (lIndex >= 0)
                //			{
                //				vData_out.push_back(vColumn[lIndex].cLand.cVegetation.cCanopy.cLitterfall.dLitterfall_nitrogen);
                //			}
                //			else
                //			{
                //				vData_out.push_back(missing_value);
                //			}
                //		}
                //	}
                //}
                // break;

              case eV_canopy_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cCanopy
                                      .dCanopy_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
                // case eV_canopy_nitrogen:
                //{
                //	for (lRow = 0; lRow < nrow; lRow++)
                //	{
                //		for (lColumn = 0; lColumn < ncolumn; lColumn++)
                //		{
                //			lIndex =
                // calculate_index_from_coordinates(lRow,  lColumn);
                // if (lIndex >= 0)
                //			{
                //				vData_out.push_back(vColumn[lIndex].cLand.cVegetation.cCanopy.dCanopy_nitrogen);
                //			}
                //			else
                //			{
                //				vData_out.push_back(missing_value);
                //			}
                //		}
                //	}
                //}
                // break;
              case eV_stem_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cStem.dStem_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_root_carbon_decomposition:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cRoot
                                      .dRoot_carbon_decomposed);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_root_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cRoot.dRoot_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_vegetation_maintenance_respiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cRespiration
                                      .dMaintenance_respiration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_vegetation_growth_respiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cRespiration
                                      .dGrowth_respiration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_vegetation_autotrophic_respiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.cRespiration
                                      .dAutotrophic_respiration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_vegetation_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.dVegetation_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_snow_cover_area:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSnow.dSca);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_snow_albedo:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSnow.dAlbedo);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
                // litter
              case eV_snow_evapotranspiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSnow.dSnow_et);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_snow_melt:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSnow.dSnowmelt);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_snow_water_equivalent:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSnow.dSwe);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_snow_temperature:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSnow.dSnow_temperature);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_snow_heat_deficit:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSnow.dSnow_heat_deficit);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_glacier_water_equivalent:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cGlacier.dGwe);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_glacier_melt:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cGlacier.dGlacier_melt);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_infiltration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff.cInfiltration
                                      .dInfiltration_hru);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_runoff_excess:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff.cInfiltration
                                      .dSurface_runoff_excess);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_runoff_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff
                                      .dSurface_runoff_inflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_runoff_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff
                                      .dSurface_runoff_outflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_surface_runoff_inflow_upslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff
                                      .dSurface_runoff_inflow_upslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_runoff_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff
                                      .dSurface_runoff_inflow_external);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_runoff_outflow_downslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff
                                      .dSurface_runoff_outflow_downslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_surface_runoff_outflow_infiltration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSurface_runoff
                                      .dSurface_runoff_outflow_infiltration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter.dLitter_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_carbon_influx:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter.dLitter_carbon_influx);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_carbon_outflux_humus:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_carbon_outflux_humus);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_undecomposed_litter_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_carbon_undecomposed);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_decomposed_litter_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_carbon_decomposed);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_decomposed_litter_ioc:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_carbon_decomposed_storage_ioc);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_decomposed_litter_soc:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_carbon_decomposed_storage_soc);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_heterotrophic_respiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_heterotrophic_respiration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_dissolved_organic_carbon_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter.dLitter_doc_inflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_dissolved_organic_carbon_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter.dLitter_doc_outflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_litter_dissolved_organic_carbon_inflow_upslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_doc_inflow_upslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_dissolved_organic_carbon_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_doc_inflow_external);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_dissolved_organic_carbon_outflow_downslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_doc_outflow_downslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_litter_dissolved_organic_carbon_outflow_leaching:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cLitter
                                      .dLitter_doc_outflow_leaching);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_dunnian_runoff:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_water_outflow_dunnian_runoff);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_interflow_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_water_interflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_interflow_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_water_interflow_outflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_interflow_inflow_upslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_interflow_inflow_upslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_interflow_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_water_interflow_inflow_external);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_interflow_outflow_downslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_water_outflow_downslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_evapotranspiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSoil.dET_soil);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_moisture:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_water_storage_moisture);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_moisture_volumetric:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_moisture_volume);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_inflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_outflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_dissolved_organic_carbon_storage:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_storage);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_concentration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_concentration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_production:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_production);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_mineralization:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_mineralization);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_adsorption:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_adsorption);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_desorption:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_desorption);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_inflow_upslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_inflow_upslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_doc_inflow_external);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_inflow_groundwater:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_doc_inflow_groundwater);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_outflow_downslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_doc_outflow_downslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_dissolved_organic_carbon_outflow_groundwater:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_doc_outflow_groundwater);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_spsoc_mineralization:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_spsoc_mineralization);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_spsoc_adsorption:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_spsoc_adsorption);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_soil_spsoc_desorption:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil.dSoil_spsoc_desorption);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(vColumn[lIndex]
                                                  .cLand.cGroundwater
                                                  .dGroundwater_inflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(vColumn[lIndex]
                                                  .cLand.cGroundwater
                                                  .dGroundwater_outflow);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_storage:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(vColumn[lIndex]
                                                  .cLand.cGroundwater
                                                  .dGroundwater_storage);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_groundwater_inflow_soil:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cGroundwater
                                      .dGroundwater_inflow_soil);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_inflow_upslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cGroundwater
                                      .dGroundwater_inflow_upslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cGroundwater
                                      .dGroundwater_inflow_external);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_outflow_downslope:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cGroundwater
                                      .dGroundwater_outflow_downslope);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_groundwater_outflow_soil:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cGroundwater
                                      .dGroundwater_outflow_soil);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_heterotrophic_respiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cSoil
                                      .dSoil_heterotrophic_respiration);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_discharge_inflow_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_discharge_outflow_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_storage:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_discharge_storage_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_inflow_upstream:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_inflow_upstream_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_inflow_lateral:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_inflow_lateral_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_inflow_external_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_inflow_groundwater:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_inflow_groundwater_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_inflow_precipitation:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_inflow_precipitation_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_outflow_downstream:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_outflow_downstream_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_outflow_groundwater:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_outflow_groundwater_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_discharge_outflow_evapotranspiration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_outflow_evapotranspiration_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_inflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_doc_inflow_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_outflow:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_doc_outflow_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_storage:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_doc_storage_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_concentration:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach.dReach_doc_concentration_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_inflow_upstream:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_doc_inflow_upstream_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_inflow_lateral:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_doc_inflow_lateral_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_inflow_external:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_discharge_inflow_external_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_reach_dissolved_organic_carbon_inflow_groundwater:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_doc_inflow_groundwater_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_outflow_downstream:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_doc_outflow_downstream_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_reach_dissolved_organic_carbon_outflow_groundwater:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cReach
                                      .dReach_doc_outflow_groundwater_daily);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_soil_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cLand.cSoil.dSoil_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case ev_stream_carbon:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].cReach.dReach_carbon);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              case eV_net_primary_production:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex]
                                      .cLand.cVegetation.dVegetation_npp);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;
              case eV_net_ecosystem_production:
              {
                for (lRow = 0; lRow < nrow; lRow++)
                {
                  for (lColumn = 0; lColumn < ncolumn; lColumn++)
                  {
                    lIndex =
                            calculate_index_from_coordinates(lRow, lColumn);
                    if (lIndex >= 0)
                    {
                      vData_out.push_back(
                              vColumn[lIndex].dColumn_nep);
                    }
                    else
                    {
                      vData_out.push_back(missing_value);
                    }
                  }
                }
              }
                break;

              default:
                // not in the map dictionary
                iFlag_output = 0;
                break;
            }

            // we only output when it is in the dictionary
            ss.str("");
            ss << sWorkspace_out << slash << sValue ;

            sPath_out = ss.str();

            if (path_test(sPath_out) == 0)
            {
              make_directory(sPath_out);
            }

            ss.str("");
            ss << sWorkspace_out << slash << sValue << slash << sYear;

            sPath_out = ss.str();

            if (path_test(sPath_out) == 0)
            {
              make_directory(sPath_out);
            }
            ss.str("");
            ss << sPath_out << slash << sValue << sYear << sDay
               << sExtension_envi;

            sFilename_out = ss.str();

            error_code = data::write_binary_vector(sFilename_out, vData_out);
            // copy the header file
            if (error_code == 1)
            {
              ss.str("");
              ss << sPath_out << slash << sValue << sYear << sDay
                 << sExtension_header;
              sHeader_out = ss.str();
              src.open(this->sFilename_header.c_str());
              dst.open(sHeader_out.c_str());
              dst << src.rdbuf();
              src.close();
              dst.close();
              // this->ofs_log << "Successfully saved : " << iYear << " " <<
              // iDay << " " << iIndex << endl;
            }
            else
            {
              // writing data out failed, usually it is because of bad
              // data
            }
          }
          else
          {
            iFlag_output = 0;
          }
        }
      }
      catch (const std::exception &e)
      {
      }

      return error_code;
    }


} // namespace ecohydrology