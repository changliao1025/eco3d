// 50==================================================
/*!
  \file   cascade.cpp
  \brief
  <long description>
  \author Chang Liao Sr.
  \date   2017-12-28
*/
// 50==================================================
#include "cascade.h"

namespace ecohydrology
{
cascade::cascade() {}

cascade::~cascade() {}

std::vector<std::vector<double>>
cascade::run_cascade_model(std::vector<double> vBoundary_in,
                           std::vector<double> vElevation_in)
{
  int iBoundary;

  int iDirection;
  int iPosition;
  long lRow, lColumn;
  double dThreshold = 0.01;
  double dDistance;
  double dPercent;
  double dElevation;
  std::ptrdiff_t iPosition1, iPosition2;
  std::array<double, 9> aElevations = {};
  std::array<double, 9> aElevations_sort = {};
  std::array<double, 9>::iterator iIterator1, iIterator2;
  std::vector<std::vector<double>> vCascade_parameter_out;
  // allocate the memory
  for (iDirection = 0; iDirection < 8; iDirection++)
    {
      std::vector<double> vDummy;
      for (lRow = 0; lRow < nrow; lRow++)
        {
          for (lColumn = 0; lColumn < ncolumn; lColumn++)
            {
              vDummy.push_back(-9999.0); // missing value
            }
        }
      vCascade_parameter_out.push_back(vDummy);
    }
  for (lRow = 0; lRow < nrow; lRow++)
    {
      for (lColumn = 0; lColumn < ncolumn; lColumn++)
        {
          iBoundary = (int)vBoundary_in[lRow * ncolumn + lColumn];
          if (iBoundary == -9999)
            {
              // outside the boundary
            }
          else // on edge or inside
            {
              // set all directions to 0.0 first
              for (iDirection = 0; iDirection < 8; iDirection++)
                {
                  vCascade_parameter_out[iDirection][lRow * ncolumn + lColumn] =
                      0.0;
                }
              dDistance = 0.0; // set to 0.0 every time
              dElevation = vElevation_in[lRow * ncolumn + lColumn];
              aElevations = get_surrounding_elevation(
                  lRow, lColumn, vBoundary_in, vElevation_in);
              aElevations_sort = aElevations; // a copy of the dem for sort
              std::sort(aElevations_sort.begin(), aElevations_sort.end());
              iIterator1 =
                  std::find(aElevations_sort.begin(), aElevations_sort.end(),
                            dElevation); // find the index of the center dem
              iPosition1 = std::distance(aElevations_sort.begin(), iIterator1);
              if (iPosition1 > 0) // the index of center dem
                {
                  for (iPosition = 0; iPosition < iPosition1; iPosition++)
                    {
                      dDistance = dDistance +
                                  (dElevation - aElevations_sort[iPosition]);
                    }
                  for (iPosition = 0; iPosition < iPosition1;
                       iPosition++) // get all lower percetange
                    {
                      iIterator2 = std::find(
                          aElevations.begin(), aElevations.end(),
                          aElevations_sort[iPosition]); // find the location of
                                                        // the lower pixel
                      iPosition2 =
                          std::distance(aElevations.begin(), iIterator2);
                      switch (iPosition2 - 1)
                        {
                        case 0:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[0][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[0][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 1:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[1][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[1][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 2:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[2][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[2][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 3:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[3][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[3][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 4:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[4][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[4][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 5:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[5][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[5][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 6:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[6][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[6][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        case 7:
                          dPercent = (dElevation - aElevations[iPosition2]) /
                                     dDistance;
                          if (dPercent > dThreshold)
                            {
                              vCascade_parameter_out[7][lRow * ncolumn +
                                                        lColumn] = dPercent;
                            }
                          else
                            {
                              vCascade_parameter_out[7][lRow * ncolumn +
                                                        lColumn] = 0.0;
                            }
                          break;
                        default: // there is no other possibilities.
                          break;
                        } // finish switch case
                    }     // finish calculate all percentage
                }
              else
                {
                }
            } // boundary type
        }     // finish col loop
    }         // finish row loop
  this->vCascade_parameter = vCascade_parameter_out;
  return vCascade_parameter_out;
}

/// Check whether is local depression within dem or not
/// \param vBoundary_in
/// \param vElevation_in
/// \return
int cascade::check_dem_depression(std::vector<double> vBoundary_in,
                                  std::vector<double> vElevation_in)
{
  int iBoundary;
  std::ptrdiff_t iPosition;
  int iDepression = 0;
  long lRow, lColumn;
  double dElevation;
  std::array<double, 9>::iterator iIterator;
  std::array<double, 9> aElevations = {};
  std::array<double, 9> aElevations_sort = {};
  for (lRow = 0; lRow < nrow; lRow++)
    {
      for (lColumn = 0; lColumn < ncolumn; lColumn++)
        {
          iBoundary = (int)vBoundary_in[lRow * ncolumn + lColumn]; // boundary
          dElevation = vElevation_in[lRow * ncolumn + lColumn];
          switch (iBoundary)
            {
            case 1: // edge
              break;
            case 2: // land
              aElevations[0] = dElevation;
              aElevations[1] = vElevation_in[(lRow - 1) * ncolumn + lColumn];
              aElevations[2] =
                  vElevation_in[(lRow - 1) * ncolumn + (lColumn + 1)];
              aElevations[3] = vElevation_in[lRow * ncolumn + (lColumn + 1)];
              aElevations[4] =
                  vElevation_in[(lRow + 1) * ncolumn + (lColumn + 1)];
              aElevations[5] = vElevation_in[(lRow + 1) * ncolumn + lColumn];
              aElevations[6] =
                  vElevation_in[(lRow + 1) * ncolumn + (lColumn - 1)];
              aElevations[7] = vElevation_in[lRow * ncolumn + (lColumn - 1)];
              aElevations[8] =
                  vElevation_in[(lRow - 1) * ncolumn + (lColumn - 1)];
              aElevations_sort = aElevations; // a copy of the dem for sort
              std::sort(aElevations_sort.begin(), aElevations_sort.end());
              iIterator =
                  std::find(aElevations_sort.begin(), aElevations_sort.end(),
                            dElevation); // find the index of the center dem
              iPosition = std::distance(aElevations_sort.begin(), iIterator);
              if (iPosition == 0)
                {
                  iDepression = iDepression + 1;
                }
              break;
            default:
              // outside
              break;
            }
        }
    }
  return iDepression;
}

///
/// \param lRow_in
/// \param lColumn_in
/// \param vBoundary_in
/// \param vElevation_in
/// \return
std::array<double, 9>
cascade::get_surrounding_elevation(long lRow_in, long lColumn_in,
                                   std::vector<double> vBoundary_in,
                                   std::vector<double> vElevation_in)
{
  int iBoundary;
  double dElevation;
  std::array<double, 9> aElevations = {0.0};
  aElevations.fill(9999.0);
  iBoundary = (int)vBoundary_in[lRow_in * ncolumn + lColumn_in];
  dElevation = vElevation_in[lRow_in * ncolumn + lColumn_in];
  switch (iBoundary)
    {
    case 1: // edge
      {
        aElevations[0] = dElevation;
        if (lRow_in > 0 &&
            vBoundary_in[(lRow_in - 1) * ncolumn + lColumn_in] != -9999)
          {
            aElevations[1] =
                vElevation_in[(lRow_in - 1) * ncolumn + lColumn_in];
          }
        if (lColumn_in < (ncolumn - 1) && lRow_in > 0 &&
            vBoundary_in[(lRow_in - 1) * ncolumn + (lColumn_in + 1)] != -9999)
          {
            aElevations[2] =
                vElevation_in[(lRow_in - 1) * ncolumn + (lColumn_in + 1)];
          }
        if (lColumn_in < (ncolumn - 1) &&
            vBoundary_in[lRow_in * ncolumn + (lColumn_in + 1)] != -9999)
          {
            aElevations[3] =
                vElevation_in[lRow_in * ncolumn + (lColumn_in + 1)];
          }
        if (lColumn_in < (ncolumn - 1) && lRow_in < (nrow - 1) &&
            vBoundary_in[(lRow_in + 1) * ncolumn + (lColumn_in + 1)] != -9999)
          {
            aElevations[4] =
                vElevation_in[(lRow_in + 1) * ncolumn + (lColumn_in + 1)];
          }
        if (lRow_in < (nrow - 1) &&
            vBoundary_in[(lRow_in + 1) * ncolumn + lColumn_in] != -9999)
          {
            aElevations[5] =
                vElevation_in[(lRow_in + 1) * ncolumn + lColumn_in];
          }
        if (lRow_in < (nrow - 1) && lColumn_in > 0 &&
            vBoundary_in[(lRow_in + 1) * ncolumn + lColumn_in - 1] != -9999)
          {
            aElevations[6] =
                vElevation_in[(lRow_in + 1) * ncolumn + (lColumn_in - 1)];
          }
        if (lColumn_in > 0 &&
            vBoundary_in[(lRow_in)*ncolumn + (lColumn_in - 1)] != -9999)
          {
            aElevations[7] =
                vElevation_in[lRow_in * ncolumn + (lColumn_in - 1)];
          }
        if (lColumn_in > 0 && lRow_in > 0 &&
            vBoundary_in[(lRow_in - 1) * ncolumn + (lColumn_in - 1)] != -9999)
          {
            aElevations[8] =
                vElevation_in[(lRow_in - 1) * ncolumn + (lColumn_in - 1)];
          }
      }
      break;
    case 2: // not on edge
      {
        aElevations[0] = dElevation;
        aElevations[1] = vElevation_in[(lRow_in - 1) * ncolumn + lColumn_in];
        aElevations[2] =
            vElevation_in[(lRow_in - 1) * ncolumn + (lColumn_in + 1)];
        aElevations[3] = vElevation_in[lRow_in * ncolumn + (lColumn_in + 1)];
        aElevations[4] =
            vElevation_in[(lRow_in + 1) * ncolumn + (lColumn_in + 1)];
        aElevations[5] = vElevation_in[(lRow_in + 1) * ncolumn + lColumn_in];
        aElevations[6] =
            vElevation_in[(lRow_in + 1) * ncolumn + (lColumn_in - 1)];
        aElevations[7] = vElevation_in[lRow_in * ncolumn + (lColumn_in - 1)];
        aElevations[8] =
            vElevation_in[(lRow_in - 1) * ncolumn + (lColumn_in - 1)];
      }
      break;
    default: // inactive
      // unlikely
      break;
    }
  return aElevations;
}

/// save cascade simulation
/// \param sWorkspace_data_in
/// \param vCascade_parameter_in
/// \return
int cascade::save_cascade_parameter(
    std::string sWorkspace_data_in,
    std::vector<std::vector<double>> vCascade_parameter_in)
{
  int error_code = 1;
  int iDirection;
  std::string sDirection;
  std::string sFilename_out;
  std::stringstream ss;
  // the direction code used in ArcGIS is used here:
  // http://pro.arcgis.com/en/pro-app/tool-reference/spatial-analyst/how-flow-direction-works.htm
  // std::array<int, 8> aDirection = { {1, 2, 4, 8, 16, 32, 64, 128} };
  for (iDirection = 0; iDirection < 8; iDirection++)
    {
      sDirection = convert_integer_to_string(int(pow(2.0, iDirection)), 3);
      ss.str("");
      ss << sWorkspace_data_in << slash << "cascade_para_" << sDirection
         << ".dat";
      sFilename_out = ss.str();
      data::write_binary_vector(sFilename_out,
                                vCascade_parameter_in[iDirection - 1]);
    }
  return error_code;
}

///
/// \param sFilename_outlet
/// \return
// std::vector<vector<long>> cascade::read_stream_outlet(std::string
// sFilename_outlet)
//{
//  long lRow, lColumn;
//  std::vector<long> vRow_outlet;
//  std::vector<long> vColumn_outlet;
//  std::vector<vector<long>> vOutlets;
//  std::ifstream ifs;
//  ifs.open(sFilename_outlet.c_str(), ios::in);
//  if (ifs.good())
//  {
//    ifs >> lRow >> lColumn;
//    vRow_outlet.push_back(lRow);
//    vColumn_outlet.push_back(lColumn);
//  }
//  vOutlets.push_back(vRow_outlet);
//  vOutlets.push_back(vColumn_outlet);
//  return vOutlets;
//}
} // namespace ecohydrology
