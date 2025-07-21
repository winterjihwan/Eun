#include "AABB.h"
#include "Util/Util.h"

AABB::AABB(JPH::AABox handle) {
  _handle = handle;
}

glm::vec3 AABB::get_center() {
  glm::vec3 center = Util::to_vec3(_handle.GetCenter());
  return center;
}

glm::vec3 AABB::get_extent() {
  glm::vec3 extent = Util::to_vec3(_handle.GetExtent());
  return extent;
}

bool AABB::contains(const glm::vec3 &point) {
  JPH::Vec3 jolt_point = Util::to_jolt_vec3(point);
  return _handle.Contains(jolt_point);
}

bool AABB::expanded_contains(glm::vec3 point, float radius) {
  JPH::Vec3  jolt_point = Util::to_jolt_vec3(point);
  JPH::AABox expanded   = _handle;
  expanded.ExpandBy(JPH::Vec3(radius, radius, radius));
  return expanded.Contains(jolt_point);
}
