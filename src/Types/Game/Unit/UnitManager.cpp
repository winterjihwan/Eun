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
  dark_knight.animation_names.walk   = "";
  dark_knight.animation_names.attack = "";
  dark_knight.audio_files.attack     = "";
  dark_knight.damage                 = 10;
  dark_knight.base_position          = glm::vec3(0.0f, PLATFORM_HEIGHT, 0.0f);
  dark_knight.base_scale             = glm::vec3(0.005f);
  dark_knight.collider_shape         = Capsule(0.2f, 0.5f);
  dark_knight.collider_offset        = glm::vec3(0.0f, 0.4f, 0.0f);
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
