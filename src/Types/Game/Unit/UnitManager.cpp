#include "UnitManager.h"
#include "Defines.h"
#include "Types/Game/Unit/UnitCommon.h"
#include "Util//Util.h"

#include <vector>

namespace UnitManager {
std::vector<UnitInfo> _units;

void init() {
  _units.clear();

  // Dark_Knight
  UnitInfo &dark_knight              = _units.emplace_back();
  dark_knight.name                   = "Dark_Knight";
  dark_knight.model_name             = "Dark_Knight";
  dark_knight.animation_names.idle   = "Dark_Knight_Idle";
  dark_knight.animation_names.walk   = "Dark_Knight_Walk";
  dark_knight.animation_names.attack = "Dark_Knight_Kick";
  dark_knight.audio_files.attack     = "";
  dark_knight.damage                 = 10;
  dark_knight.attack_cooldown        = 1.5f;
  dark_knight.enhance_chance         = 0.9f;
  dark_knight.base_position          = glm::vec3(0.0f, PLATFORM_HEIGHT, 0.0f);
  dark_knight.base_scale             = glm::vec3(0.005f);
  dark_knight.collider_shape         = Capsule(0.2f, 0.5f);
  dark_knight.collider_offset        = glm::vec3(0.0f, 0.4f, 0.0f);

  // Demon_Fairy
  UnitInfo &demon_fairy              = _units.emplace_back();
  demon_fairy.name                   = "Demon_Fairy";
  demon_fairy.model_name             = "Demon_Fairy";
  demon_fairy.animation_names.idle   = "Demon_Fairy_Idle";
  demon_fairy.animation_names.walk   = "Demon_Fairy_Walk";
  demon_fairy.animation_names.attack = "Demon_Fairy_Cast";
  demon_fairy.audio_files.attack     = "";
  demon_fairy.damage                 = 20;
  demon_fairy.attack_cooldown        = 2.0f;
  demon_fairy.enhance_chance         = 0.7f;
  demon_fairy.base_position          = glm::vec3(0.0f, PLATFORM_HEIGHT, 0.0f);
  demon_fairy.base_scale             = glm::vec3(0.005f);
  demon_fairy.collider_shape         = Capsule(0.2f, 0.5f);
  demon_fairy.collider_offset        = glm::vec3(0.0f, 0.4f, 0.0f);
}

int get_unit_count() {
  return _units.size();
}

UnitInfo *get_unit_info_by_name(const std::string &name) {
  for (int i = 0; i < _units.size(); i++) {
    if (_units[i].name == name) {
      return &_units[i];
    }
  }
  return nullptr;
}

UnitInfo *get_unit_info_by_index(int index) {
  if (index >= 0 && index < _units.size()) {
    return &_units[index];
  }
  return nullptr;
}

int32_t get_unit_level_from_name(const std::string &name) {
  std::vector<UnitInfo> _units;
  for (int i = 0; i < _units.size(); i++) {
    if (_units[i].name == name) {
      return i;
    }
  }
  return -1;
}
} // namespace UnitManager
