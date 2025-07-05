#include "AllyManager.h"
#include "Types/Game/Ally/AllyCommon.h"
#include "Util//Util.h"

#include <vector>

namespace AllyManager {
std::vector<AllyInfo> _allies;

void init() {
  _allies.clear();

  /* Allies */

  // Maw
  AllyInfo &maw              = _allies.emplace_back();
  maw.name                   = "Maw";
  maw.skinned_model          = "Maw";
  maw.animation_names.idle   = "Maw_Idle";
  maw.animation_names.walk   = "";
  maw.animation_names.attack = "";
  maw.audio_files.attack     = "";
  maw.damage                 = 10;
  maw.base_scale             = glm::vec3(0.005f);
  maw.capsule_radius         = 0.2f;
  maw.capsule_height         = 0.50f;
  maw.capsule_position =
      Util::to_capsule_position(maw.base_position, maw.capsule_height, maw.capsule_radius);
}

int get_ally_count() {
  return _allies.size();
}

AllyInfo *get_ally_info_by_name(const std::string &name) {
  for (int i = 0; i < _allies.size(); i++) {
    if (_allies[i].name == name) {
      return &_allies[i];
    }
  }
  return nullptr;
}

AllyInfo *get_ally_info_by_index(int index) {
  if (index >= 0 && index < _allies.size()) {
    return &_allies[index];
  }
  return nullptr;
}

int32_t get_ally_level_from_name(const std::string &name) {
  std::vector<AllyInfo> _allies;
  for (int i = 0; i < _allies.size(); i++) {
    if (_allies[i].name == name) {
      return i;
    }
  }
  return -1;
}
} // namespace AllyManager
