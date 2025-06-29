#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

struct AllyAnimationNames {
  std::string idle;
  std::string walk;
  std::string attack;
};

struct AllyAudioFiles {
  std::string attack;
};

struct AllyInfo {
  std::string        name  = "Ally0";
  int                level = 0;
  std::string        skinned_model;
  AllyAudioFiles     audio_files;
  AllyAnimationNames animation_names;

  // Game
  float damage;

  // Transform
  glm::vec3 base_position = glm::vec3(0.0f);
  glm::quat base_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 base_scale    = glm::vec3(1.0f);

  // Collider
  float     capsule_radius;
  float     capsule_height;
  glm::vec3 capsule_position;
};
