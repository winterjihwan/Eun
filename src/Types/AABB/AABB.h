#pragma once

#include <Physics/Physics.h>
#include <glm/vec3.hpp>

struct AABB {
  AABB() = default;
  AABB(JPH::AABox handle);

  glm::vec3 get_center();
  glm::vec3 get_extent();

  bool contains(const glm::vec3 &point);
  bool expanded_contains(glm::vec3 point, float radius);

private:
  JPH::AABox _handle;
};
