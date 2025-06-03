#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Player.h"
#include "Weapon/WeaponCommon.h"
#include "Weapon/WeaponManager.h"
#include "World/World.h"
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

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
  give_weapon("Knife");
}

void Player::update_weapon(float delta_time) {
  if (Input::key_pressed(EUN_KEY_Q)) {
    next_weapon();
  }

  if (_current_weapon_type == WeaponType::HAND) {
    return;
  }

  WeaponInfo *weapon_info = get_current_weapon_info();
  // WeaponState *weapon_state = get_current_weapon_state();

  if (!weapon_info) {
    return;
  }

  // Logic
  switch (weapon_info->type) {
  case WeaponType::KNIFE:
    update_weapon_knife(delta_time);
    break;
  case WeaponType::HANDGUN:
    update_weapon_gun(delta_time);
    break;
  default:;
  };

  // Animation
  switch (_weapon_action) {
  case WeaponAction::DRAW:
    if (_current_weapon_type == WeaponType::HANDGUN) {
      _player_animator.play_animation(_player_animations.gun_draw);
    }
    break;
  case WeaponAction::STAB:
    // TODO: When stab animation ends, then reset _weapon_action to idle
    // Maybe that's force animation play?
    // _player_animator.play_animation(_player_animations.knife_stab);
    break;
  case WeaponAction::FIRE:
    // TODO: Same goes for here
    _player_animator.play_animation(_player_animations.gun_fire);
    break;
  default:
    break;
  }
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

  _current_weapon_type = weapon_info->type;
  _weapon_action       = WeaponAction::DRAW;
  equip_weapon(weapon_info);
}

void Player::equip_weapon(WeaponInfo *weapon_info) {
  Model *glock = AssetManager::get_model_by_name(weapon_info->name);

  glm::vec3 offset_pos   = weapon_info->offset_pos;
  glm::vec3 offset_rot   = weapon_info->offset_rot;
  glm::vec3 offset_scale = weapon_info->offset_scale;

  EntityCreateInfo entity_create_info;
  entity_create_info.name      = weapon_info->name;
  entity_create_info.model     = glock;
  entity_create_info.transform = glm::mat4(1.0f);

  std::function<void(Entity &, float)> on_update = [=](Entity &self, float) {
    glm::mat4 offset           = Util::transform(offset_pos, offset_rot, offset_scale);
    glm::mat4 player_transform = player_view_transform();
    glm::mat4 hand_transform   = _player_animator.get_right_hand_bone_global_transform();
    self.get_transform()       = player_transform * hand_transform * offset;
  };

  Entity entity;
  entity.init(std::move(entity_create_info));
  entity.set_on_update(on_update);

  World::add_entity(std::move(entity));
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
