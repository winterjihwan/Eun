#include "Ally.h"
#include "CreateInfo.h"

void Ally::init(NpcCreateInfo &&info) {
  AllyCreateInfo &ally_info = static_cast<AllyCreateInfo &>(info);

  _damage = ally_info.damage;

  Npc::init(std::move(info));
}
