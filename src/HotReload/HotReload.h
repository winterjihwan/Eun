#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>
#include <string>

/* TransformConfig Usage
{
"glock_offset": {
"translate": [0.0, 0.0, 0.1],
"rotate_deg": [90.0, 0, 0, 1],
"scale": [100.0, 100.0, 100.0]
},
}
*/

namespace HotReload {

void init();
void hot_reload(const std::string &path = "hot_reload.json");

glm::mat4 get_transform(const std::string &name);
glm::vec3 get_translate(const std::string &name);
glm::vec3 get_rotation_deg(const std::string &name);
glm::vec3 get_scale(const std::string &name);

} // namespace HotReload
