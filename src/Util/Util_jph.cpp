#include "Util.h"

namespace Util {
JPH::Vec3 to_jolt_vec3(const glm::vec3 &v) {
  return JPH::Vec3(v.x, v.y, v.z);
}

JPH::Quat to_jolt_quat(const glm::quat &q) {
  return JPH::Quat(q.x, q.y, q.z, q.w);
}

glm::vec3 to_vec3(const JPH::Vec3 &v) {
  return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
}

JPH::ShapeRefC generate_capsule_shape(float capsule_height, float capsule_radius) {
  JPH::CapsuleShapeSettings shape_settings(capsule_height * 0.5f, capsule_radius);
  return shape_settings.Create().Get();
}

} // namespace Util
