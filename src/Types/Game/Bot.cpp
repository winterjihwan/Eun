#include "Bot.h"
#include "CreateInfo.h"

void Bot::init(NpcCreateInfo &&info) {
  BotCreateInfo &bot_info = static_cast<BotCreateInfo &>(info);

  // Game
  _health         = bot_info.health;
  _current_health = _health;
  _reward         = bot_info.reward;

  Npc::init(std::move(info));
}

bool Bot::take_damage(float damage) {
  _current_health -= damage;
  if (_current_health <= 0.0f) {
    _current_health = _health;
    return true;
  }
  return false;
}

int Bot::get_reward() {
  return _reward;
}
