#pragma once

#include "Npc.h"

struct NpcAlly : public Npc {
  NpcAlly() = default;

  void init(NpcCreateInfo &&npc_create_info);

private:
  float _damage = 0.0f;
};
