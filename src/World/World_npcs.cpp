#include "Types/Game/Unit/Unit.h"
#include "World/World.h"
#include <string>

namespace World {

void spawn_unit(const std::string &name, const glm::vec3 &position) {
  Unit unit;
  unit.init(name, position);

  add_unit(std::move(unit));
}

} // namespace World
