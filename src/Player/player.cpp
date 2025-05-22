#include "Player.h"
#include "AssetManager/AssetManager.h"
#include "Defines.h"
#include "Keycodes.h"
#include "Weapon/WeaponManager.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position  = position;
  _character = create_character_virtual(
      glm::vec3(_position.x, _position.y, _position.z), PLAYER_HEIGHT, 0.25f);

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
  _weapon_view_animator = AssetManager::get_animator_by_name("Pistol_Idle"); // HACK
}

void Player::update(float delta_time, Camera camera) {
  update_movement(delta_time, camera);
  update_weapon(delta_time);
  update_flashlight();
}

glm::vec3 Player::get_pos() {
  RVec3 pos = _character->GetPosition();
  return glm::vec3(pos.GetX(), pos.GetY() + PLAYER_HEIGHT * 0.5f, pos.GetZ());
}
