#pragma once

#include "Weapon/WeaponCommon.h"

namespace WeaponManager {
void        init();
int         get_weapon_count();
WeaponInfo *get_weapon_info_by_name(const std::string &name);
WeaponInfo *get_weapon_info_by_index(int index);
int32_t     get_weapon_index_from_weapon_name(const std::string &weaponName);
} // namespace WeaponManager
