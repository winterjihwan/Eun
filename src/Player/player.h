#pragma once

#include "Camera/Camera.h"
#include <glm/glm.hpp>

struct Player {
  void init(glm::vec3 position);
  void update(float delta_time, Camera camera);

  void update_movement(float delta_time, Camera camera);
  void update_flashlight();

  glm::vec3 get_pos();
  bool      get_flashlight_on();

private:
  float     _speed = 5.0f;
  glm::vec3 _position;
  float     _y_velocity    = -3.5f;
  bool      _on_ground     = false;
  bool      _flashlight_on = false;
};
