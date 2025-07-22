#pragma once

#include "Types/Shape/Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

struct UnitAnimationNames {
  std::string idle;
  std::string walk;
  std::string attack;
};

struct UnitAudioFiles {
  std::string attack;
};

struct UnitInfo {
  std::string        name  = "Unit0";
  int                level = 0;
  std::string        model_name;
  UnitAudioFiles     audio_files;
  UnitAnimationNames animation_names;

  // Game
  float damage;
  float attack_cooldown;
  float enhance_chance;

  // Transform
  glm::vec3 base_position = glm::vec3(0.0f);
  glm::quat base_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 base_scale    = glm::vec3(1.0f);

  // Collider
  Shape     collider_shape;
  glm::vec3 collider_offset;
};
