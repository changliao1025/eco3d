#include "lake.h"
namespace ecohydrology
{
lake::lake() {}

lake::~lake() {}

int lake::initialize_lake(eColumn_type eCU_type)
{
  int error_code = 1;
  this->eCU_type = eCU_type;
  cLandcover.initialize_landcover();
  return error_code;
}

int lake::update_lake_parameter(int iLand_cover_type)
{
  int error_code = 1;
  cLandcover.update_landcover_parameter(iLand_cover_type);
  return error_code;
}

} // namespace ecohydrology
