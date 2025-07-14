#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Player.h"
#include "Types/Game/Weapon/WeaponManager.h"
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

  // Owned Weapons
  give_weapon("Knife");
  _weapon_action = WeaponAction::IDLE;
}

void Player::update_weapon(float delta_time) {
  if (Input::key_pressed(EUN_KEY_Q)) {
    next_weapon();
  }

  if (_current_weapon_type == WeaponType::HAND) {
    return;
  }

  WeaponInfo *weapon_info = get_current_weapon_info();
  if (!weapon_info) {
    return;
  }

  // Logic
  switch (weapon_info->type) {
  case WeaponType::KNIFE:
    update_weapon_knife(delta_time);
    break;
  case WeaponType::GLOCK:
    update_weapon_gun(delta_time);
    break;
  default:;
  };

  // Animation
  if (_weapon_view.all_anim_states_complete()) {
    _weapon_action = WeaponAction::IDLE;
    _weapon_view.loop_animation(weapon_info->animation_names.idle);
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
  _weapon_view.play_animation(weapon_info->animation_names.draw);

  if (weapon_info->type == WeaponType::KNIFE) {
    _weapon_view.set_skinned_model("Knife_View");
  } else if (weapon_info->type == WeaponType::GLOCK) {
    _weapon_view.set_skinned_model("Glock_View");
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

void Player::perform_stab(float damage) {
  // Decals
  Camera *camera = Game::get_camera();

  std::optional<RayHitInfo> hit = Physics::raycast(_position, camera->get_front(), 2.0f);
  if (!hit.has_value()) {
    return;
  }

  PhysicsUserData *data = hit->user_data;
  if (!data) {
    return;
  }

  // Knife Scratch Decal
  if (data->object_type == ObjectType::BUILDING || data->object_type == ObjectType::GAME_OBJECT) {
    DecalCreateInfo info;
    info.hit_position = Util::to_vec3(hit->hit_pos);
    info.hit_normal   = Util::to_vec3(hit->hit_normal);
    info.type         = DecalType::SCRATCH;
    info.mesh         = AssetManager::get_mesh_by_name("Knife_Scratch");

    World::add_decal(Decal(info));
  }

  // Building Interaction
  if (data->physics_type == PhysicsType::RIGID_DYNAMIC &&
      data->object_type == ObjectType::BUILDING) {
    // Damage Building
    Building *bot  = World::get_building_by_object_id(data->object_id);
    bool      dead = bot->take_damage(damage);
    if (dead) {
      _minerals += bot->get_reward();
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
