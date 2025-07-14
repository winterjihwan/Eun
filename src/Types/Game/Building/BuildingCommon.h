#pragma once

#include "Types/Shape/Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

struct BuildingAnimationNames {
  std::string idle;
};

struct BuildingAudioFiles {
  std::string hurt;
};

struct BuildingInfo {
  std::string name  = "Building0";
  int         level = 0;
  std::string model_name;
  bool        is_animated = true;

  BuildingAudioFiles     audio_files;
  BuildingAnimationNames animation_names;

  // Game
  float health;
  float reward;

  // Transform
  glm::vec3 base_position = glm::vec3(0.0f);
  glm::quat base_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 base_scale    = glm::vec3(1.0f);

  // Collider
  Shape     collider_shape;
  glm::vec3 collider_offset = glm::vec3(0.0f);
};
