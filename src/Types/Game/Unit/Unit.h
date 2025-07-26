#pragma once

#include "Types/Game/Building/Building.h"
#include "Types/Game/Npc.h"
#include "UnitCommon.h"

struct Unit : public Npc {
  Unit() = default;

  void init(const std::string &name, glm::vec3 position);
  void update(float delta_time) override;

  void idle();
  void attack(Building &building);
  void move_xz(glm::vec2 xz) override;
  void stop() override;
  void die();
  bool is_dead();

  float get_enhance_chance();

private:
  bool               _dead            = false;
  float              _damage          = 0.0f;
  float              _enhance_chance  = 0.0f;
  float              _attack_timer    = 0.0f;
  float              _attack_cooldown = 1.0f;
  UnitAnimationNames _animation_names;
};
