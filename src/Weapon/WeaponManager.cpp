#include "WeaponManager.h"

#include <vector>

namespace WeaponManager {
std::vector<WeaponInfo> _weapons;

void init() {
  _weapons.clear();

  // Weapons
  WeaponInfo &glock            = _weapons.emplace_back();
  glock.name                   = "Glock";
  glock.model_name             = "Glock";
  glock.type                   = WeaponType::HANDGUN;
  glock.animation_names.idle   = "Gun_Idle";
  glock.animation_names.draw   = "Gun_Draw";
  glock.animation_names.fire   = "Gun_Fire";
  glock.animation_names.reload = "Gun_Reload";
  glock.audio_files.fire       = "Glock_Fire.wav";
  glock.audio_files.reload     = "Glock_Reload.wav";
  // pistol.audio_files.draw        = ;
  glock.damage = 5;
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
