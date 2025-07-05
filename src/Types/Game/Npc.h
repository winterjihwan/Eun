#pragma once
#include "CreateInfo.h"
#include "Types/AABB/AABB.h"
#include "Types/Game/AnimEntity.h"
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <string>

struct Npc {
  Npc()          = default;
  virtual ~Npc() = default;

  void init(NpcCreateInfo &&npc_create_info);
  void update(float delta_time);
  void submit_render_item();

  void set_velocity(glm::vec3 velocity);

  AnimEntity        *get_anim_entity();
  const std::string &get_name();
  uint64_t          &get_id();
  glm::vec3          get_position();
  JPH::BodyID       *get_body();
  AABB              *get_aabb();

protected:
  std::string _name = "Npc";
  AnimEntity  _anim_entity;

  // Physics
  JPH::BodyID _body;
  AABB        _aabb;
  uint64_t    _uid;
  float       _capsule_height = 0.0f;
  float       _capsule_radius = 0.0f;
};
