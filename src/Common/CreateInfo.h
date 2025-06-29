#pragma once

#include "Enums.h"
#include "Types/Renderer/Mesh.h"
#include "Types/Renderer/Model.h"
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

struct NpcCreateInfo {
  NpcCreateInfo() = default;

  std::string name;
  std::string skinned_model;

  glm::vec3  position    = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::quat  rotation    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3  scale       = glm::vec3(1.0f, 1.0f, 1.0f);
  ObjectType object_type = ObjectType::ALLY;

  // Collider
  float     capsule_radius;
  float     capsule_height;
  glm::vec3 capsule_position;

  NpcState npc_state = NpcState::IDLE;
};

struct BotCreateInfo : public NpcCreateInfo {
  float health = 0.0f;
  int   reward = 0;
};

struct AllyCreateInfo : public NpcCreateInfo {
  float damage = 0.0f;
};

struct BloodVolumetricCreateInfo {
  Model       *model;
  unsigned int exr_texture_index;

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 front;
};

struct Entity;
struct EntityCreateInfo {
  std::string                          name;
  std::variant<Model *, Mesh *>        renderable;
  glm::vec3                            position    = glm::vec3(0.0f);
  glm::quat                            rotation    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3                            scale       = glm::vec3(1.0f);
  ObjectType                           object_type = ObjectType::NONE;
  std::function<void(Entity &, float)> on_update   = nullptr;
  std::function<void(Entity &, float)> on_stand    = nullptr;
};
