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

  // Default weapon
  give_weapon("Pistol");
  give_weapon("HK_416");

  // Anim Entity
  {
    AnimEntity weapon_view;
    Model     *pistol_model          = AssetManager::get_model_by_name("Pistol");
    Animator  *pistol_idle_animation = AssetManager::get_animator_by_name("Pistol_Idle");
    glm::mat4  weapon_transform      = weapon_view_transform();

    weapon_view.init("Weapon_View", pistol_model, pistol_idle_animation, weapon_transform);
    World::add_anim_entity(std::move(weapon_view));
    _weapon_anim_entity = World::get_anim_entity_by_name("Weapon_View");
  }
}

void Player::update_weapon(float delta_time) {
  if (Input::key_pressed(EUN_KEY_Q)) {
    next_weapon();
  }

  WeaponInfo *weapon_info = get_current_weapon_info();
  // WeaponState *weapon_state = get_current_weapon_state();

  if (!weapon_info) {
    return;
  }

  switch (weapon_info->type) {
  case WeaponType::PISTOL:
  case WeaponType::AUTOMATIC:
    update_weapon_gun(delta_time);
    break;
  default:;
  };

  Animator *weapon_animator = _weapon_anim_entity->get_animator();

  if (_weapon_action == WeaponAction::DRAW && weapon_animator->IsDone()) {
    Animator *idle_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
    _weapon_anim_entity->set_animator(idle_animator);
    _weapon_action = WeaponAction::IDLE;
  }

  if (_weapon_action == WeaponAction::FIRE && weapon_animator->IsDone()) {
    Animator *idle_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
    _weapon_anim_entity->set_animator(idle_animator);
    _weapon_action = WeaponAction::IDLE;
  }

  if (_weapon_action == WeaponAction::INSPECT && weapon_animator->IsDone()) {
    Animator *idle_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.idle);
    _weapon_anim_entity->set_animator(idle_animator);
    _weapon_action = WeaponAction::IDLE;
  }

  _weapon_anim_entity->set_model_transform(weapon_view_transform());
}

void Player::next_weapon() {
  _current_weapon_index++;
  if (_current_weapon_index == _weapon_states.size()) {
    _current_weapon_index = 0;
  }
  while (!_weapon_states[_current_weapon_index].has) {
    _current_weapon_index++;
    if (_current_weapon_index == _weapon_states.size()) {
      _current_weapon_index = 0;
    }
  }

  switch_weapon(_weapon_states[_current_weapon_index].name);
}

void Player::switch_weapon(const std::string &name) {
  WeaponState *state       = get_weapon_state_by_name(name);
  WeaponInfo  *weapon_info = WeaponManager::get_weapon_info_by_name(name);

  if (!state || !weapon_info) {
    return;
  }

  for (int i = 0; i < _weapon_states.size(); i++) {
    if (_weapon_states[i].name == name) {
      _current_weapon_index = i;
    }
  }

  Model    *weapon_model    = AssetManager::get_model_by_name(weapon_info->model_name);
  Animator *weapon_animator = AssetManager::get_animator_by_name(weapon_info->animation_names.draw);
  _weapon_anim_entity->set_model(weapon_model);
  _weapon_anim_entity->set_animator(weapon_animator);

  weapon_animator->PlayAnimation();
  _weapon_action = WeaponAction::DRAW;
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

glm::mat4 weapon_view_transform() {
  glm::mat4 weapon_transform = glm::mat4(1.0f);
  weapon_transform           = glm::translate(weapon_transform, Game::get_player()->get_pos());
  weapon_transform           = glm::translate(weapon_transform, glm::vec3(0.0f, -0.3f, 0.0f));
  weapon_transform = glm::translate(weapon_transform, Game::get_camera()->get_front() * 0.5f);
  glm::quat rot = glm::quatLookAt(-Game::get_camera()->get_front(), Game::get_camera()->get_up());
  weapon_transform *= glm::toMat4(rot);
  weapon_transform = glm::scale(weapon_transform, glm::vec3(0.5f));

  return weapon_transform;
}
