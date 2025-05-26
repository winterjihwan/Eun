#include "Player.h"
#include "Defines.h"
#include "Keycodes.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position  = position;
  _character = create_character_virtual(
      glm::vec3(_position.x, _position.y, _position.z), PLAYER_HEIGHT, 0.25f);

  init_weapon();
}

void Player::update(float delta_time, Camera camera) {
  update_movement(delta_time, camera);
  update_weapon(delta_time);
  update_flashlight();
}

glm::vec3 Player::get_pos() {
  RVec3     pos    = _character->GetPosition();
  glm::vec3 result = glm::vec3(pos.GetX(), pos.GetY() + PLAYER_HEIGHT * 0.5f, pos.GetZ());

  // std::cout << "[Player::get_pos] Position = (" << result.x << ", " << result.y << ", " <<
  // result.z
  //           << ")" << std::endl;

  return result;
}
