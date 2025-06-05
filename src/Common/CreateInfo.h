#pragma once

#include "Enums.h"
#include "Model.h"
#include "Types/Animation/Animator.h"
#include <glm/glm.hpp>

struct BulletCreateInfo {
  glm::vec3 origin       = glm::vec3(0);
  glm::vec3 direction    = glm::vec3(0);
  int32_t   weapon_index = 0;
  uint32_t  damage       = 0;
};

struct DecalCreateInfo {
  DecalType type;
  glm::vec3 hit_position = glm::vec3(0.0f);
  glm::vec3 hit_normal   = glm::vec3(0.0f);

  Mesh *mesh;
};

struct NpcAnimations {
  Animation *idle;
  Animation *walk;
  Animation *death;
};

struct NpcCreateInfo {
  NpcCreateInfo() = default;

  std::string   name;
  Model        *model;
  NpcAnimations animations;
  glm::mat4     model_transform = glm::mat4(1.0f);

  // Collider Capsule
  float     capsule_radius;
  float     capsule_height;
  glm::vec3 capsule_position;

  NpcState npc_state = NpcState::IDLE;
};

struct BloodVolumetricCreateInfo {
  Model       *model;
  unsigned int exr_texture_index;

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 front;
};

struct AnimEntityCreateInfo {
  std::string name;
  Model      *model;
  Animator   *animator;
  glm::mat4   transform;
};

struct EntityCreateInfo {
  std::string                   name;
  std::variant<Model *, Mesh *> renderable;
  glm::mat4                     transform;
};

struct PlayerAnimations {
  // Base
  Animation *idle;
  Animation *walk;

  // Knife
  Animation *idle_knife;
  Animation *stab;
};
