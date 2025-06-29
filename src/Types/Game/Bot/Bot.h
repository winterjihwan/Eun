#pragma once

#include "Types/Game/Bot/BotCommon.h"
#include "Types/Game/Npc.h"

struct Bot : public Npc {
  Bot() = default;

  void init(const std::string &name);
  bool take_damage(float damage);
  int  get_reward();

private:
  float             _health         = 0.0f;
  float             _current_health = 0.0f;
  int               _reward         = 0.0f;
  BotAnimationNames _animation_names;
};
