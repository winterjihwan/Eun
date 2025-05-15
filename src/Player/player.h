#pragma once

#include "Camera.h"
#include <glm/glm.hpp>

struct Player {
  void init(glm::vec3 position);
  void update(float delta_time, Camera camera);

  void update_flashlight() {
  }

  glm::vec3 get_pos();

private:
  float     _speed = 5.0f;
  glm::vec3 _position;
};
