#pragma once

#include "CreateInfo.h"
#include "Npc.h"

struct Ally : public Npc {
  Ally() = default;

  void init(NpcCreateInfo &&info);

private:
  float _damage = 0.0f;
};
