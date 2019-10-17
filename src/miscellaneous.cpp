#include "miscellaneous.h"

miscellaneous::miscellaneous(void) {}
miscellaneous::~miscellaneous(void) {}

std::string miscellaneous::find_alternative_file(int iDay, int iYear,
                                                 std::string sExtension_in,
                                                 std::string sPrefix_in,
                                                 std::string sWorkspace_in)
{
  int search_year;
  std::string sFilename_out = "";
  std::string sDay = convert_integer_to_string(iDay, 3);
  std::string sYear = "";

  auto max_search = 10000;
  int iFound = 0;
  for (auto iSearch = 1; iSearch <= max_search; iSearch++)
    {
      if (iFound == 0)
        {
          search_year = iYear - iSearch;
          sYear = convert_integer_to_string(search_year, 4);

          sFilename_out = sWorkspace_in + slash + sYear + slash + sPrefix_in +
                          sYear + sDay + sExtension_in;
          if (file_test(sFilename_out) == 1)
            {
              iFound = 1;
              break;
            }
          else
            {
              search_year = iYear + iSearch;
              sYear = convert_integer_to_string(search_year, 4);

              sFilename_out = sWorkspace_in + slash + sYear + slash +
                              sPrefix_in + sYear + sDay + sExtension_in;
              if (file_test(sFilename_out) == 1)
                {
                  iFound = 1;
                  break;
                }
              else
                {
                  continue;
                }
            }
        }
      else
        {
          break;
        }
    }
  if (iFound == 1)
    {
      return sFilename_out;
    }
  else
    {
      sFilename_out = "";
      return sFilename_out;
    }
}
