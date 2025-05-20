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

void Camera::update_camera_vectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
  front.y = sin(glm::radians(_pitch));
  front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
  _front  = glm::normalize(front);
  _right  = glm::normalize(glm::cross(_front, _world_up));
  _up     = glm::normalize(glm::cross(_right, _front));
}
