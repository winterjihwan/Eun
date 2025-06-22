#include "Core/Timer.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_weapon_knife(float delta_time) {
  if (Input::left_mouse_pressed()) {
    stab();
  }
}

void Player::stab() {
  // Animation
  WeaponInfo *weapon_info = get_current_weapon_info();
  _weapon_view.play_animation(weapon_info->animation_names.stab);
  _weapon_action = WeaponAction::STAB;

  // Extra
  float stab_delay = 0.0f;
  TimerManager::add_timer(stab_delay, [this]() { perform_stab(); });
}
