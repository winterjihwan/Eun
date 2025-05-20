#include "Enums.h"
#include "Player.h"
#include "Weapon/WeaponManager.h"

void Player::update_weapon_logic(float delta_time) {
  WeaponInfo *weapon_info = get_current_weapon_info();
  // WeaponState *weapon_state = get_current_weapon_state();

  if (!weapon_info) {
    return;
  }

  if (_weapon_action == WeaponAction::DRAW) {
    // TODO: Draw animation
    _weapon_action = WeaponAction::IDLE;
  } else if (_weapon_action == WeaponAction::IDLE) {
  }
}

void Player::next_weapon() {
  _current_weapon_index++;
  if (_current_weapon_index == _weapon_states.size()) {
    _current_weapon_index = 0;
  }
  while (!_weapon_states[_current_weapon_index].has) {
    _current_weapon_index++;
    if (_current_weapon_index == _weapon_states.size()) {
      _current_weapon_index = 0;
    }
  }

  switch_weapon(_weapon_states[_current_weapon_index].name, IDLE);
}

void Player::switch_weapon(const std::string &name, WeaponAction weapon_action) {
  WeaponState *state       = get_weapon_state_by_name(name);
  WeaponInfo  *weapon_info = WeaponManager::get_weapon_info_by_name(name);

  if (!state || !weapon_info) {
    return;
  }

  for (int i = 0; i < _weapon_states.size(); i++) {
    if (_weapon_states[i].name == name) {
      _current_weapon_index = i;
    }
  }

  _weapon_action = weapon_action;
}

void Player::fire_gun() {
  // TODO: Fire gun animation
}

void Player::give_weapon(const std::string &name) {
  WeaponState *state      = get_weapon_state_by_name(name);
  WeaponInfo  *weaponInfo = WeaponManager::get_weapon_info_by_name(name);
  if (state && weaponInfo) {
    state->has = true;
  }
}

WeaponState *Player::get_weapon_state_by_name(const std::string &name) {
  for (int i = 0; i < _weapon_states.size(); i++) {
    if (_weapon_states[i].name == name) {
      return &_weapon_states[i];
    }
  }
  return nullptr;
}

WeaponState *Player::get_current_weapon_state() {
  WeaponInfo *weaponInfo = get_current_weapon_info();
  if (!weaponInfo)
    return nullptr;

  return get_weapon_state_by_name(weaponInfo->name);
}

WeaponInfo *Player::get_current_weapon_info() {
  return WeaponManager::get_weapon_info_by_name(_weapon_states[_current_weapon_index].name);
}
