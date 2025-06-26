#include "Camera.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"

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

void Camera::update(const glm::vec3 &watch) {
  if (Input::key_pressed(EUN_KEY_P)) {
    toggle_view(watch);
  }

  if (_view_mode == ViewMode::FIRST) {
    first_person_view(watch);
    _view = glm::lookAt(watch, watch + _front, _up);
  } else {
    third_person_view(watch);
    _view = glm::lookAt(_position, watch, _up);
  }
}

void Camera::toggle_view(const glm::vec3 &watch) {
  if (_view_mode == ViewMode::FIRST) {
    _view_mode = ViewMode::THIRD;
  } else {
    _view_mode = ViewMode::FIRST;
  }
}

void Camera::first_person_view(const glm::vec3 &watch) {
  _position = watch;
}

void Camera::third_person_view(const glm::vec3 &watch) {
  float distance_behind = 5.0f;
  float height_above    = 0.5f;

  glm::vec3 offset = -get_front() * distance_behind + glm::vec3(0.0f, height_above, 0.0f);
  _position        = watch + offset;
}

glm::mat4 Camera::view_matrix() {
  return _view;
}

glm::mat4 Camera::projection() {
  return glm::perspective(
      glm::radians(get_zoom()), (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT, NEAR, FAR);
}

glm::vec3 Camera::get_pos() {
  return _position;
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

float Camera::get_yaw() {
  // return glm::degrees(atan2(_front.z, _front.x));
  return _yaw;
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
