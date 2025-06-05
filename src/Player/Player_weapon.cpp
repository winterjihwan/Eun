#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
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
    // if (_current_weapon_type == WeaponType::HANDGUN) {
    //   _player_animator.play_animation(_player_animations.gun_draw);
    // }
    break;
  case WeaponAction::STAB:
    _player_state = PlayerState::STAB;
    if (_player_animator.is_complete()) {
      _weapon_action = WeaponAction::IDLE;
    }
    break;
  case WeaponAction::FIRE:
    // _player_animator.play_animation(_player_animations.gun_fire);
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

  if (weapon_info->type == WeaponType::KNIFE) {
    Model *brian_knife = AssetManager::get_model_by_name("Brian_Knife");
    _player_anim_entity->set_model(brian_knife);
  }
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

// TODO: Decal position
// Delay for stab sound
// Faster stab
// More stabs?
void Player::perform_stab() {
  Camera *camera = Game::get_camera();

  float     stab_dist = 2.0f;
  JPH::Vec3 origin    = Util::to_jolt_vec3(_position);
  JPH::Vec3 direction = Util::to_jolt_vec3(camera->get_front());

  std::optional<RayHitInfo> hit = Physics::raycast(origin, direction, stab_dist);
  if (!hit.has_value() || !hit->user_data) {
    return;
  }

  PhysicsUserData *data = hit->user_data;

  if (data->physics_type == PhysicsType::RIGID_DYNAMIC && data->object_type == ObjectType::NPC) {
    // Blood Volumetric
    BloodVolumetricCreateInfo info;
    info.position          = Util::from_jolt_vec3(hit->hit_pos);
    info.rotation          = glm::vec3(0.0f);
    info.front             = camera->get_front();
    info.exr_texture_index = 1;
    info.model             = AssetManager::get_model_by_name("Blood_1");

    World::add_blood_volumetric(BloodVolumetric(info));

    // Blood Decal
    glm::vec3 offset =
        glm::vec3(Util::random_float(-0.3f, 0.3f), 0.0f, Util::random_float(-0.3f, 0.3f));

    DecalCreateInfo blood_info;
    blood_info.hit_position = Util::from_jolt_vec3(hit->hit_pos) + offset;
    blood_info.hit_normal   = Util::from_jolt_vec3(hit->hit_normal);
    blood_info.type         = DecalType::BLOOD;
    blood_info.mesh         = AssetManager::get_mesh_by_name("Blood_Stain_1");

    World::add_decal(Decal(blood_info));
  }

  if (data->physics_type == PhysicsType::RIGID_STATIC) {
    DecalCreateInfo decal_info;
    decal_info.hit_position = Util::from_jolt_vec3(hit->hit_pos);
    decal_info.hit_normal   = Util::from_jolt_vec3(hit->hit_normal);
    decal_info.type         = DecalType::PLASTER;
    decal_info.mesh         = AssetManager::get_mesh_by_name("Knife_Scratch");

    World::add_decal(Decal(decal_info));
  }
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
