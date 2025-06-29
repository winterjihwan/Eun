#include "NpcEnemy.h"
#include "CreateInfo.h"

void NpcEnemy::init(NpcCreateInfo &&info) {
  // Game
  _health         = info.health;
  _current_health = _health;
  _reward         = info.reward;

  Npc::init(std::move(info));
}

bool NpcEnemy::take_damage(float damage) {
  _current_health -= damage;
  if (_current_health <= 0.0f) {
    _current_health = _health;
    return true;
  }
  return false;
}

int NpcEnemy::get_reward() {
  return _reward;
}
