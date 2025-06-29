#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

struct BotAnimationNames {
  std::string idle;
};

struct BotAudioFiles {
  std::string hurt;
};

struct BotInfo {
  std::string       name  = "Bot0";
  int               level = 0;
  std::string       skinned_model;
  BotAudioFiles     audio_files;
  BotAnimationNames animation_names;

  // Game
  float health;
  float reward;

  // Transform
  glm::vec3 base_position = glm::vec3(0.0f);
  glm::quat base_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 base_scale    = glm::vec3(1.0f);

  // Collider
  float     capsule_radius;
  float     capsule_height;
  glm::vec3 capsule_position;
};
