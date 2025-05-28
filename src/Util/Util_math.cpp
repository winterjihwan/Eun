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

} // namespace Util
