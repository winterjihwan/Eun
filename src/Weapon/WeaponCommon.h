#pragma once

#include <string>

enum class WeaponType { MELEE, PISTOL, SHOTGUN, AUTOMATIC, UNDEFINED };

struct AnimationNames {
  std::string idle;
  std::string fire;
  std::string reload;
};

struct WeaponInfo {
  std::string    name;
  std::string    model_name;
  AnimationNames animation_names;
  WeaponType     type;
};

struct WeaponState {
  bool        has  = false;
  std::string name = "UNDEFINED_STRING";
};
