#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_flashlight() {
  if (Input::key_pressed(EUN_KEY_F)) {
    _flashlight_on = !_flashlight_on;
  }

  if (_flashlight_on) {
  }
}

bool Player::get_flashlight_on() {
  return _flashlight_on;
}
