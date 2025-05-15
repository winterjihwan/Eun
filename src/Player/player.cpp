#include "Player.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position = position;
}

void Player::update(float delta_time, Camera camera) {
  float     velocity = _speed * delta_time;
  glm::vec3 front    = camera.Front;
  front.y            = 0.0f;
  glm::vec3 right    = camera.Right;

  glm::vec3 displacement = glm::vec3(0);

  if (Input::key_down(SFPS_KEY_W)) {
    displacement += front;
  }
  if (Input::key_down(SFPS_KEY_S)) {
    displacement -= front;
  }
  if (Input::key_down(SFPS_KEY_A)) {
    displacement -= right;
  }
  if (Input::key_down(SFPS_KEY_D)) {
    displacement += right;
  }

  if (glm::length(displacement) > 0.0f) {
    displacement = glm::normalize(displacement);
    _position += displacement * velocity;
  }
}

glm::vec3 Player::get_pos() {
  return _position;
}
