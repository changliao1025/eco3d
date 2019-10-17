#pragma once

#include "dimension.h"
#include "global.h"
#include <array>
#include <cmath>

using namespace std;

// Biome-specific parameters for function q10 to describe the effect of
// temperature on plant respiration
const std::array<double, nvegetation_type> aQ10a0 = {
    {2.35665, 2.35665, 2.35665, 2.35665, 2.35665, 2.35665, 2.35665, 2.35665,
     2.35665, 2.35665, 2.35665, 2.35665}};
const std::array<double, nvegetation_type> aQ10a1 = {
    {-0.053077, -0.053077, -0.053077, -0.053077, -0.053077, -0.053077,
     -0.053077, -0.053077, -0.053077, -0.053077, -0.053077, -0.053077}};
const std::array<double, nvegetation_type> aQ10a2 = {
    {0.0023842, 0.0023842, 0.0023842, 0.0023842, 0.0023842, 0.0023842,
     0.0023842, 0.0023842, 0.0023842, 0.0023842, 0.0023842, 0.0023842}};

const std::array<double, nvegetation_type> aQ10a3 = {
    {-0.0000411, -0.0000411, -0.0000411, -0.0000411, -0.0000411, -0.0000411,
     -0.0000411, -0.0000411, -0.0000411, -0.0000411, -0.0000411, -0.0000411}};
// 50==================================================
const std::array<double, nvegetation_type> aKra = {
    {-0.000587, -0.000587, -0.0000806, -0.0000806, -0.0000806, -0.000587,
     -0.0000806, -0.0000403, -0.0000806, -0.0000806, -0.0000806, -0.000587}};

const std::array<double, nvegetation_type> aKrb = {
    {-19.043, -19.043, -18.76, -20.155, -20.541032, -20.04475, -19.669, -19.814,
     -20.84583, -20.832651, -19.0005, -20.832651}};
// 50==================================================
// this class implements major vegetation autotrophic respiration process
// algorithm, mainly maintenance respiration and growth respiration in this
// model, the maintenance respiration include canopy respiration, stem
// respiration and root respiration  however, not all of them are implemented
// yet
namespace ecohydrology
{
class respiration
{
public:
  respiration();

  ~respiration();

  int iVegetation_type;
  double dMaintenance_respiration;
  double dGrowth_respiration;
  double dAutotrophic_respiration;

  double dKra;
  double dKrb;
  double dQ10a0;
  double dQ10a1;
  double dQ10a2;
  double dQ10a3;

  int calculate_plant_autotrophic_respiration(
      double dVegetation_carbon_in, double dVegetation_gpp_in,
      double dVegetation_temperature_in);

  int calculate_plant_growth_respiration(double dGpp_in);

  int calculate_plant_maintenance_respiration(
      double dCarbon_vegetation_in, double dTemperature_vegetation_in);

  double calculate_plant_respiration_q10(double dTemperature_in);

  int initialize_respiration(eColumn_type eCU_type, int iVegetation_type);

  int run_respiration_model(int iVegetation_type, double dVegetation_carbon_in,
                            double dVegetation_gpp_in,
                            double dVegetation_temperature_in);

  int update_respiration_parameter(int iVegetation_type);
};
} // namespace ecohydrology
