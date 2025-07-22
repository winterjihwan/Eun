#pragma once
#include <glm/glm.hpp>

namespace Enhance {
void       init();
void       update(float delta_time);
glm::vec2 &get_xz();
glm::vec3 &get_scale();
} // namespace Enhance
