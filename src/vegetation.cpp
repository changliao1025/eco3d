// 50==================================================
/*!
  \file   vegetation.cpp
  \brief
  <long description>
  \author Chang Liao
  \date   2017-01-18
*/
// 50==================================================
#include "vegetation.h"

namespace ecohydrology
{
vegetation::vegetation()
{
  iVegetation_type_default = 0;

  dVegetation_carbon = missing_value;

  dVegetation_npp = missing_value;
}

vegetation::~vegetation() {}
// 50==================================================
/*!
  <long-description>

  \param eCU_type
  \return <ReturnValue>
*/
// 50==================================================
int vegetation::initialize_vegetation(eColumn_type eCU_type)
{
  int error_code = 0;
  // use the default value
  iVegetation_type = iVegetation_type_default;
  switch (eCU_type)
    {
    case eCU_glacier:
      {
      }
      break;
    case eCU_lake:
      break;
    case eCU_land:
      {

        if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
          {
            // there is no vegetation st this time step
            iFlag_vegetation_previous = 0;
            dVegetation_carbon = 0.0;
            dVegetation_npp = 0.0;
            dVegetation_gpp = 0.0;
          }
        else
          {
            iFlag_vegetation_previous = 1;
            dVegetation_carbon = 0.0;
            dVegetation_npp = 0.0;
            dVegetation_gpp = 0.0;
          }
      }
      break;
    case eCU_stream:
      break;
    case eCU_swale:
      break;
    default:
      break;
    }
  cCanopy.initialize_canopy(eCU_type, iVegetation_type);
  cStem.initialize_stem(eCU_type, iVegetation_type);
  cRoot.initialize_root(eCU_type, iVegetation_type);
  cRespiration.initialize_respiration(eCU_type, iVegetation_type);
  return error_code;
}
// 50==================================================
/*!
  <calculate the npp using gpp and RM>
  \param dGpp_in, calculated GPP from canopy photosynthesis, unit, g C * m(-2)
  day(-1) \param dRespiration_autotrophic_in, calculated respiration from whole
  plant respiration, unit, g C * m(-2) day(-1) \return <ReturnValue>
*/
// 50==================================================
int vegetation::calculate_vegetation_npp(double dGpp_in,
                                         double dRespiration_autotrophic_in)
{
  int error_code = 1;
  dVegetation_npp = dGpp_in - dRespiration_autotrophic_in;
  if (dVegetation_npp < 0.0)
    {
      dVegetation_npp = 0.0;
    }
  return error_code;
}
// 50==================================================
/*!
  <Since land cover type data from modis not only include vegetation,
  but also include water body, etc. So this function is used to
  map land cover type[0-17] fo vegetation type[1-12]>
  \param iLand_cover_type
  \return <ReturnValue>
*/
// 50==================================================
int vegetation::convert_land_cover_to_vegetation_type(int iLand_cover_type)
{
  int iVegetation_type = -1; // the default missing
  if (iLand_cover_type != -1)
    {
      switch (iLand_cover_type)
        {
        case 0: // water
          iVegetation_type = -1;
          break;
        case 13: // urban
          iVegetation_type = -1;
          break;
        case 14: // mosaic
          iVegetation_type = 12;
          break;
        case 15: // snow and ice
          iVegetation_type = -1;
          break;
        case 16: // barren
          iVegetation_type = -1;
          break;
        case 17: // glacier will be added in the future?
          iVegetation_type = -1;
          std::cout << "Something is wrong about vegetation type " << endl;
          break;
        default:
          // the default vegetation type is the same with landcover
          iVegetation_type = iLand_cover_type;
          break;
        }
    }
  else
    {
    }
  if (iVegetation_type > 12)
    {
      std::cout << "Something is wrong about vegetation type " << endl;
    }
  return iVegetation_type;
}

// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
int vegetation::run_vegetation_model(int iMonth, int iPrecipitation_type,
                                     double dArea, double dLand_pet,
                                     double dLai_in, double dRain_in,
                                     double dSnow_in, double dFpar_in,
                                     double dPar_in,
                                     double dTemperature_mean_in,
                                     double dVapor_pressure_deficit_in)
{
  int error_code = 1;
  double dLai = dLai_in;
  if (dLai < small_value)
    {
      dLai = 0.0;
    }
  cCanopy.run_canopy_model(iMonth, iPrecipitation_type, iVegetation_type, dArea,
                           dLand_pet, dFpar_in, dLai, dRain_in, dSnow_in,
                           dPar_in, dTemperature_mean_in,
                           dVapor_pressure_deficit_in);
  cRoot.run_root_model(iVegetation_type, iMonth, dTemperature_mean_in);

  cRespiration.run_respiration_model(
      iVegetation_type, dVegetation_carbon, dVegetation_gpp,
      dTemperature_mean_in // should be vegetation temperature
  );

  return error_code;
}

// 50==================================================
/*!
  <long-description>
  \param iVegetation_type
  \return <ReturnValue>
*/
// 50==================================================
int vegetation::update_vegetation_parameter(int iVegetation_type)
{
  int error_code = 0;
  this->iVegetation_type = iVegetation_type;

  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
    }
  else
    {
      cCanopy.update_canopy_parameter(iVegetation_type);
      cStem.update_stem_parameter(iVegetation_type);
      cRoot.update_root_parameter(iVegetation_type);
      cRespiration.update_respiration_parameter(iVegetation_type);
    }

  return error_code;
}

// 50==================================================
/*!
  <long-description>
  \param
  \return <ReturnValue>
*/
// 50==================================================
int vegetation::update_vegetation_status()
{
  int error_code = 1;

  if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
    {
      // vegetation
      iFlag_vegetation_previous = 0;
      dVegetation_carbon = 0.0;
      dVegetation_npp = 0.0;
      dVegetation_gpp = 0.0;
    }
  else
    {
      iFlag_vegetation_previous = 1;
    }

  // canopy update
  cCanopy.update_canopy_status(iVegetation_type);
  cCanopy.dCanopy_carbon = cCanopy.dCanopy_carbon -
                           cRespiration.dMaintenance_respiration * 0.4 -
                           cRespiration.dGrowth_respiration * 0.4;
  if (cCanopy.dCanopy_carbon < near_zero)
    {
      cCanopy.dCanopy_carbon = 0.0;
    }
  // stem update

  cStem.dStem_carbon = cStem.dStem_carbon +
                       cCanopy.cPhotosynthesis.dPhotosynthesis_gpp * 0.1 -
                       cRespiration.dMaintenance_respiration * 0.1 -
                       cRespiration.dGrowth_respiration * 0.1;
  if (cStem.dStem_carbon < near_zero)
    {
      cStem.dStem_carbon = 0.0;
    }
  // root update
  cRoot.update_root_status(iVegetation_type);

  cRoot.dRoot_carbon = cRoot.dRoot_carbon +
                       cCanopy.cPhotosynthesis.dPhotosynthesis_gpp * 0.5 -
                       cRespiration.dMaintenance_respiration * 0.5 -
                       cRespiration.dGrowth_respiration * 0.5;

  if (cRoot.dRoot_carbon < near_zero)
    {
      cRoot.dRoot_carbon = 0.0;
    }
  // total
  dVegetation_carbon =
      cCanopy.dCanopy_carbon + cStem.dStem_carbon + cRoot.dRoot_carbon;

  if (dVegetation_carbon < near_zero)
    {
      dVegetation_carbon = 0.0;
    }
  // calculate NPP
  dVegetation_gpp = cCanopy.cPhotosynthesis.dPhotosynthesis_gpp;
  calculate_vegetation_npp(dVegetation_gpp,
                           cRespiration.dAutotrophic_respiration);

  return error_code;
}

//	int vegetation::reset_vegetation_type(int iVegetation_type)
//	{
//		int error_code = 1;
//		if (iVegetation_type < 1 || iVegetation_type > nvegetation_type)
//		{
//			iFlag_vegetation_previous = 0;
//		}
//		else
//		{
//			this->iVegetation_type = -1;
//		}
//
//		return error_code;
//	}
} // namespace ecohydrology
