#include "Enhance.h"
#include "Types/Game/Unit/Unit.h"
#include "Util/Util.h"
#include "World/World.h"

namespace Enhance {
glm::vec2 _xz    = glm::vec2(0.0f, 0.0f);
glm::vec3 _scale = glm::vec3(1.0f);

void init() {
  _xz    = glm::vec2(-8.0f, 0.0f);
  _scale = glm::vec3(4.0f, 8.0f, 1.0f);
}

void update(float delta_time) {
  std::vector<Unit> &units = World::get_units();

  // Hack
  for (Unit &unit : units) {
    glm::vec3 position = unit.get_position();
    glm::vec2 unit_xz  = glm::vec2(position.x, position.z);
    glm::vec2 center   = Enhance::get_xz();
    glm::vec2 half     = glm::vec2(Enhance::get_scale().x, Enhance::get_scale().y) * 0.5f;

    if (glm::all(glm::lessThanEqual(glm::abs(unit_xz - center), half))) {
      float rand = Util::random_float(0.0f, 1.0f);
      unit.die();

      // Success
      if (rand < unit.get_enhance_chance()) {
        World::spawn_unit("Demon_Fairy");
      }
    }
  }
}

glm::vec2 &get_xz() {
  return _xz;
}

glm::vec3 &get_scale() {
  return _scale;
}

} // namespace Enhance
