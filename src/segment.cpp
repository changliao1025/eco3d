#include "segment.h"
namespace ecohydrology
{
segment::segment(void)
{
  // aInflow_lateral.fill(0.0);
  // aInflow_upstream.fill(0.0);
  // aInflow.fill(0.0);
  // aOutflow.fill(0.0);
}
segment::~segment(void) {}
// 50==================================================
// prepare the parameters for segment routing
// 50==================================================
int segment::initialize()
{

  int error_code = 1;
  return error_code;
}
// 50==================================================
// all the segment must be calculated at the same time because of the network
// once the upsegment state is updated in hourly time step, the down segment
// must be updated as well. int segment::calculate_segment_flow_routing()
//{
// int error_code = 1;
// return error_code;
//}

// int segment::run()
//{
// int error_code = 1;
// calculate_segment_flow_routing();
// return error_code;
//}
int segment::save_variables()
{
  int error_code = 1;
  // save inflow and ouotflow of each stream segment
  return error_code;
}
} // namespace ecohydrology
