#include "player.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position = position;
}

void Player::pressed_move_forward(float delta_time, Camera camera) {
  float     velocity = _speed * delta_time;
  glm::vec3 front    = camera.Front;
  front.y            = 0.0f;
  _position += front * velocity;
}

void Player::pressed_move_backward(float delta_time, Camera camera) {
  float     velocity = _speed * delta_time;
  glm::vec3 front    = camera.Front;
  front.y            = 0.0f;
  _position -= front * velocity;
}

void Player::pressed_move_left(float delta_time, Camera camera) {
  float velocity = _speed * delta_time;
  _position -= camera.Right * velocity;
}

void Player::pressed_move_right(float delta_time, Camera camera) {
  float velocity = _speed * delta_time;
  _position += camera.Right * velocity;
}

glm::vec3 Player::get_pos() {
  return _position;
}
