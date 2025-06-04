#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_weapon_knife(float delta_time) {
  if (Input::left_mouse_pressed()) {
    stab();
  }
}

void Player::stab() {
  WeaponInfo *weapon_info = get_current_weapon_info();

  Audio::play_audio(weapon_info->audio_files.stab, 1.0f);
  _weapon_action = WeaponAction::STAB;

  perform_stab();
}
