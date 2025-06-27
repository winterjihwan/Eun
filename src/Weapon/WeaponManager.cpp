#include "WeaponManager.h"

#include <vector>

namespace WeaponManager {
std::vector<WeaponInfo> _weapons;

void init() {
  _weapons.clear();

  /* Weapons */

  // Glock
  WeaponInfo &glock            = _weapons.emplace_back();
  glock.name                   = "Glock";
  glock.model_name             = "Glock";
  glock.type                   = WeaponType::GLOCK;
  glock.animation_names.idle   = "Glock_Idle";
  glock.animation_names.draw   = "Glock_Draw";
  glock.animation_names.fire   = "Glock_Fire0";
  glock.animation_names.reload = "Glock_Reload";
  glock.audio_files.fire       = "Glock_Fire.wav";
  glock.audio_files.reload     = "Glock_Reload.wav";
  // pistol.audio_files.draw        = ;
  glock.damage       = 10;
  glock.offset_pos   = glm::vec3(0.0f, 15.0f, 0.1f);
  glock.offset_rot   = glm::radians(glm::vec3(0.0f, 180.0f, 270.0f));
  glock.offset_scale = glm::vec3(100.0f);

  // Weapon
  WeaponInfo &knife          = _weapons.emplace_back();
  knife.name                 = "Knife";
  knife.model_name           = "Knife";
  knife.type                 = WeaponType::KNIFE;
  knife.animation_names.idle = "Knife_Idle";
  knife.animation_names.draw = "Knife_Draw";
  knife.animation_names.stab = std::vector<std::string>{
      "Knife_Swing0",
      "Knife_Swing1",
      "Knife_Swing2",
  };
  knife.audio_files.stab = "Glock_Fire.wav";
  knife.damage           = 12;
  knife.offset_pos       = glm::vec3(0.0f, 0.0f, 0.0f);
  knife.offset_rot       = glm::radians(glm::vec3(0.0f, 0.0f, 0.0f));
  knife.offset_scale     = glm::vec3(100.0f);
}

int get_weapon_count() {
  return _weapons.size();
}

WeaponInfo *get_weapon_info_by_name(const std::string &name) {
  for (int i = 0; i < _weapons.size(); i++) {
    if (_weapons[i].name == name) {
      return &_weapons[i];
    }
  }
  return nullptr;
}

WeaponInfo *get_weapon_info_by_index(int index) {
  if (index >= 0 && index < _weapons.size()) {
    return &_weapons[index];
  }
  return nullptr;
}

int32_t get_weapon_index_from_weapon_name(const std::string &weaponName) {
  std::vector<WeaponInfo> _weapons;
  for (int i = 0; i < _weapons.size(); i++) {
    if (_weapons[i].name == weaponName) {
      return i;
    }
  }
  return -1;
}
} // namespace WeaponManager
