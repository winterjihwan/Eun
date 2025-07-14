#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) {
  _view_width   = right - left;
  _aspect_ratio = (right - left) / (top - bottom);
}

void OrthoCamera::set_position(const glm::vec3 &position) {
  _position = position;
}

void OrthoCamera::move(const glm::vec3 &delta) {
  _position += delta;
}

glm::mat4 OrthoCamera::view() const {
  return glm::lookAt(_position, _position + glm::vec3(0.0f, -1.0f, 0.0f), _up);
}

glm::mat4 OrthoCamera::projection() const {
  float half_width  = _zoom_level;
  float half_height = _zoom_level / _aspect_ratio;
  return glm::ortho(-half_width, half_width, -half_height, half_height, _near, _far);
}

glm::vec3 OrthoCamera::get_position() const {
  return _position;
}
