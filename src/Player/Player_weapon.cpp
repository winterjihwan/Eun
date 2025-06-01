#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"
#include "Weapon/WeaponManager.h"
#include "World/World.h"

// TODO: Ammo system

glm::mat4 weapon_view_transform();

void Player::init_weapon() {
  // Weapon
  _weapon_states.clear();
  for (int i = 0; i < WeaponManager::get_weapon_count(); i++) {
    WeaponState &state = _weapon_states.emplace_back();
    state.name         = WeaponManager::get_weapon_info_by_index(i)->name;
    state.has          = false;
  }

  // Default weapons
  // TODO: Equip immediately
  give_weapon("Glock");
}

void Player::update_weapon(float delta_time) {
  if (Input::key_pressed(EUN_KEY_Q)) {
    next_weapon();
  }

  if (!_equipped) {
    return;
  }

  WeaponInfo *weapon_info = get_current_weapon_info();
  // WeaponState *weapon_state = get_current_weapon_state();

  if (!weapon_info) {
    return;
  }

  switch (weapon_info->type) {
  case WeaponType::HANDGUN:
  case WeaponType::AUTOMATIC:
    update_weapon_gun(delta_time);
    break;
  default:;
  };

  // Animator *weapon_animator = _weapon_anim_entity->get_animator();

  // if (_weapon_action == WeaponAction::DRAW && weapon_animator->IsDone()) {
  //   Animator *idle_animator =
  //   AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
  //   _weapon_anim_entity->set_animator(idle_animator);
  //   _weapon_action = WeaponAction::IDLE;
  // }
  //
  // if (_weapon_action == WeaponAction::FIRE && weapon_animator->IsDone()) {
  //   Animator *idle_animator =
  //   AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
  //   _weapon_anim_entity->set_animator(idle_animator);
  //   _weapon_action = WeaponAction::IDLE;
  // }
  //
  // if (_weapon_action == WeaponAction::INSPECT && weapon_animator->IsDone()) {
  //   Animator *idle_animator =
  //   AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
  //   _weapon_anim_entity->set_animator(idle_animator);
  //   _weapon_action = WeaponAction::IDLE;
  // }
  //
  // _weapon_anim_entity->set_model_transform(weapon_view_transform());
}

void Player::next_weapon() {
  if (_weapon_states.empty()) {
    return;
  }

  int size = _weapon_states.size();
  for (int i = 1; i <= size; ++i) {
    int next_index = (_current_weapon_index + i) % size;
    if (_weapon_states[next_index].has) {
      _current_weapon_index = next_index;
      switch_weapon(_weapon_states[next_index].name);
      return;
    }
  }

  // No Weapon
}

void Player::switch_weapon(const std::string &name) {
  WeaponState *state       = get_weapon_state_by_name(name);
  WeaponInfo  *weapon_info = WeaponManager::get_weapon_info_by_name(name);

  if (!state || !weapon_info) {
    std::cout << "Player::switch_weapon() failed, weapon name: " << name << std::endl;
    assert(0);
  }

  for (int i = 0; i < _weapon_states.size(); i++) {
    if (_weapon_states[i].name == name) {
      _current_weapon_index = i;
    }
  }

  equip_weapon(weapon_info->model_name);

  _weapon_action = WeaponAction::DRAW;
  _equipped      = true;
  _player_animator.play_animation(_player_animations.gun_draw);
}

void Player::equip_weapon(const std::string &name) {
  Model *glock = AssetManager::get_model_by_name(name);
  // glm::mat4 hand_transform = _animator->GetBoneGlobalTransform("mixamorig12_LeftHand");

  // glm::mat4 pete_transform = glm::translate(glm::mat4(1.0f), glm::vec3(13.0f, 0, -5.0f));
  //
  // glm::mat4 local_offset = glm::mat4(1.0f);
  // local_offset           = glm::translate(local_offset, glm::vec3(0.0f, 15.0f, 0.1f));
  // local_offset *=
  //     glm::eulerAngleXYZ(glm::radians(0.0f), glm::radians(180.0f), glm::radians(270.0f));
  // local_offset = glm::scale(local_offset, glm::vec3(100.0f));
  //
  // glm::mat4 model_glock = pete_transform * hand_transform * local_offset;
  // model_glock           = pete_transform * hand_transform * local_offset;
  //
  // shader.setMat4("model", model_glock);
  // glock->draw(shader);
}

void Player::give_weapon(const std::string &name) {
  WeaponState *state      = get_weapon_state_by_name(name);
  WeaponInfo  *weaponInfo = WeaponManager::get_weapon_info_by_name(name);
  if (state && weaponInfo) {
    state->has = true;
  }
}

void Player::spawn_bullet(float variance) {
  WeaponInfo *weapon_info = get_current_weapon_info();

  glm::vec3 bullet_direction = Game::get_camera()->get_front();

  bullet_direction.x += Util::random_float(-(variance * 0.5f), variance * 0.5f);
  bullet_direction.y += Util::random_float(-(variance * 0.5f), variance * 0.5f);
  bullet_direction.z += Util::random_float(-(variance * 0.5f), variance * 0.5f);
  bullet_direction = glm::normalize(bullet_direction);

  BulletCreateInfo createInfo;
  createInfo.origin       = _position;
  createInfo.direction    = bullet_direction;
  createInfo.damage       = weapon_info->damage;
  createInfo.weapon_index = WeaponManager::get_weapon_index_from_weapon_name(weapon_info->name);

  World::add_bullet(Bullet(createInfo));
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

glm::mat4 Player::weapon_view_transform() {
  glm::vec3 base_pos = get_pos() + glm::vec3(0.0f, -0.3f, 0.0f);
  base_pos += Game::get_camera()->get_front() * 0.5f;

  glm::quat view_rot =
      glm::quatLookAt(-Game::get_camera()->get_front(), Game::get_camera()->get_up());

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), base_pos);
  transform *= glm::toMat4(view_rot);
  transform = glm::scale(transform, glm::vec3(0.5f));

  return transform;
}
