#include "Util.h"
#include <iomanip>

namespace Util {
std::string to_string(TextureType type) {
  switch (type) {
  case TextureType::DIFFUSE:
    return "texture_diffuse";
  case TextureType::SPECULAR:
    return "texture_specular";
  case TextureType::NORMAL:
    return "texture_normal";
  case TextureType::HEIGHT:
    return "texture_height";
  }
}

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
} // namespace Util
