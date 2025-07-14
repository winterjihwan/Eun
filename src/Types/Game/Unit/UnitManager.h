#pragma once

#include "UnitCommon.h"

namespace UnitManager {
void      init();
int       get_unit_count();
UnitInfo *get_unit_info_by_name(const std::string &name);
UnitInfo *get_unit_info_by_index(int index);
int32_t   get_unit_level_from_name(const std::string &name);
} // namespace UnitManager
