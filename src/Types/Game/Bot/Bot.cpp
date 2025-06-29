#include "Bot.h"
#include "CreateInfo.h"
#include "Types/Game/Bot/BotManager.h"

void Bot::init(const std::string &name) {
  BotInfo *info = BotManager::get_bot_info_by_name(name);

  _health          = info->health;
  _current_health  = _health;
  _reward          = info->reward;
  _animation_names = info->animation_names;

  NpcCreateInfo npc_info;
  npc_info.name             = info->name;
  npc_info.skinned_model    = info->skinned_model;
  npc_info.position         = info->base_position;
  npc_info.scale            = info->base_scale;
  npc_info.object_type      = ObjectType::BOT;
  npc_info.capsule_radius   = info->capsule_radius;
  npc_info.capsule_height   = info->capsule_height;
  npc_info.capsule_position = info->capsule_position;

  Npc::init(std::move(npc_info));

  _anim_entity.loop_animation(_animation_names.idle);
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
