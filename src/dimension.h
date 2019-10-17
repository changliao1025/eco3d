// 50==================================================
/*!
\file   dimension.h
\brief

The global dimension definition.

\author Chang Liao
\date   2016-09-30
*/
// 50==================================================
#pragma once
// the global variables for all other modules
// 50==================================================
// const double inch_2_centimeter = 2.54;
// const double dArea = 500.0 * 500.0;
// 50==================================================
// time
const int nmonth = 12;
// spatial
const long ncolumn = 388;
const long nrow = 310;
// watershed
const long nsegment = 37;
// 50==================================================
// land cover type is based on modis land cover product IGBP scheme
// http://www.changliao.us/2016/05/ecosystem-modeling-101.html
// land cover type index from 0 to 17 (0 is water, so in fact its range from 1
// to 16, 17 is missing value)
// 50==================================================
const int nlandcover_type = 16;
// 50==================================================
// soil type is based on soil taxonomy
// a basic system of soil classification for making and interpreting soil
// surveys the soil types are Alfisols: 1 Andisols:2 Aridisols: 3 Entisols:4
// Gelisols:5
// Histosols:6
// Inceptisols:7
// Mollisols:8
// Oxisols:9
// Spodosol:10
// Ultisoils:11
// Vertosols:12
// rocky soil: 13
// can also be found at:
// http://www.nrcs.usda.gov/wps/portal/nrcs/detail/soils/edu/?cid=nrcs142p2_053588
// soil type index from 1 to 12
// 50==================================================
const int nsoil_type = 13;
const int nsoil_layer = 6;
// 50==================================================
// vegetation type is based on land cover type, we define non-vegetation type as
// missing type vegetation type index from 1 to 12
// 50==================================================
// 1 Evergreen Needleleaf forest
// 2 Evergreen Broadleaf forest
// 3 Deciduous Needleleaf forest
// 4 Deciduous Broadleaf Forest
// 5 Mixed Forest
// 6 Close Shurbland
// 7 Open shurbland
// 8 Woody savanan
// 9 Savanna
// 10 grassland
// 11 wetland
// 12 cropland
const int nvegetation_type = 12;
// land, stream, lake, swale, glacier
const int nhru_type = 5;
// 50==================================================
// const int iFlag_run_carbon_cascade = 1;
enum eColumn_type // same with hru type
{
  eCU_glacier = 1,
  eCU_lake = 2,
  eCU_land = 3,
  eCU_stream = 4,
  eCU_swale = 5 // C++11 allows the extra comma
};
