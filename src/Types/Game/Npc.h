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

private:
  std::string   _name = "NPC";
  Model        *_model;
  Animator      _npc_animator;
  NpcAnimations _npc_animations;
  NpcState      _npc_state       = NpcState::IDLE;
  glm::mat4     _model_transform = glm::mat4(1.0f);
  AnimEntity    _npc_entity;
  JPH::BodyID   _collider;
};
