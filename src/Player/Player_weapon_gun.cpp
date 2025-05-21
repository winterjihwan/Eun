#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_weapon_gun(float delta_time) {
  if (Input::left_mouse_pressed()) {
    _weapon_action = WeaponAction::FIRE;
  }
}
