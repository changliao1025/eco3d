// 50==================================================
/*!
  \file   landcover.h
  \brief

  <long description>

  \author Chang Liao
  \date   2017-07-26
*/
// 50==================================================
#pragma once
namespace ecohydrology
{
class landcover
{
public:
  landcover();
  ~landcover();
  int iLand_cover_type;
  int iLand_cover_type_default;
  // 50==================================================
  // function
  // 50==================================================
  int initialize_landcover();
  int update_landcover_parameter(int iLand_cover_type);
};
} // namespace ecohydrology
