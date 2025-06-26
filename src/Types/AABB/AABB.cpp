#include "AABB.h"
#include <algorithm>

AABB::AABB(glm::vec3 min, glm::vec3 max) {
  bounds_min = min;
  bounds_max = max;
  calculate_center_and_extents();
}
void AABB::grow(AABB &b) {
  if (b.bounds_min.x != 1e30f && b.bounds_min.x != -1e30f) {
    grow(b.bounds_min);
    grow(b.bounds_max);
  }
  AABB::calculate_center_and_extents();
}
void AABB::grow(glm::vec3 p) {
  bounds_min = glm::vec3(
      std::min(bounds_min.x, p.x), std::min(bounds_min.y, p.y), std::min(bounds_min.z, p.z));
  bounds_max = glm::vec3(
      std::max(bounds_max.x, p.x), std::max(bounds_max.y, p.y), std::max(bounds_max.z, p.z));
  calculate_center_and_extents();
}
float AABB::area() {
  glm::vec3 e = bounds_max - bounds_min;
  return e.x * e.y + e.y * e.z + e.z * e.x;
}

void AABB::calculate_center_and_extents() {
  center  = {(bounds_min.x + bounds_max.x) / 2,
             (bounds_min.y + bounds_max.y) / 2,
             (bounds_min.z + bounds_max.z) / 2};
  extents = bounds_max - bounds_min;
}

bool AABB::contains_point(glm::vec3 point) const {
  return (point.x >= bounds_min.x && point.x <= bounds_max.x) &&
         (point.y >= bounds_min.y && point.y <= bounds_max.y) &&
         (point.z >= bounds_min.z && point.z <= bounds_max.z);
}

bool AABB::intersects_AABB(const AABB &other) const {
  return (bounds_min.x <= other.bounds_max.x && bounds_max.x >= other.bounds_min.x) &&
         (bounds_min.y <= other.bounds_max.y && bounds_max.y >= other.bounds_min.y) &&
         (bounds_min.z <= other.bounds_max.z && bounds_max.z >= other.bounds_min.z);
}

bool AABB::intersects_AABB(const AABB &other, float threshold) const {
  glm::vec3 inflated_min_a = bounds_min - glm::vec3(threshold);
  glm::vec3 inflated_max_a = bounds_max + glm::vec3(threshold);
  glm::vec3 inflated_min_b = other.bounds_min - glm::vec3(threshold);
  glm::vec3 inflated_max_b = other.bounds_max + glm::vec3(threshold);

  return (inflated_min_a.x <= inflated_max_b.x && inflated_max_a.x >= inflated_min_b.x) &&
         (inflated_min_a.y <= inflated_max_b.y && inflated_max_a.y >= inflated_min_b.y) &&
         (inflated_min_a.z <= inflated_max_b.z && inflated_max_a.z >= inflated_min_b.z);
}
