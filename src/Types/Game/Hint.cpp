#include "Hint.h"
#include "Defines.h"
#include "UI/UIBackend.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Hint::init(const std::string &label, std::function<std::string()> text_callback) {
  _label         = label;
  _text_callback = text_callback;
}

void Hint::set_position(const glm::vec3 &world_position) {
  _world_position = world_position;
}

void Hint::set_visible(bool visible) {
  _visible = visible;
}

void Hint::update(const glm::mat4 &view, const glm::mat4 &projection) {
  glm::vec4 clip = projection * view * glm::vec4(_world_position, 1.0f);
  if (clip.w <= 0.0f) {
    _visible = false;
    return;
  }

  glm::vec3 ndc = glm::vec3(clip) / clip.w;

  _screen_position = glm::vec2((ndc.x * 0.5f + 0.5f) * VIEWPORT_WIDTH, 200.0f);
}

void Hint::render() {
  if (!_visible || !_text_callback)
    return;

  std::string text = _label + "  " + _text_callback();

  UIBackend::blit_text(text,
                       "AncizarSerif",
                       static_cast<int>(_screen_position.x),
                       static_cast<int>(_screen_position.y),
                       UIAlignment::CENTERED,
                       0.5f,
                       glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));
}
