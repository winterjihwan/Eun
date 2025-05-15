#include "Defines.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"
#include <glm/glm.hpp>

void Player::update_movement(float delta_time, Camera camera) {
  float     velocity = _speed * delta_time;
  glm::vec3 front    = camera.get_front();
  front.y            = 0.0f;
  glm::vec3 right    = camera.get_right();

  glm::vec3 displacement = glm::vec3(0);

  glm::vec3 walk = glm::vec3(0);
  if (Input::key_down(SFPS_KEY_W)) {
    walk += front;
  }
  if (Input::key_down(SFPS_KEY_S)) {
    walk -= front;
  }
  if (Input::key_down(SFPS_KEY_A)) {
    walk -= right;
  }
  if (Input::key_down(SFPS_KEY_D)) {
    walk += right;
  }

  walk = glm::length(walk) > 0.0f ? glm::normalize(walk) : glm::vec3(0.0f);
  displacement += walk;

  if (_on_ground) {
    _y_velocity = -3.5f;
  } else {
    _y_velocity -= GRAVITY * delta_time;
  }
  // displacement.y += _y_velocity * delta_time;

  _position += displacement * velocity;
}
