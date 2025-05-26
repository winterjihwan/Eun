#include "Util.h"

namespace Util {
JPH::Vec3 to_jolt_vec3(const glm::vec3 &v) {
  return JPH::Vec3(v.x, v.y, v.z);
}

} // namespace Util
