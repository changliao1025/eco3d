// 50==================================================
/*!
  \file   cascade.h
  \brief

  The class for surface and subsurface cascade component

  \author
  \date   2017-03-02
*/
// 50==================================================
#pragma once
// std=====================================================
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
// 50==================================================
#include "conversion.h"
#include "data.h"
#include "dimension.h"
#include "global.h"
#include "mathematics.h"

using namespace std;

// 50==================================================
namespace ecohydrology
{
class cascade
{
public:
  cascade(void);

  ~cascade(void);

  std::vector<std::vector<double>> vCascade_parameter; // saved for output

  // 50==================================================
  int check_dem_depression(std::vector<double> vBoundary_in,
                           std::vector<double> vElevation_in);

  // 50==================================================
  std::array<double, 9>
  get_surrounding_elevation(long lRow_in, long lColumn_in,
                            std::vector<double> vBoundary_in,
                            std::vector<double> vElevation_in);

  std::vector<std::vector<double>>
  run_cascade_model(std::vector<double> vBoundary_in,
                    std::vector<double> vElevation_in);

  int save_cascade_parameter(
      std::string sWorkspace_data,
      std::vector<std::vector<double>> vCascade_parameter_in);

  // std::vector<vector<long>> read_stream_outlet(std::string sFilename_outlet);
};
} // namespace ecohydrology
