#include "Util.h"

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
} // namespace Util
