#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_weapon_gun(float delta_time) {
  if (Input::left_mouse_down()) {
    Audio::play_audio("Pistol_fire.wav", 1.0f);
    _weapon_action = WeaponAction::FIRE;
  }
  if (Input::key_pressed(EUN_KEY_R)) {
    Audio::play_audio("Pistol_reload.wav", 1.0f);
    _weapon_action = WeaponAction::RELOAD;
  }
  if (Input::key_pressed(EUN_KEY_E)) {
    _weapon_action = WeaponAction::INSPECT;
  }
}
