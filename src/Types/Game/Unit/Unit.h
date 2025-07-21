#pragma once

#include "Types/Game/Npc.h"
#include "UnitCommon.h"

struct Unit : public Npc {
  UnitAnimationNames _animation_names;

  Unit() = default;

  void init(const std::string &name, glm::vec3 position);

  void move_xz(glm::vec2 xz) override;
  void stop() override;

private:
  float _damage = 0.0f;
};
