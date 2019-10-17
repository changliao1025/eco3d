#pragma once

#include <array>
#include <iostream>
#include <string>
// 50==================================================
//
// 50==================================================
#include "dimension.h"
#include "global.h"

using namespace std;

// 50==================================================
// proportion of vegetation carbon lost as litterfall
// parameters are obtained from TEM model
const std::array<double, nvegetation_type> aCoefficient_litterfall_carbon = {
    {0.003483, 0.004722, 0.003483, 0.003889, 0.002037, 0.0016975, 0.0016975,
     0.010659, 0.052910, 0.052910, 0.013333, 0.010659}};
// proportion of vegetation nitrogen lost as litterfall
const std::array<double, nvegetation_type> aCoefficient_litterfall_nitrogen = {
    {0.011370, 0.020567, 0.017890, 0.00700, 0.007955, 0.011540, 0.011540,
     0.0131, 0.03188, 0.03188, 0.00443, 0.0131}};

// 50==================================================
namespace ecohydrology
{
class litterfall
{
public:
  litterfall();

  ~litterfall();

  // int iVegetation_type;
  double dLitterfall_carbon;
  double dCoefficient_litterfall_carbon;

  int calculate_litterfall_carbon(double dCarbon_canopy_in);

  int initialize_litterfall(eColumn_type eCU_type, int iVegetation_type);

  int run_litterfall_model(int iVegetation_type, double dCarbon_canopy_in);

  int update_litterfall_parameter(int iVegetation_type);
  // double dCoefficient_litterfall_nitrogen;
  // double dLitterfall_nitrogen;
  // int calculate_litterfall_nitrogen(double dNitrogen_structural_in);
};
} // namespace ecohydrology
