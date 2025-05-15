#include "Player.h"
#include "Keycodes.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position = position;
}

void Player::update(float delta_time, Camera camera) {
  update_movement(delta_time, camera);
  update_flashlight();
}

glm::vec3 Player::get_pos() {
  return _position;
}
