#pragma once

#include "Weapon/WeaponCommon.h"

namespace WeaponManager {
void        Init();
int         GetWeaponCount();
WeaponInfo *GetWeaponInfoByName(const std::string &name);
WeaponInfo *GetWeaponInfoByIndex(int index);
int32_t     GetWeaponIndexFromWeaponName(const std::string &weaponName);
} // namespace WeaponManager
