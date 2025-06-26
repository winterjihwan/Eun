#pragma once

#include <glm/vec3.hpp>

struct AABB {
  AABB() = default;
  AABB(glm::vec3 min, glm::vec3 max);
  void  grow(AABB &b);
  void  grow(glm::vec3 p);
  float area();
  bool  intersects_sphere(const glm::vec3 &sphere_center, float radius) const;
  bool  intersects_AABB(const AABB &other) const;
  bool  intersects_AABB(const AABB &other, float threshold) const;
  bool  contains_point(glm::vec3 point) const;

  const glm::vec3 get_center() const {
    return center;
  }

  const glm::vec3 get_bounds_min() const {
    return bounds_min;
  }

  const glm::vec3 get_bounds_max() const {
    return bounds_max;
  }

  const glm::vec3 GetExtents() const {
    return extents;
  }

private:
  void      calculate_center_and_extents();
  glm::vec3 extents    = glm::vec3(0);
  glm::vec3 center     = glm::vec3(0);
  glm::vec3 bounds_min = glm::vec3(1e30f);
  glm::vec3 bounds_max = glm::vec3(-1e30f);
};
