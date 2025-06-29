#include "NpcAlly.h"
#include "CreateInfo.h"

void NpcAlly::init(NpcCreateInfo &&info) {
  // Game
  _damage = info.damage;

  Npc::init(std::move(info));
}
