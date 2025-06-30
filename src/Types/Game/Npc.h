#pragma once
#include "CreateInfo.h"
#include "Types/Game/AnimEntity.h"
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <string>

struct Npc {
  Npc()          = default;
  virtual ~Npc() = default;

  void init(NpcCreateInfo &&npc_create_info);
  void update(float delta_time);

  void set_velocity();

  AnimEntity        *get_anim_entity();
  const std::string &get_name();
  uint64_t          &get_id();
  glm::vec3          get_position();
  JPH::BodyID        get_body();

protected:
  std::string _name = "Npc";
  AnimEntity  _anim_entity;

  // Physics
  JPH::BodyID _body;
  JPH::AABox  _aabb;
  uint64_t    _uid;
};
