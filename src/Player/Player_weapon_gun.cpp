#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"

void Player::update_weapon_gun(float delta_time) {
  WeaponInfo *weapon_info = get_current_weapon_info();

  if (Input::left_mouse_pressed()) {
    Audio::play_audio(weapon_info->audio_files.fire, 1.0f);
    _weapon_action = WeaponAction::FIRE;
    Animator *weapon_animator =
        AssetManager::get_animator_by_name(weapon_info->animation_names.fire);
    _weapon_anim_entity->set_animator(weapon_animator);
    weapon_animator->PlayAnimation();
  }

  if (Input::key_pressed(EUN_KEY_R)) {
    Audio::play_audio(weapon_info->audio_files.reload, 1.0f);
    // TODO: Check if can reload
    _weapon_action = WeaponAction::RELOAD;
    Animator *weapon_animator =
        AssetManager::get_animator_by_name(weapon_info->animation_names.reload);
    _weapon_anim_entity->set_animator(weapon_animator);
    weapon_animator->PlayAnimation();
  }

  if (Input::key_pressed(EUN_KEY_E)) {
    _weapon_action = WeaponAction::INSPECT;
    Animator *weapon_animator =
        AssetManager::get_animator_by_name(weapon_info->animation_names.inspect);
    _weapon_anim_entity->set_animator(weapon_animator);
    weapon_animator->PlayAnimation();
  }
}
