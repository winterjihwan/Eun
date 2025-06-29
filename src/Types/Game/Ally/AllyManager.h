#pragma once

#include "AllyCommon.h"

namespace AllyManager {
void      init();
int       get_ally_count();
AllyInfo *get_ally_info_by_name(const std::string &name);
AllyInfo *get_ally_info_by_index(int index);
int32_t   get_ally_level_from_name(const std::string &name);
} // namespace AllyManager
