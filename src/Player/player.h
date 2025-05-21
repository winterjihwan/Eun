#pragma once

#include "Camera/Camera.h"
#include "Enums.h"
#include "Physics/Physics.h"
#include "Types/Animation/Animator.h"
#include "Weapon/WeaponCommon.h"
#include <glm/glm.hpp>

struct Player {
  void init(glm::vec3 position);
  void update(float delta_time, Camera camera);

  void update_movement(float delta_time, Camera camera);

  BodyID create_player_physics(glm::vec3 position, float height, float radius, ObjectLayer layer);

  void update_flashlight();

  glm::vec3 get_pos();
  bool      get_flashlight_on();

  // Weapon
  void         update_weapon(float delta_time);
  void         update_weapon_gun(float delta_time);
  void         switch_weapon(const std::string &name, WeaponAction weaponAction);
  void         next_weapon();
  void         fire_gun();
  void         give_weapon(const std::string &name);
  WeaponState *get_weapon_state_by_name(const std::string &name);
  WeaponState *get_current_weapon_state();
  WeaponInfo  *get_current_weapon_info();
  Animator    *get_weapon_view_animator();

private:
  float       _speed = 5.0f;
  glm::vec3   _position;
  float       _y_velocity    = 0.0f;
  bool        _on_ground     = false;
  bool        _flashlight_on = false;
  JPH::BodyID _body_id;

  // Weapon
  int                      _current_weapon_index = 0;
  std::vector<WeaponState> _weapon_states;
  WeaponAction             _weapon_action;
  Animator                *_weapon_view_animator; // HACK
};
