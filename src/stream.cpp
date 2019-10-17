#include "stream.h"
namespace ecohydrology
{
stream::stream(void) {}
stream::~stream(void) {}
int stream::initialize_stream(int iHru_type, double dHydraulic_radius_in,
                              double dLength_in, double dManning_roughness_in,
                              double dSlope_in)
{
  int error_code = 1;
  return error_code;
}

int stream::run_stream_model()
{
  int error_code = 1;

  return error_code;
}
} // namespace ecohydrology
