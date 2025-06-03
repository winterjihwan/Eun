#include "HotReload.h"
#include "Types.h"
#include <fstream>
#include <string>

constexpr std::string s_reload_path = "hot_reload.json";

namespace HotReload {
static std::unordered_map<std::string, TransformConfig> configs;

void init() {
  hot_reload();
}

void hot_reload(const std::string &path) {
  if (!std::filesystem::exists(s_reload_path)) {
    return;
  }

  std::ifstream f(s_reload_path);
  if (!f.is_open())
    return;

  nlohmann::json json;
  f >> json;

  configs.clear();

  // Process TransformConfig
  for (auto &[name, cfg] : json.items()) {
    TransformConfig t;
    if (cfg.contains("translate"))
      t.translate = glm::vec3(cfg["translate"][0], cfg["translate"][1], cfg["translate"][2]);
    if (cfg.contains("rotate_deg"))
      t.rotate_deg = glm::vec3(cfg["rotate_deg"][0], cfg["rotate_deg"][1], cfg["rotate_deg"][2]);
    if (cfg.contains("scale"))
      t.scale = glm::vec3(cfg["scale"][0], cfg["scale"][1], cfg["scale"][2]);

    configs[name] = t;
  }
}

glm::mat4 get_transform(const std::string &name) {
  auto it = configs.find(name);
  if (it == configs.end())
    return glm::mat4(1.0f);

  const TransformConfig &cfg = it->second;
  glm::mat4              m   = glm::translate(glm::mat4(1.0f), cfg.translate);
  m                          = glm::rotate(m, glm::radians(cfg.rotate_deg.x), glm::vec3(1, 0, 0));
  m                          = glm::rotate(m, glm::radians(cfg.rotate_deg.y), glm::vec3(0, 1, 0));
  m                          = glm::rotate(m, glm::radians(cfg.rotate_deg.z), glm::vec3(0, 0, 1));
  m                          = glm::scale(m, cfg.scale);
  return m;
}

glm::vec3 get_translate(const std::string &name) {
  auto it = configs.find(name);
  return it != configs.end() ? it->second.translate : glm::vec3(0.0f);
}

glm::vec3 get_rotation_deg(const std::string &name) {
  auto it = configs.find(name);
  return it != configs.end() ? it->second.rotate_deg : glm::vec3(0.0f);
}

glm::vec3 get_scale(const std::string &name) {
  auto it = configs.find(name);
  return it != configs.end() ? it->second.scale : glm::vec3(1.0f);
}

} // namespace HotReload
