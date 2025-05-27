#include "Util.h"

namespace Util {
JPH::Vec3 to_jolt_vec3(const glm::vec3 &v) {
  return JPH::Vec3(v.x, v.y, v.z);
}

glm::vec3 from_jolt_vec3(const JPH::Vec3 &v) {
  return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
}

} // namespace Util
