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

void Player::perform_stab() {
  float range           = 2.0f;
  float angle_threshold = 0.8f;

  glm::vec3 origin  = _position;
  glm::vec3 forward = Game::get_camera()->get_front();

  for (Npc &npc : World::get_npcs()) {
    glm::vec3 to_enemy  = npc.get_position() - origin;
    float     distance  = glm::length(to_enemy);
    float     angle_cos = glm::dot(glm::normalize(to_enemy), glm::normalize(forward));

    if (distance <= range && angle_cos >= angle_threshold) {
      glm::vec3 hit_pos    = npc.get_position();
      glm::vec3 hit_normal = glm::vec3(0.0f, 1.0f, 0.0f);

      static unsigned int blood_volumetric_index = 1;
      if (++blood_volumetric_index > 6)
        blood_volumetric_index = 1;

      BloodVolumetricCreateInfo blood_volumetric_create_info;
      blood_volumetric_create_info.position          = hit_pos;
      blood_volumetric_create_info.rotation          = glm::vec3(0.0f);
      blood_volumetric_create_info.front             = forward;
      blood_volumetric_create_info.exr_texture_index = blood_volumetric_index;
      blood_volumetric_create_info.model =
          AssetManager::get_model_by_name(std::format("Blood_{}", blood_volumetric_index));

      World::add_blood_volumetric(BloodVolumetric(blood_volumetric_create_info));

      JPH::Vec3 blood_origin = Util::to_jolt_vec3(hit_pos + 0.02f * hit_normal);
      JPH::Vec3 blood_dir    = JPH::Vec3(0.0f, -1.0f, 0.0f);

      auto decal_hit = Physics::raycast(blood_origin, blood_dir, 10.0f);
      if (!decal_hit || !decal_hit->user_data) {
        break;
      }
      if (decal_hit->user_data->object_type != ObjectType::MAP) {
        break;
      }

      static unsigned int _blood_stain_index = 1;
      if (++_blood_stain_index > 4) {
        _blood_stain_index = 1;
      }

      glm::vec3 offset =
          glm::vec3(Util::random_float(-0.3f, 0.3f), 0.0f, Util::random_float(-0.3f, 0.3f));

      DecalCreateInfo blood_info;
      blood_info.hit_position = Util::from_jolt_vec3(decal_hit->hit_pos) + offset;
      blood_info.hit_normal   = Util::from_jolt_vec3(decal_hit->hit_normal);
      blood_info.type         = DecalType::BLOOD;
      blood_info.mesh =
          AssetManager::get_mesh_by_name(std::format("Blood_Stain_{}", _blood_stain_index));

      World::add_decal(Decal(blood_info));
      break;
    }
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
