// 50==================================================
/*!
  \file   landcover.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-26
*/
// 50==================================================
#include "landcover.h"
namespace ecohydrology
{
landcover::landcover() { iLand_cover_type_default = 1; }
landcover::~landcover() {}
// 50==================================================
/*!
  <long-description>

  \param
  \return <ReturnValue>
*/
// 50==================================================
int landcover::initialize_landcover()
{
  int error_code = 1;
  this->iLand_cover_type = iLand_cover_type_default;
  return error_code;
}
// 50==================================================
/*!
  <long-description>

  \param iLand_cover_type
  \return <ReturnValue>
*/
// 50==================================================
int landcover::update_landcover_parameter(int iLand_cover_type)
{
  int error_code = 1;
  this->iLand_cover_type = iLand_cover_type;
  return error_code;
}
} // namespace ecohydrology
