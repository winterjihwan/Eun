#pragma once
#include "CreateInfo.h"
#include "Types/AABB/AABB.h"
#include "Types/Game/AnimEntity.h"
#include "Types/Game/Entity.h"
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <string>

struct Npc {
  Npc()          = default;
  virtual ~Npc() = default;

  void init(NpcCreateInfo &&npc_create_info);
  void update(float delta_time);
  void submit_render_item();

  void         set_velocity(glm::vec3 velocity);
  virtual void move_xz(glm::vec2 xz);
  virtual void stop();

  AnimEntity        *get_anim_entity();
  const std::string &get_name();
  uint64_t          &get_id();
  JPH::BodyID       *get_body();
  AABB              *get_aabb();
  glm::vec3          get_position();

protected:
  std::string                      _name = "Npc";
  std::variant<AnimEntity, Entity> _entity;

  // Physics
  JPH::BodyID              _body;
  AABB                     _aabb;
  uint64_t                 _uid;
  glm::vec3                _collider_offset;
  std::optional<glm::vec2> _move_xz;
};
