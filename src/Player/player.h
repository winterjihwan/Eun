#pragma once

#include "Camera/Camera.h"
// TODO: Remove the shit out of this
#include "Jolt/Jolt.h"
#include <glm/glm.hpp>

// TODO: Remove the shit out of this
#include "Jolt/Physics/Body/BodyID.h"

struct Player {
  void init(glm::vec3 position);
  void update(float delta_time, Camera camera);

  void update_movement(float delta_time, Camera camera);
  void check_on_ground();

  void update_flashlight();

  glm::vec3 get_pos();
  bool      get_flashlight_on();

private:
  float       _speed = 5.0f;
  glm::vec3   _position;
  float       _y_velocity    = 0.0f;
  bool        _on_ground     = false;
  bool        _flashlight_on = false;
  JPH::BodyID _body_id;
};
