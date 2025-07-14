#pragma once

#include "Types/Game/Npc.h"
#include "UnitCommon.h"

struct Unit : public Npc {
  Unit() = default;

  void init(const std::string &name, glm::vec3 position);

private:
  float              _damage = 0.0f;
  UnitAnimationNames _animation_names;
};
