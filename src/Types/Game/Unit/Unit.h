#pragma once

#include "Types/Game/Building/Building.h"
#include "Types/Game/Npc.h"
#include "UnitCommon.h"

struct Unit : public Npc {
  enum class State {
    Idle,
    Moving,
    Attacking,
  };

  Unit() = default;

  void init(const std::string &name, glm::vec3 position);
  void idle();
  void attack(Building &building);
  void update(float delta_time);

  void move_xz(glm::vec2 xz) override;
  void stop() override;

private:
  State              _state           = State::Idle;
  float              _damage          = 0.0f;
  float              _attack_cooldown = 0.0f;
  UnitAnimationNames _animation_names;
};
