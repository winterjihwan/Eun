#pragma once

#include "Camera/Camera.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Types/Game/AnimEntity.h"
#include "Weapon/WeaponCommon.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <glm/glm.hpp>

struct Player {
  void init(glm::vec3 position);
  void update(float delta_time, Camera camera);

  JPH::CharacterVirtual *create_character_virtual(glm::vec3 position, float height, float radius);

  glm::vec3 get_pos();
  glm::vec3 get_pos_feet();

  // Flashlight
  bool get_flashlight_on();

  // Weapon
  WeaponState *get_weapon_state_by_name(const std::string &name);
  WeaponState *get_current_weapon_state();
  WeaponInfo  *get_current_weapon_info();

private:
  AnimEntity      *_player_anim_entity;
  Animator         _player_animator;
  PlayerAnimations _player_animations;
  PlayerState      _player_state = PlayerState::IDLE;

  JPH::CharacterVirtual *_character;
  float                  _speed = 5.0f;
  glm::vec3              _position;
  float                  _y_velocity = 0.0f;

  // Player
  glm::mat4 player_view_transform();
  void      update_movement(float delta_time, Camera camera);
  void      update_anim_entity();

  // Flashlight
  bool _flashlight_on = false;

  void update_flashlight();

  // Weapon
  WeaponType               _current_weapon_type  = WeaponType::HAND;
  int                      _current_weapon_index = 0;
  std::vector<WeaponState> _weapon_states;
  WeaponAction             _weapon_action = WeaponAction::IDLE;

  void init_weapon();
  void update_weapon(float delta_time);
  void update_weapon_gun(float delta_time);
  void update_weapon_knife(float delta_time);
  void equip_weapon(WeaponInfo *weapon_info);
  void switch_weapon(const std::string &name);
  void next_weapon();
  void stab();
  void reload_gun();
  void fire_gun();
  void inspect_gun();
  void give_weapon(const std::string &name);
  void spawn_bullet(float variance);
};
