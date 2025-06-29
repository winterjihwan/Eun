#include "BotManager.h"
#include "Types/Game/Bot/BotCommon.h"

#include <vector>

namespace BotManager {
std::vector<BotInfo> _bots;

void init() {
  _bots.clear();

  /* Bots */

  // Mannequin
  BotInfo &mannequin             = _bots.emplace_back();
  mannequin.name                 = "Mannequin";
  mannequin.skinned_model        = "Mannequin";
  mannequin.animation_names.idle = "Mannequin_Idle";
  mannequin.audio_files.hurt     = "";
  mannequin.health               = 100;
  mannequin.reward               = 10;
  mannequin.base_position        = glm::vec3(0.0f, -3.0f, -5.0f);
  mannequin.base_scale           = glm::vec3(.01f, .01f, .01f);
  mannequin.capsule_radius       = 0.2f;
  mannequin.capsule_height       = 1.5f;
  mannequin.capsule_position =
      glm::vec3(mannequin.base_position.x,
                (mannequin.capsule_height + 2.0f * mannequin.capsule_radius) / 2.0f +
                    mannequin.base_position.y,
                mannequin.base_position.z);
}

int get_bot_count() {
  return _bots.size();
}

BotInfo *get_bot_info_by_name(const std::string &name) {
  for (int i = 0; i < _bots.size(); i++) {
    if (_bots[i].name == name) {
      return &_bots[i];
    }
  }
  return nullptr;
}

BotInfo *get_bot_info_by_index(int index) {
  if (index >= 0 && index < _bots.size()) {
    return &_bots[index];
  }
  return nullptr;
}

int32_t get_bot_level_from_name(const std::string &name) {
  std::vector<BotInfo> _bots;
  for (int i = 0; i < _bots.size(); i++) {
    if (_bots[i].name == name) {
      return i;
    }
  }
  return -1;
}
} // namespace BotManager
