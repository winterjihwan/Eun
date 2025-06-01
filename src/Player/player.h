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
  void init_weapon();
  void switch_weapon(const std::string &name);
  void next_weapon();
  void reload_gun();
  void fire_gun();
  void inspect_gun();
  void give_weapon(const std::string &name);
  void spawn_bullet(float variance);

  WeaponState *get_weapon_state_by_name(const std::string &name);
  WeaponState *get_current_weapon_state();
  WeaponInfo  *get_current_weapon_info();

private:
  AnimEntity     *_player_anim_entity;
  PlayerAnimators _player_animators;
  PlayerState     _player_state = PlayerState::IDLE;

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
  int                      _current_weapon_index = 0;
  std::vector<WeaponState> _weapon_states;
  WeaponAction             _weapon_action;
  // AnimEntity              *_weapon_anim_entity;

  void update_weapon(float delta_time);
  void update_weapon_gun(float delta_time);

  glm::mat4 weapon_view_transform();
};
