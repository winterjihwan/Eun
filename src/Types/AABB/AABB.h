#pragma once

#include <Physics/Physics.h>
#include <glm/vec3.hpp>

struct AABB {
  AABB() = default;
  AABB(JPH::AABox handle);

  glm::vec3 get_center();
  glm::vec3 get_extent();

private:
  JPH::AABox _handle;
};
