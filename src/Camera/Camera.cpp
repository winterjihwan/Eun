#include "Camera.h"

void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch) {
  xoffset *= _mouse_sensitivity;
  yoffset *= _mouse_sensitivity;

  _yaw += xoffset;
  _pitch += yoffset;

  if (constrainPitch) {
    if (_pitch > 89.0f)
      _pitch = 89.0f;
    if (_pitch < -89.0f)
      _pitch = -89.0f;
  }

  update_camera_vectors();
}

void Camera::process_mouse_scroll(float yoffset) {
  _zoom -= (float)yoffset;
  if (_zoom < 1.0f)
    _zoom = 1.0f;
  if (_zoom > 45.0f)
    _zoom = 45.0f;
}

glm::vec3 Camera::get_front() {
  return _front;
}

glm::vec3 Camera::get_right() {
  return _right;
}

glm::vec3 Camera::get_up() {
  return _up;
}

float Camera::get_zoom() {
  return _zoom;
}
