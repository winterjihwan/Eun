#include "BuildingManager.h"
#include "Types/Game/Building/BuildingCommon.h"
#include "Util/Util.h"

#include <vector>

namespace BuildingManager {
std::vector<BuildingInfo> _bots;

void init() {
  _bots.clear();

  /* Buildings */

  // Police_Car
  BuildingInfo &police_car  = _bots.emplace_back();
  police_car.name           = "Police_Car";
  police_car.model_name     = "Police_Car";
  police_car.is_animated    = false;
  police_car.health         = 100;
  police_car.reward         = 10;
  police_car.base_position  = glm::vec3(0.0f, -2.28f, -8.0f);
  glm::quat rotation_x      = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
  glm::quat rotation_y      = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0, 0, 1));
  police_car.base_rotation  = rotation_x * rotation_y;
  police_car.base_scale     = glm::vec3(0.4);
  police_car.collider_shape = Cube(5.8f, 0.5f, 2.0f);
}

int get_bot_count() {
  return _bots.size();
}

BuildingInfo *get_bot_info_by_name(const std::string &name) {
  for (int i = 0; i < _bots.size(); i++) {
    if (_bots[i].name == name) {
      return &_bots[i];
    }
  }

  assert(0);
}

BuildingInfo *get_bot_info_by_index(int index) {
  if (index >= 0 && index < _bots.size()) {
    return &_bots[index];
  }

  assert(0);
}

int32_t get_bot_level_from_name(const std::string &name) {
  std::vector<BuildingInfo> _bots;
  for (int i = 0; i < _bots.size(); i++) {
    if (_bots[i].name == name) {
      return i;
    }
  }

  assert(0);
}
} // namespace BuildingManager
