#pragma once
//#include "geology.h"
//#include "groundwater.h"
//#include "hru.h"
//#include "landcover.h"
//#include "reach.h"

namespace ecohydrology
{
class stream
{
public:
  stream(void);
  ~stream(void);

  int initialize_stream(int iHru_type, double dHydraulic_radius, double dLength,
                        double dManning_roughness, double dSlope);

  int run_stream_model();
};
} // namespace ecohydrology