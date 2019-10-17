#pragma once
// the global variables for all other modules
// c library

// c++ library

#include <cmath>
#include <string>

// cros-platform header
// 50==================================================
// local header
// 50==================================================
#include "conversion.h"
#include "system.h"
// 50==================================================
using namespace std;
// 50==================================================
// model parameters
// 50==================================================

class miscellaneous
{
public:
  miscellaneous(void);
  ~miscellaneous(void);
  static std::string find_alternative_file(int iDay, int iYear,
                                           std::string sExtension_in,
                                           std::string sPrefix_in,
                                           std::string sWorkspace_in);
};
