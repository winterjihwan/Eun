#include "WeaponManager.h"

#include <vector>

namespace WeaponManager {
std::vector<WeaponInfo> _weapons;

void init() {
  _weapons.clear();

  // Weapons
  WeaponInfo &pistol             = _weapons.emplace_back();
  pistol.name                    = "Pistol";
  pistol.model_name              = "Pistol";
  pistol.type                    = WeaponType::PISTOL;
  pistol.animation_names.draw    = "Pistol_Draw";
  pistol.animation_names.idle    = "Pistol_Idle";
  pistol.animation_names.inspect = "Pistol_Inspect";
  pistol.animation_names.fire    = "Pistol_Fire";
  pistol.animation_names.reload  = "Pistol_Reload";
  pistol.audio_files.fire        = "Pistol_fire.wav";
  pistol.audio_files.reload      = "Pistol_reload.wav";
  // pistol.audio_files.draw        = ;

  WeaponInfo &hk_416             = _weapons.emplace_back();
  hk_416.name                    = "HK_416";
  hk_416.model_name              = "HK_416";
  hk_416.type                    = WeaponType::AUTOMATIC;
  hk_416.animation_names.draw    = "HK_416_Draw";
  hk_416.animation_names.idle    = "HK_416_Idle";
  hk_416.animation_names.inspect = "HK_416_Inspect";
  hk_416.animation_names.fire    = "HK_416_Fire";
  hk_416.animation_names.reload  = "HK_416_Reload";
  hk_416.audio_files.fire        = "Pistol_fire.wav";
  hk_416.audio_files.reload      = "Pistol_reload.wav";
  // hk_416.audio_files.draw        = ;
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
