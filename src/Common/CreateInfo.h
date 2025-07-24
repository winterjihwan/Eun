#pragma once

#include "Enums.h"
#include "Types/Renderer/Mesh.h"
#include "Types/Renderer/Model.h"
#include "Types/Shape/Shape.h"
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
  std::string model_name;
  bool        is_animated = true;

  glm::vec3  position    = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::quat  rotation    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3  scale       = glm::vec3(1.0f, 1.0f, 1.0f);
  ObjectType object_type = ObjectType::UNIT;

  NpcState npc_state = NpcState::IDLE;

  // Collider
  Shape     collider_shape;
  glm::vec3 collider_offset;
};

struct BuildingCreateInfo : public NpcCreateInfo {
  float health = 0.0f;
  int   reward = 0;
};

struct UnitCreateInfo : public NpcCreateInfo {
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
  glm::vec3                            position      = glm::vec3(0.0f);
  glm::quat                            rotation      = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3                            scale         = glm::vec3(1.0f);
  ObjectType                           object_type   = ObjectType::NONE;
  bool                                 skip_physics  = false;
  bool                                 crystal       = false;
  std::function<void(Entity &, float)> on_update     = nullptr;
  std::function<void(Entity &, float)> on_stand      = nullptr;
  std::function<void(Entity &, float)> on_unit_enter = nullptr;
};
