#pragma once

#include "Npc.h"

struct NpcEnemy : public Npc {
  NpcEnemy() = default;

  void init(NpcCreateInfo &&npc_create_info);
  bool take_damage(float damage);
  int  get_reward();

private:
  float _health         = 0.0f;
  float _current_health = 0.0f;
  int   _reward         = 0.0f;
};
