#include "Util.h"
#include <iomanip>

namespace Util {
// HACK
std::string to_string(const std::string &name, const glm::mat4 &mat) {
  std::ostringstream oss;
  oss << name << " =\n";
  oss << std::fixed << std::setprecision(3);
  for (int i = 0; i < 4; ++i) {
    oss << "[ ";
    for (int j = 0; j < 4; ++j) {
      oss << std::setw(8) << mat[j][i] << " ";
    }
    oss << "]\n";
  }
  return oss.str();
}

std::string to_string(ObjectType object_type) {
  switch (object_type) {
  case ObjectType::NONE:
    return "NONE";
  case ObjectType::MAP:
    return "MAP";
  case ObjectType::PLATFORM:
    return "PLATFORM";
  case ObjectType::GAME_OBJECT:
    return "GAME_OBJECT";
  case ObjectType::BUILDING:
    return "BUILDING";
  case ObjectType::ENHANCE:
    return "ENHANCE";
  case ObjectType::DECAL:
    return "DECAL";
  case ObjectType::UNIT:
    return "UNIT";
  default:
    assert(0);
  }
}
} // namespace Util
