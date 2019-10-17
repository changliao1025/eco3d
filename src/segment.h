#pragma once
#include <array>
#include <sstream>
//#include "global.h"
//#include "dimension.h"
//#include "system.h"

using namespace std;
// 50==================================================

// 50==================================================
// in our ecosystem model, streams are broken into a list of segments.
// each segment is further broken into a list of reaches.
// please refer to http://water.usgs.gov/nawqa/protocols/OFR-93-408/habitp5.html
// 50==================================================
namespace ecohydrology
{
class segment
{
public:
  segment(void);
  ~segment(void);

  // 50==================================================
  // function
  // 50==================================================

  int initialize();

  int save_variables();

  // variables=========================================================
  // std::string sFilename_segment_topology; //the file to store all the
  // topology relation

  // std::array< int, nsegment> aTime_step;
  // std::array< double, nsegment> aC0; //parameter
  // std::array< double, nsegment> aC1;
  // std::array< double, nsegment> aC2;

  // std::array< double, nsegment> aInflow_lateral; //the total lateral flow
  // into the segment std::array< double, nsegment> aInflow_upstream; //the total
  // upstream flow into the segment std::array< double, nsegment> aInflow;
  // //total inflow std::array< double, nsegment> aOutflow;
};
} // namespace ecohydrology
