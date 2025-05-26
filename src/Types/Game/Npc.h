#pragma once
#include "CreateInfo.h"
#include "Enums.h"
#include "Types/Game/AnimEntity.h"
#include <string>

struct Npc {
  Npc() = default;

  void init(NpcCreateInfo &&npc_create_info);

  void        set_animation_state(NpcAnimationState npc_animation_state);
  AnimEntity *get_anim_entity();

  const std::string &get_name();

private:
  std::string       _name = "NPC";
  Model            *_model;
  NpcAnimators      _animators;
  NpcAnimationState _animation_state = NpcAnimationState::IDLE;
  glm::mat4         _model_transform = glm::mat4(1.0f);
  AnimEntity        _anim_entity;
  JPH::BodyID       _collider;
};
