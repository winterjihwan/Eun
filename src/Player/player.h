#pragma once

#include "camera.h"
#include <glm/glm.hpp>

struct Player {
  void init(glm::vec3 position);
  void pressed_move_forward(float delta_time, Camera camera);
  void pressed_move_backward(float delta_time, Camera camera);
  void pressed_move_left(float delta_time, Camera camera);
  void pressed_move_right(float delta_time, Camera camera);

  // Getter, Setter
  glm::vec3 get_pos();

private:
  float     _speed = 5.0f;
  glm::vec3 _position;
};
