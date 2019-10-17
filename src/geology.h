// 50==================================================
/*!
\file   geology.h
\brief

The class for geology component.

\author Chang Liao
\date   2016-09-30
*/
// 50==================================================
#pragma once
namespace ecohydrology
{
class geology
{
public:
  geology();
  ~geology();

  int iBoundary; // whether this pixel is active or not
  double dResolution;
  double dArea;
  double dAspect; // degree
  double dLatitude;
  double dSlope;
  double dSurface_elevation;
};
} // namespace ecohydrology
