#include "Building.h"
#include "CreateInfo.h"
#include "Types/Game/Building/BuildingManager.h"

void Building::init(const std::string &name) {
  BuildingInfo *info = BuildingManager::get_bot_info_by_name(name);

  _health          = info->health;
  _current_health  = _health;
  _reward          = info->reward;
  _animation_names = info->animation_names;

  NpcCreateInfo npc_info;
  npc_info.name            = info->name;
  npc_info.model_name      = info->model_name;
  npc_info.is_animated     = info->is_animated;
  npc_info.position        = info->base_position;
  npc_info.rotation        = info->base_rotation;
  npc_info.scale           = info->base_scale;
  npc_info.object_type     = ObjectType::BUILDING;
  npc_info.collider_shape  = info->collider_shape;
  npc_info.collider_offset = info->collider_offset;

  Npc::init(std::move(npc_info));

  if (AnimEntity *anim_entity = get_anim_entity()) {
    anim_entity->loop_animation(_animation_names.idle);
  }
}

bool Building::take_damage(float damage) {
  _current_health -= damage;
  if (_current_health <= 0.0f) {
    _current_health = _health;
    return true;
  }
  return false;
}

int Building::get_reward() {
  return _reward;
}
