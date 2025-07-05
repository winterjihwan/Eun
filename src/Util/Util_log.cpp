#include <glm/glm.hpp>
#include <iostream>
#include <string>

namespace Util {
void log_vec3(const glm::vec3 &v, const std::string &label) {
  if (!label.empty())
    std::cout << label << ": ";
  std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}
} // namespace Util
