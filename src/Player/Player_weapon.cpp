#include "AssetManager/AssetManager.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"
#include "Weapon/WeaponManager.h"

void Player::update_weapon(float delta_time) {
  if (Input::key_pressed(EUN_KEY_Q)) {
    next_weapon();
  }

  WeaponInfo *weapon_info = get_current_weapon_info();
  // WeaponState *weapon_state = get_current_weapon_state();

  if (!weapon_info) {
    return;
  }

  switch (weapon_info->type) {
  case WeaponType::PISTOL:
  case WeaponType::AUTOMATIC:
    update_weapon_gun(delta_time);
    break;
  default:;
  };

  if (_weapon_action == WeaponAction::DRAW) {
    _weapon_view_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.draw);
    if (_weapon_view_animator->IsDone()) {
      _weapon_action = WeaponAction::IDLE;
    }

  } else if (_weapon_action == WeaponAction::FIRE) {
    _weapon_view_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.fire);
    if (_weapon_view_animator->IsDone()) {
      _weapon_action = WeaponAction::IDLE;
    }

  } else if (_weapon_action == WeaponAction::RELOAD) {
    _weapon_view_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.reload);
    if (_weapon_view_animator->IsDone()) {
      _weapon_action = WeaponAction::IDLE;
    }

  } else if (_weapon_action == WeaponAction::INSPECT) {
    _weapon_view_animator =
        AssetManager::get_animator_by_name(weapon_info->animation_names.inspect);
    if (_weapon_view_animator->IsDone()) {
      _weapon_action = WeaponAction::IDLE;
    }

  } else if (_weapon_action == WeaponAction::IDLE) {
    _weapon_view_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
  }

  _weapon_view_animator->UpdateAnimation(delta_time);
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

  switch_weapon(_weapon_states[_current_weapon_index].name, DRAW);
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

Animator *Player::get_weapon_view_animator() {
  return _weapon_view_animator;
}
