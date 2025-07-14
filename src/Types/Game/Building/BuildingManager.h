#pragma once

#include "BuildingCommon.h"

namespace BuildingManager {
void          init();
int           get_bot_count();
BuildingInfo *get_bot_info_by_name(const std::string &name);
BuildingInfo *get_bot_info_by_index(int index);
int32_t       get_bot_level_from_name(const std::string &name);
} // namespace BuildingManager
