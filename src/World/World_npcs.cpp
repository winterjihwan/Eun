#include "Types/Game/Ally/Ally.h"
#include "World/World.h"
#include <string>

namespace World {

void spawn_ally(const std::string &name, const glm::vec3 &position) {
  Ally ally;
  ally.init(name, position);

  add_ally(std::move(ally));
}

} // namespace World
