#pragma once

#include "Camera/Camera.h"
#include "Enums.h"
#include "Physics/Physics.h"
#include "Types/Game/AnimEntity.h"
#include "Weapon/WeaponCommon.h"
#include <glm/glm.hpp>

struct Player {
  void init(glm::vec3 position);
  void update(float delta_time, Camera camera);

  void update_movement(float delta_time, Camera camera);

  JPH::CharacterVirtual *create_character_virtual(glm::vec3 position, float height, float radius);

  glm::vec3 get_pos();

  // Flashlight
  void update_flashlight();
  bool get_flashlight_on();

  // Weapon
  void         init_weapon();
  void         update_weapon(float delta_time);
  void         update_weapon_gun(float delta_time);
  void         switch_weapon(const std::string &name);
  void         next_weapon();
  void         fire_gun();
  void         give_weapon(const std::string &name);
  WeaponState *get_weapon_state_by_name(const std::string &name);
  WeaponState *get_current_weapon_state();
  WeaponInfo  *get_current_weapon_info();

private:
  JPH::CharacterVirtual *_character;
  float                  _speed = 5.0f;
  glm::vec3              _position;
  float                  _y_velocity    = 0.0f;
  bool                   _flashlight_on = false;

  // Weapon
  int                      _current_weapon_index = 0;
  std::vector<WeaponState> _weapon_states;
  WeaponAction             _weapon_action;
  AnimEntity              *_weapon_anim_entity;
};
