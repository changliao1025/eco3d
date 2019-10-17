// 50==================================================
/*!
  \file   ecosystem_cascade.cpp
  \brief
  <long description>
  \author Chang Liao
  \date   2017-07-05
*/
// 50==================================================
#include "eco3d.h"

namespace ecohydrology
{

int eco3d::setup_cascade_parameter()
{
  int error_code = 1;
  int iBoundary_from, iBoundary_to;
  int iDirection;
  long lRow_from, lRow_to;
  long lColumn_from, lColumn_to;
  long lIndex_from, lIndex_to;
  for (lRow_from = 0; lRow_from < nrow; lRow_from++)
    {

      for (lColumn_from = 0; lColumn_from < ncolumn; lColumn_from++)
        {
          iBoundary_from = (int)vBoundary[lRow_from * ncolumn + lColumn_from];
          lIndex_from =
              calculate_index_from_coordinates(lRow_from, lColumn_from);
          if (iBoundary_from != missing_value && lIndex_from >= 0)
            {
              for (iDirection = 0; iDirection < 8; iDirection++)
                {
                  switch (iDirection)
                    {
                    case 0:
                      lRow_to = lRow_from - 1;   // row - 1
                      lColumn_to = lColumn_from; // col unchanged
                      break;
                    case 1:
                      lRow_to = lRow_from - 1;
                      lColumn_to = lColumn_from + 1;
                      break;
                    case 2:
                      lRow_to = lRow_from;
                      lColumn_to = lColumn_from + 1;
                      break;
                    case 3:
                      lRow_to = lRow_from + 1;
                      lColumn_to = lColumn_from + 1;
                      break;
                    case 4:
                      lRow_to = lRow_from + 1;
                      lColumn_to = lColumn_from;
                      break;
                    case 5:
                      lRow_to = lRow_from + 1;
                      lColumn_to = lColumn_from - 1;
                      break;
                    case 6:
                      lRow_to = lRow_from;
                      lColumn_to = lColumn_from - 1;
                      break;
                    case 7:
                      lRow_to = lRow_from - 1;
                      lColumn_to = lColumn_from - 1;
                      break;
                    default:
                      lRow_to = -1;
                      lColumn_to = -1;
                      break;
                    } // end switch
                  // 50==================================================
                  // both index must be greater or equal than 0
                  // or else, continue to the next neighbor
                  // 50==================================================
                  if (lRow_to < 0 || lRow_to >= nrow || lColumn_to < 0 ||
                      lColumn_to >= ncolumn)
                    {
                      continue;
                    }
                  // 50==================================================
                  // get the new boundary and index
                  // 50==================================================
                  iBoundary_to = (int)vBoundary[lRow_to * ncolumn + lColumn_to];
                  // 50==================================================
                  // check whether the neighbor is still within boundary.
                  // 50==================================================
                  if (iBoundary_to != missing_value)
                    {
                      lIndex_to =
                          calculate_index_from_coordinates(lRow_to, lColumn_to);
                      (vColumn.at(lIndex_from)).aNeighbor_index.at(iDirection) =
                          lIndex_to;
                      (vColumn.at(lIndex_from))
                          .aNeighbor_cascade_percentage.at(iDirection) =
                          (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                           lColumn_from];
                    }
                  else
                    {
                      continue;
                    }
                }
            }
        }
    }

  return error_code;
}
// 50==================================================
/*!
  After all the progresses are simulated, then we will use the cascade algorithm
  to connect the columns with neighbors
  \param i the row index
  \param j the column index
*/
// 50==================================================
int eco3d::all_cascade(long lRow_in, long lColumn_in)
{
  int error_code = 1;
  // 50==================================================
  int iBoundary_to; // boundary type of grids
  // int iHru_type_from, iHru_type_to; //hru type of grids
  int iDirection;               // directions of cascade
  long lRow_from, lColumn_from; // location of center
  long lRow_to, lColumn_to;     // the corrdinate indices of the flow-to grid
  long lIndex_from, lIndex_to;  // the real index in the active "matrix"

  eColumn_type eCU_type_from, eCU_type_to;
  // assign the location indices
  lRow_from = lRow_in;
  lColumn_from = lColumn_in;
  // 50==================================================
  // in theory, water flows to up 8 directions, however, a threshold is used to
  // remove near- zero fraction
  // 50==================================================
  // get the index and hru type of the center column
  lIndex_from = calculate_index_from_coordinates(lRow_from, lColumn_from);
  // iHru_type_from = (int)vHru_type[lRow_from * ncolumn + lColumn_from];
  eCU_type_from = vColumn[lIndex_from].eCU_type;

  // the cascade is simulated based on the hru type
  switch (eCU_type_from)
    {
    case eCU_glacier: // glacier
      {
        for (iDirection = 0; iDirection < 8; iDirection++)
          {
            lIndex_to =
                ((vColumn.at(lIndex_from)).aNeighbor_index).at(iDirection);
            if (lIndex_to >= 0)
              {
                eCU_type_to = (vColumn.at(lIndex_to)).eCU_type;
                switch (eCU_type_to)
                  {
                  case eCU_glacier: // from glacier to glacier
                    {
                      // in glacier region, we only consider the surface
                      // hydrology process
                      if(iFlag_run_water_cascade==1)
                      {
                        vColumn[lIndex_to]
                                .cGlacier.cSurface_runoff
                                .dSurface_runoff_inflow_upslope +=
                                vColumn[lIndex_from]
                                        .cGlacier.cSurface_runoff
                                        .dSurface_runoff_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];
                      }

                    }
                    break;
                  case eCU_lake: // from glacier to lake
                    {
                    }
                    break;
                  case eCU_land: // from glacier to land
                    {
                      // hortonian runoff
                      if(iFlag_run_water_cascade==1)
                      {
                        vColumn[lIndex_to]
                                .cLand.cSurface_runoff
                                .dSurface_runoff_inflow_upslope +=
                                vColumn[lIndex_from]
                                        .cGlacier.cSurface_runoff
                                        .dSurface_runoff_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];
                      }
                    }
                    break;
                  case eCU_stream: // from glacier to stream
                    {
                      // when water and carbon flow from glacier to stream, the
                      // units need to be converted
                      // 50==================================================
                      // hydrology
                      // 50==================================================
                      // hortonian runoff
                      if(iFlag_run_water_cascade==1)
                      {
                        vColumn[lIndex_to]
                                .cReach.dReach_discharge_inflow_lateral_daily +=
                                vColumn[lIndex_from]
                                        .cGlacier.cSurface_runoff
                                        .dSurface_runoff_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from] *
                                        dArea; // unit cubic meter per day;
                      }
                    }
                    break;
                  case eCU_swale: // from glacier to swale
                    {
                    }
                    break;
                  default: // inactive hru
                    // nothing will be changed
                    break;
                  }
              }

            else // it is out of bound
              {
                // nothing here
                continue;
              }
          } // end direction
      }
      break;
    case eCU_lake: // from lake
      {
      }
      break;
    case eCU_land: // from land
      {
        // 50==================================================
        // in this model, direction is indexed using ArcGIS aspect scheme,
        // for details of the setup_eco3d, please take a look at my blog at
        // www.changliao.us and search for "cascade"
        // 50==================================================
        // the starting neighbor is the point which pointing north and continues
        // clockwise
        for (iDirection = 0; iDirection < 8; iDirection++)
          {
            lIndex_to =  ((vColumn.at(lIndex_from)).aNeighbor_index).at(iDirection);
            if (lIndex_to >= 0)
              {
                eCU_type_to = vColumn[lIndex_to].eCU_type;
                switch (eCU_type_to)
                  {
                  case eCU_glacier: // land to glacier  //from land to land
                    {
                      // in glacier region, we only consider the surface
                      // hydrology process
                      if(iFlag_run_water_cascade==1)
                      {
                        vColumn[lIndex_to]
                                .cGlacier.cSurface_runoff
                                .dSurface_runoff_inflow_upslope +=
                                vColumn[lIndex_from]
                                        .cLand.cSurface_runoff
                                        .dSurface_runoff_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];
                      }
                      // there is no soil under glacier
                    }
                    break;
                  case eCU_lake: // from land to lake
                    {
                    }
                    break;
                  case eCU_land: // from land to land
                    {
                      if(iFlag_run_water_cascade==1)
                      {
                        // runoff
                        vColumn[lIndex_to]
                                .cLand.cSurface_runoff
                                .dSurface_runoff_inflow_upslope +=
                                vColumn[lIndex_from]
                                        .cLand.cSurface_runoff
                                        .dSurface_runoff_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];

                        // soil interflow
                        vColumn[lIndex_to]
                                .cLand.cSoil.dSoil_interflow_fast_upslope +=
                                vColumn[lIndex_from]
                                        .cLand.cSoil.dSoil_interflow_fast_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];

                        vColumn[lIndex_to]
                                .cLand.cSoil.dSoil_interflow_slow_upslope +=
                                vColumn[lIndex_from]
                                        .cLand.cSoil.dSoil_interflow_slow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];

                        // groundwater flow
                        vColumn[lIndex_to]
                                .cLand.cGroundwater.dGroundwater_inflow_upslope +=
                                vColumn[lIndex_from]
                                        .cLand.cGroundwater.dGroundwater_outflow *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from];

                        if (iFlag_run_carbon_cascade == 1)
                        {
                          // doc
                          vColumn[lIndex_to]
                                  .cLand.cLitter.dLitter_doc_inflow_upslope +=
                                  vColumn[lIndex_from]
                                          .cLand.cLitter.dLitter_doc_outflow_downslope *
                                          (vCascade_parameter[iDirection])[lRow_from *
                                                  ncolumn +
                                                  lColumn_from];

                          vColumn[lIndex_to]
                                  .cLand.cSoil.dSoil_doc_inflow_upslope +=
                                  vColumn[lIndex_from]
                                          .cLand.cSoil.dSoil_doc_outflow_downslope *
                                          (vCascade_parameter[iDirection])[lRow_from *
                                                  ncolumn +
                                                  lColumn_from];

                          //part three soil runoff
                          vColumn[lIndex_to]
                                  .cLand.cSoil.dSoil_doc_inflow_upslope +=
                                  vColumn[lIndex_from]
                                          .cLand.cSoil.dSoil_doc_outflow_dunnian_runoff *
                                          (vCascade_parameter[iDirection])[lRow_from *
                                                  ncolumn +
                                                  lColumn_from];
                        }
                      }
                      // we assume litter does not cascade in common scenarios
                    }
                    break;
                  case eCU_stream: // from land to stream
                    {
                      if(iFlag_run_water_cascade==1)
                      {
                        // when water and carbon flow from land to stream, the
                        // units need to be converted
                        // 50==================================================
                        // hydrology
                        // 50==================================================
                        // runoff
                        vColumn[lIndex_to]
                                .cReach.dReach_discharge_inflow_lateral_daily +=
                                vColumn[lIndex_from]
                                        .cLand.cSurface_runoff
                                        .dSurface_runoff_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from] *
                                        dArea; // unit cubic meter;

                        // soil interflow
                        vColumn[lIndex_to]
                                .cReach.dReach_discharge_inflow_lateral_daily +=
                                vColumn[lIndex_from]
                                        .cLand.cSoil.dSoil_water_outflow_downslope *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from] *
                                        dArea; // unit cubic meter

                        // groundwater flow
                        vColumn[lIndex_to]
                                .cReach.dReach_discharge_inflow_lateral_daily +=
                                vColumn[lIndex_from]
                                        .cLand.cGroundwater.dGroundwater_outflow *
                                        (vCascade_parameter[iDirection])[lRow_from * ncolumn +
                                                lColumn_from] *
                                        dArea; // unit cubic meter


                        if (iFlag_debug_reach == 1)
                        {
                          if (lIndex_to == lCU_index_debug)
                          {
                            /*  std::cout<<vColumn[lCU_index_debug].cReach.dReach_discharge_inflow_lateral_daily
                                       <<std::endl
                                      <<vColumn[lCU_index_debug].cReach.dReach_doc_inflow_lateral_daily
                                      <<std::endl
                                      ;
                                      */
                          }
                        }
                        // 50==================================================
                        // doc, stream only accept mass, so convert from kg/m2 to
                        // kg using area
                        // 50==================================================
                        if (iFlag_run_carbon_cascade == 1)
                        {
                          // part one, from litter
                          vColumn[lIndex_to]
                                  .cReach.dReach_doc_inflow_lateral_litter_daily +=
                                  vColumn[lIndex_from]
                                          .cLand.cLitter.dLitter_doc_outflow_downslope *
                                          (vCascade_parameter[iDirection])[lRow_from *
                                                  ncolumn +
                                                  lColumn_from] *
                                          dArea; // unit kg per day
                          // part two, from soil
                          vColumn[lIndex_to]
                                  .cReach.dReach_doc_inflow_lateral_soil_daily +=
                                  vColumn[lIndex_from]
                                          .cLand.cSoil.dSoil_doc_outflow_downslope *
                                          (vCascade_parameter[iDirection])[lRow_from *
                                                  ncolumn +
                                                  lColumn_from]
                                                  * dArea; // unit, kg per day
                          //part three soil runoff
                          vColumn[lIndex_to]
                                  .cReach.dReach_doc_inflow_lateral_soil_daily +=
                                  vColumn[lIndex_from]
                                          .cLand.cSoil.dSoil_doc_outflow_dunnian_runoff *
                                          (vCascade_parameter[iDirection])[lRow_from *
                                                  ncolumn +
                                                  lColumn_from]
                                                  * dArea; // unit, kg per day

                        }
                      }
                    }
                    break;
                  case eCU_swale: // from land to swale,
                    {
                    }
                    break;
                  default: // inactive hru
                    // nothing will be changed
                    break;
                  }
              }
            else
              {
                continue;
              }

          } // end direction
      }
      break;
    case eCU_stream: // from stream
      {
        // stream to stream is routed with stream function
      }
      break;
    case eCU_swale: // from swale
      {
      }
      break;
    default:
      {
        // nothing here yet
      }
      break;
    }
  return error_code;
}

int eco3d::read_cascade_parameter()
{
  int error_code = 1;
  int iDirection;
  std::string sDirection;
  std::string sFilename_cascade;
  std::vector<double> vDummy;
  std::stringstream ss;
  vCascade_parameter.clear();
  // read_eco3d from existing prepared files

  sLog = "===Level 3: Start to read cascade parameter files ";
  ofs_log << sLog << std::endl;
  ofs_log.flush();
  std::cout << sLog << std::endl;

  for (iDirection = 0; iDirection < 8; iDirection++)
    {
      // get the index
      sDirection = convert_integer_to_string(int(pow(2.0, iDirection)), 3);
      // get the filename
      ss.str("");
      ss << sWorkspace_data << slash << "cascade_para_" << sDirection
         << sExtension_envi;
      sFilename_cascade = ss.str();
      // read_eco3d the data
      vDummy = data::read_binary_vector(sFilename_cascade);
      vCascade_parameter.push_back(vDummy);
    }
 sLog = "===Level 3: Finished reading cascade parameter files ";
  ofs_log << sLog << std::endl;
  ofs_log.flush();
  std::cout << sLog << std::endl;
  return error_code;
}

// 50==================================================
/*!
//surface cascade simulation
\param
\return <ReturnValue>
*/
// 50==================================================
int eco3d::call_cascade_model()
{
  int error_code = 1;

  this->ofs_log << "Start to run " << sModule_cascade << std::endl;
  error_code = cCascade.check_dem_depression(vBoundary, vElevation);
  if (error_code == 0) // there is no depression
    {
      vCascade_parameter = cCascade.run_cascade_model(vBoundary, vElevation);
      cCascade.save_cascade_parameter(sWorkspace_data, vCascade_parameter);
      this->ofs_log << "Finished " << sModule_cascade << std::endl;
      setup_cascade_parameter ();
    }
  else
    {
      this->ofs_log
          << "Please check the DEM datasets since there are depressions!"
          << std::endl;
    }

  return error_code;
}
} // namespace ecohydrology
