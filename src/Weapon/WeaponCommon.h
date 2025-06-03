#pragma once

#include <glm/glm.hpp>
#include <string>

enum class WeaponType {
  HAND = 0,
  KNIFE,
  HANDGUN,
  SHOTGUN,
  AUTOMATIC,
  UNDEFINED,
};

struct AnimationNames {
  std::string idle;
  std::string draw;

  // Gun
  std::string fire;
  std::string reload;

  // Knife
  std::string stab;
};

struct AudioFiles {
  std::string draw;
  std::string fire;
  std::string reload;
  std::string stab;
};

struct WeaponInfo {
  std::string    name;
  std::string    model_name;
  AudioFiles     audio_files;
  AnimationNames animation_names;
  WeaponType     type;
  float          damage;

  // Local Offset
  glm::vec3 offset_pos;
  glm::vec3 offset_rot;
  glm::vec3 offset_scale;
};

struct WeaponState {
  bool        has  = false;
  std::string name = "UNDEFINED_STRING";
};
