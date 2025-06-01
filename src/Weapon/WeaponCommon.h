#pragma once

#include <string>

enum class WeaponType {
  MELEE = 0,
  HANDGUN,
  SHOTGUN,
  AUTOMATIC,
  UNDEFINED,
};

struct AnimationNames {
  std::string draw;
  std::string idle;
  std::string fire;
  std::string reload;
};

struct AudioFiles {
  std::string draw;
  std::string fire;
  std::string reload;
};

struct WeaponInfo {
  std::string    name;
  std::string    model_name;
  AudioFiles     audio_files;
  AnimationNames animation_names;
  WeaponType     type;
  float          damage;
};

struct WeaponState {
  bool        has  = false;
  std::string name = "UNDEFINED_STRING";
};
