#include "Player.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Weapon/WeaponCommon.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position  = position;
  _character = create_character_virtual(
      glm::vec3(_position.x, _position.y, _position.z), PLAYER_HEIGHT, 0.15f);

  init_weapon();
}

void Player::update(float delta_time, Camera camera) {
  update_movement(delta_time, camera);
  update_weapon(delta_time);
  update_flashlight();
  update_anim_entity();
}

void Player::update_anim_entity() {
  Animation *animation = 0;

  switch (_player_state) {
  case PlayerState::IDLE:
    if (_current_weapon_type == WeaponType::HAND) {
      animation = _player_animations.idle;
    } else if (_current_weapon_type == WeaponType::KNIFE) {
      animation = _player_animations.idle_knife;
    } else {
      animation = _player_animations.idle;
    };
    break;

  case PlayerState::WALKING_FORWARD:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.walk
                                                              : _player_animations.walk;
    break;

  case PlayerState::WALKING_BACKWARD:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.walk
                                                              : _player_animations.walk;
    break;

  case PlayerState::WALKING_LEFT:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.walk
                                                              : _player_animations.walk;
    break;

  case PlayerState::WALKING_RIGHT:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.walk
                                                              : _player_animations.walk;
    break;

  case PlayerState::JUMPING:
    // TODO: Enable jumping animation
    // animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.gun_jump
    //                                                           : _player_animations.jump;
    break;

  case PlayerState::STAB:
    animation = _player_animations.stab;

    break;
  }
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
