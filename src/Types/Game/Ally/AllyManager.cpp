#include "AllyManager.h"
#include "Types/Game/Ally/AllyCommon.h"
#include "Util//Util.h"

#include <vector>

namespace AllyManager {
std::vector<AllyInfo> _allies;

void init() {
  _allies.clear();

  /* Allies */

  // Greece_Soldier
  AllyInfo &greece_soldier              = _allies.emplace_back();
  greece_soldier.name                   = "Greece_Soldier";
  greece_soldier.skinned_model          = "Greece_Soldier";
  greece_soldier.animation_names.idle   = "Greece_Soldier_Idle";
  greece_soldier.animation_names.walk   = "";
  greece_soldier.animation_names.attack = "";
  greece_soldier.audio_files.attack     = "";
  greece_soldier.damage                 = 10;
  greece_soldier.base_scale             = glm::vec3(0.005f);
  greece_soldier.capsule_radius         = 0.1f;
  greece_soldier.capsule_height         = 0.75f;
  greece_soldier.capsule_position       = Util::to_capsule_position(
      greece_soldier.base_position, greece_soldier.capsule_height, greece_soldier.capsule_radius);
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
