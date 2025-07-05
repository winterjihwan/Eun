#include "Util.h"

namespace Util {
float random_float(float min, float max) {
  float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return min + r * (max - min);
}

glm::vec3 random_dir_in_hemisphere(const glm::vec3 &normal) {
  glm::vec3 rand =
      glm::vec3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
  rand = normalize(rand);
  return dot(rand, normal) > 0 ? rand : -rand;
}

glm::mat4 lerp(const glm::mat4 &a, const glm::mat4 &b, float factor) {
  glm::mat4 result(0.0f);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      result[i][j] = glm::mix(a[i][j], b[i][j], factor);
  return result;
}

glm::mat4
transform(const glm::vec3 &position, const glm::vec3 &rotation_rad, const glm::vec3 &scale) {
  glm::mat4 transform = glm::mat4(1.0f);
  transform           = glm::translate(transform, position);
  transform *= glm::eulerAngleXYZ(rotation_rad.x, rotation_rad.y, rotation_rad.z);
  transform = glm::scale(transform, scale);
  return transform;
}

glm::vec3
to_capsule_position(const glm::vec3 &base_position, float capsule_height, float capsule_radius) {
  return glm::vec3(base_position.x,
                   base_position.y + (capsule_height + 2.0f * capsule_radius) * 0.5f,
                   base_position.z);
}

glm::vec3
to_base_position(const glm::vec3 &capsule_position, float capsule_height, float capsule_radius) {
  return glm::vec3(capsule_position.x,
                   capsule_position.y - (capsule_height + 2.0f * capsule_radius) * 0.5f,
                   capsule_position.z);
}
} // namespace Util
