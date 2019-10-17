#pragma once

#include "dimension.h"
#include "global.h"
#include <array>
#include <cmath>

using namespace std;

// 50==================================================
// Biome-specific nitrogen uptake parameters for function nupxclm
// the maximum rate of N uptake by the vegetation (g/m2/day)

const std::array<double, nvegetation_type> aNmax = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nmaxcut = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nmax1a = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nmax1b = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nmax2a = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
const std::array<double, nvegetation_type> nmax2b = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};
// 50==================================================
const std::array<double, nvegetation_type> aKn1 = {
    {0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042, 0.0042,
     0.0042, 0.0042, 0.0042}};

// 50==================================================
namespace ecohydrology
{
class root
{
public:
  root();

  ~root();

  double dRoot_carbon;
  double dRoot_carbon_decomposed;

  //  double dNmax;
  //  double dmaxcut;
  //  double dmax1a;
  //  double dmax1b;
  //  double dmax2a;
  //  double dmax2b;
  //
  //  double dKn1;

  int initialize_root(eColumn_type eCU_type, int iVegetation_type);

  int root_decomposition(int iMonth, double dTemperature_in);

  int run_root_model(int iVegetation_type, int iMonth, double dTemperature_in);

  int update_root_parameter(int iVegetation_type);

  int update_root_status(int iVegetation_type);

  // double dRoot_nitrogen;
  // double dNitrogen_uptake;
  // int calculate_nitrogen_uptake(double dNitrogen_available_in,
  //	double dQ10_in,
  //	double dScalar_soil_moisture_nitrogen_uptake_in);
};
} // namespace ecohydrology
