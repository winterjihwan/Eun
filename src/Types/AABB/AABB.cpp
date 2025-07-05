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
