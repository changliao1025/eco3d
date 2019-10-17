// c++ library/  //50==================================================
/*!
  \file   column.h
  \brief

  The class the most basic unit in ECO3D model.

  \author
  \date   2017-03-02
*/
// 50==================================================
#include <array>
#include <string>
// local header files======================================
#include "atmosphere.h"
#include "dimension.h"
#include "geology.h"
#include "glacier.h"
#include "lake.h"
#include "land.h"
#include "reach.h"
using namespace std;
namespace ecohydrology
{
// 50==================================================
class column
{
public:
  column();
  ~column();

  int iFlag_source_or_sink;
  int iHru_type;


  long lGrid_index;

  long lColumn_index;

  long lRow_index;
  double dColumn_carbon;

  double dColumn_nitrogen;



  double dColumn_nep;

  std::array<long, 8> aNeighbor_index;
  std::array<double, 8> aNeighbor_cascade_percentage;

  eColumn_type eCU_type;
  atmosphere cAtmosphere;
  geology cGeology;
  glacier cGlacier;
  lake cLake;
  land cLand;
  reach cReach;
  int calculate_column_nep();
  int initialize_column();

  int run_column_model(int iDay_doy, int iMonth_ymd, double dFpar_in,
                       double dLai_in, double dPrecipitation_in,
                       double dPrecipitation_basin_in,
                       double dTemperature_basin_max_in,
                       double dTemperature_dewpoint_in,
                       double dTemperature_max_in, double dTemperature_min_in);
  int update_column_status();
  int update_column_parameter(int iFlag_run_carbon_cascade,
                              int iFlag_run_water_cascade,
                              int iLand_cover_type, int iSoil_type,
                              int iVegetation_type,
                              double dImpervious_surface_percentage_in,
                              double dPervious_surface_percentage_in,
                              double dSurface_depression_percentage_in);
};
} // namespace ecohydrology
