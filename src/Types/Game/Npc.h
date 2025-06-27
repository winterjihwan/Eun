#pragma once
#include "CreateInfo.h"
#include "Enums.h"
#include "Types/Game/AnimEntity.h"
#include <string>

struct Npc {
  Npc() = default;

  void init(NpcCreateInfo &&npc_create_info);
  void update(float delta_time);

  void        set_animation_state(NpcState npc_animation_state);
  AnimEntity *get_anim_entity();

  const std::string &get_name();
  glm::vec3          get_position();

private:
  std::string   _name = "Npc";
  AnimEntity    _npc_entity;
  NpcAnimations _npc_animations;
  NpcState      _npc_state = NpcState::IDLE;

  // Physics
  JPH::BodyID _body;
  JPH::AABox  _aabb;
  uint64_t    _uid;
};
