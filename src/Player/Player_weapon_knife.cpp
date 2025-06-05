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

  float stab_delay = 0.4f;
  TimerManager::add_timer(stab_delay, [this]() { perform_stab(); });
  _weapon_action = WeaponAction::STAB;
}
