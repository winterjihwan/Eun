#include "AssetManager/AssetManager.h"
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

  if (Input::key_pressed(EUN_KEY_E)) {
    inspect_gun();
  }
}

void Player::reload_gun() {
  WeaponInfo *weapon_info = get_current_weapon_info();

  Audio::play_audio(weapon_info->audio_files.reload, 1.0f);
  // TODO: Check if can reload
  _weapon_action = WeaponAction::RELOAD;
  Animator *weapon_animator =
      AssetManager::get_animator_by_name(weapon_info->animation_names.reload);
  _weapon_anim_entity->set_animator(weapon_animator);
  weapon_animator->PlayAnimation();
}

void Player::fire_gun() {
  WeaponInfo *weapon_info = get_current_weapon_info();

  Audio::play_audio(weapon_info->audio_files.fire, 1.0f);

  _weapon_action            = WeaponAction::FIRE;
  Animator *weapon_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.fire);
  _weapon_anim_entity->set_animator(weapon_animator);
  weapon_animator->PlayAnimation();

  // Actual fire logic

  spawn_bullet(0.5f);
}

void Player::inspect_gun() {
  WeaponInfo *weapon_info = get_current_weapon_info();

  _weapon_action = WeaponAction::INSPECT;
  Animator *weapon_animator =
      AssetManager::get_animator_by_name(weapon_info->animation_names.inspect);
  _weapon_anim_entity->set_animator(weapon_animator);
  weapon_animator->PlayAnimation();
}
