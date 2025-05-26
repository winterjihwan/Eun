#include "Util.h"

namespace Util {
float random_float(float min, float max) {
  float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return min + r * (max - min);
}

} // namespace Util
