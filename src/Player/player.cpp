#include "Player.h"
#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Weapon/WeaponCommon.h"
#include "World/World.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position  = position;
  _character = create_character_virtual(
      glm::vec3(_position.x, _position.y, _position.z), PLAYER_HEIGHT, 0.15f);

  // Animations
  _player_animations.idle              = AssetManager::get_animation_by_name("Breathe_Idle");
  _player_animations.walk_forward      = AssetManager::get_animation_by_name("Walk_Forward");
  _player_animations.walk_backward     = AssetManager::get_animation_by_name("Walk_Backward");
  _player_animations.walk_left         = AssetManager::get_animation_by_name("Walk_Left");
  _player_animations.walk_right        = AssetManager::get_animation_by_name("Walk_Right");
  _player_animations.jump              = AssetManager::get_animation_by_name("Jump");
  _player_animations.knife_idle        = AssetManager::get_animation_by_name("Knife_Idle");
  _player_animations.knife_stab        = AssetManager::get_animation_by_name("Knife_Stab");
  _player_animations.gun_idle          = AssetManager::get_animation_by_name("Gun_Idle");
  _player_animations.gun_fire          = AssetManager::get_animation_by_name("Gun_Fire");
  _player_animations.gun_draw          = AssetManager::get_animation_by_name("Gun_Draw");
  _player_animations.gun_walk_forward  = AssetManager::get_animation_by_name("Gun_Walk_Forward");
  _player_animations.gun_walk_backward = AssetManager::get_animation_by_name("Gun_Walk_Backward");
  _player_animations.gun_walk_left     = AssetManager::get_animation_by_name("Gun_Walk_Left");
  _player_animations.gun_walk_right    = AssetManager::get_animation_by_name("Gun_Walk_Right");
  _player_animations.gun_jump          = AssetManager::get_animation_by_name("Gun_Jump");

  // Player Anim Entity
  AnimEntityCreateInfo anim_entity_create_info;
  anim_entity_create_info.name      = "Player";
  anim_entity_create_info.model     = AssetManager::get_model_by_name("Brian");
  anim_entity_create_info.animator  = &_player_animator;
  anim_entity_create_info.transform = glm::translate(glm::mat4(1.0f), _position);

  AnimEntity player_entity;
  player_entity.init(std::move(anim_entity_create_info));
  _player_anim_entity = World::add_anim_entity(std::move(player_entity));

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
      animation = _player_animations.knife_idle;
    } else {
      animation = _player_animations.gun_idle;
    };
    break;

  case PlayerState::WALKING_FORWARD:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.gun_walk_forward
                                                              : _player_animations.walk_forward;
    break;

  case PlayerState::WALKING_BACKWARD:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.gun_walk_backward
                                                              : _player_animations.walk_backward;
    break;

  case PlayerState::WALKING_LEFT:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.gun_walk_left
                                                              : _player_animations.walk_left;
    break;

  case PlayerState::WALKING_RIGHT:
    animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.gun_walk_right
                                                              : _player_animations.walk_right;
    break;

  case PlayerState::JUMPING:
    // TODO: Enable jumping animation
    // animation = (_current_weapon_type == WeaponType::HANDGUN) ? _player_animations.gun_jump
    //                                                           : _player_animations.jump;
    break;

  case PlayerState::STAB:
    animation = _player_animations.knife_stab;

    break;
  }

  if (animation) {
    _player_anim_entity->play_animation(animation);
  }

  _player_anim_entity->set_model_transform(player_view_transform());
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
