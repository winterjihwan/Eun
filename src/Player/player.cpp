#include "Player.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types/UID/UID.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position  = position;
  _character = Physics::create_character_virtual(
      _position, PLAYER_HEIGHT, 0.15f, ObjectType::PLAYER, UID::get_next());
  _weapon_view.set_name("Weapon_View");
  _weapon_view.set_scale(glm::vec3(.01f, .01f, .01f));

  init_weapon();
}

void Player::update(float delta_time, Camera camera) {
  if (!Game::is_game_mode(GameMode::TPS)) {
    return;
  }

  update_ui();
  update_movement(delta_time, camera);
  update_feet_interaction(delta_time);
  update_weapon(delta_time);
  update_flashlight();
  update_weapon_view(delta_time);
}

glm::vec3 Player::get_pos() {
  JPH::RVec3 pos    = _character->GetPosition();
  glm::vec3  result = glm::vec3(pos.GetX(), pos.GetY() + PLAYER_HEIGHT * 0.5f, pos.GetZ());

  return result;
}

glm::vec3 Player::get_pos_feet() {
  return _position - glm::vec3(0, PLAYER_HEIGHT, 0);
}

glm::mat4 Player::player_view_transform() {
  glm::vec3 pos = get_pos_feet();
  float     yaw = Game::get_camera()->get_yaw();

  glm::mat4 transform = glm::mat4(1.0f);
  transform           = glm::translate(transform, pos);
  transform           = glm::rotate(transform, glm::radians(-yaw + 90.0f), glm::vec3(0, 1, 0));

  return transform;
}
