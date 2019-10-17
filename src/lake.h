#pragma once
#include "geology.h"
#include "groundwater.h"
#include "hru.h"
#include "landcover.h"

using namespace std;
namespace ecohydrology
{
class lake
{
public:
  lake();
  ~lake();
  eColumn_type eCU_type;

  hru cHru;
  groundwater cGroundwater;
  landcover cLandcover;
  geology cGeology;

  int initialize_lake(eColumn_type eCU_type);
  int update_lake_parameter(int iLand_cover_type);
};
} // namespace ecohydrology
