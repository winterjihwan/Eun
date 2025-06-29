#pragma once

#include "BotCommon.h"

namespace BotManager {
void     init();
int      get_bot_count();
BotInfo *get_bot_info_by_name(const std::string &name);
BotInfo *get_bot_info_by_index(int index);
int32_t  get_bot_level_from_name(const std::string &name);
} // namespace BotManager
