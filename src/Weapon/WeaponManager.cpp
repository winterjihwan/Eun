#include "WeaponManager.h"

#include <vector>

namespace WeaponManager {

std::vector<WeaponInfo> g_weapons;

void Init() {
  g_weapons.clear();

  // Weapons
  WeaponInfo &hk_416            = g_weapons.emplace_back();
  hk_416.name                   = "HK_416";
  hk_416.model_name             = "HK_416";
  hk_416.type                   = WeaponType::AUTOMATIC;
  hk_416.animation_names.idle   = "HK_416_Idle";
  hk_416.animation_names.draw   = "HK_416_Draw";
  hk_416.animation_names.reload = "HK_416_Reload";
}

WeaponInfo *GetWeaponInfoByName(const std::string &name) {
  for (int i = 0; i < g_weapons.size(); i++) {
    if (g_weapons[i].name == name) {
      return &g_weapons[i];
    }
  }
  return nullptr;
}

WeaponInfo *GetWeaponInfoByIndex(int index) {
  if (index >= 0 && index < g_weapons.size()) {
    return &g_weapons[index];
  }
  return nullptr;
}

int GetWeaponCount() {
  return g_weapons.size();
}

int32_t GetWeaponIndexFromWeaponName(const std::string &weaponName) {
  std::vector<WeaponInfo> g_weapons;
  for (int i = 0; i < g_weapons.size(); i++) {
    if (g_weapons[i].name == weaponName) {
      return i;
    }
  }
  return -1;
}
} // namespace WeaponManager
