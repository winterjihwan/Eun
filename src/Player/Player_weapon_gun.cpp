#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_weapon_gun(float delta_time) {
  if (Input::left_mouse_pressed()) {
    fire_gun();
  }

  if (Input::key_pressed(EUN_KEY_R)) {
    reload_gun();
  }
}

void Player::reload_gun() {
  WeaponInfo *weapon_info = get_current_weapon_info();

  Audio::play_audio(weapon_info->audio_files.reload, 1.0f);
  // TODO: Check if can reload
  _weapon_action = WeaponAction::RELOAD;
}

void Player::fire_gun() {
  WeaponInfo *weapon_info = get_current_weapon_info();

  Audio::play_audio(weapon_info->audio_files.fire, 1.0f);

  _weapon_action = WeaponAction::FIRE;

  spawn_bullet(0.02f);
}
