#pragma once

#include "AllyCommon.h"
#include "Types/Game/Npc.h"

struct Ally : public Npc {
  Ally() = default;

  void init(const std::string &name);

private:
  float              _damage = 0.0f;
  AllyAnimationNames _animation_names;
};
