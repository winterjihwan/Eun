#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <string>

struct Hint {
  Hint() = default;

  void init(const std::string &label, std::function<std::string()> _text_callback);
  void update(const glm::mat4 &view, const glm::mat4 &projection);
  void render();

  void set_position(const glm::vec3 &world_position);
  void set_visible(bool visible);

private:
  std::string                  _label;
  std::function<std::string()> _text_callback;
  glm::vec3                    _world_position = glm::vec3(0.0f);
  glm::vec2                    _screen_position;
  bool                         _visible = true;
};
