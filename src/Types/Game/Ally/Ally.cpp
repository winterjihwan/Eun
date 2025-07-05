#include "Ally.h"
#include "CreateInfo.h"
#include "Types/Game/Ally/AllyManager.h"
#include "Util/Util.h"

void Ally::init(const std::string &name, glm::vec3 offset_position) {
  AllyInfo *info = AllyManager::get_ally_info_by_name(name);

  _damage          = info->damage;
  _animation_names = info->animation_names;

  NpcCreateInfo npc_info;
  npc_info.name            = info->name;
  npc_info.model_name      = info->model_name;
  npc_info.position        = info->base_position + offset_position;
  npc_info.scale           = info->base_scale;
  npc_info.object_type     = ObjectType::ALLY;
  npc_info.collider_shape  = info->collider_shape;
  npc_info.collider_offset = info->collider_offset;

  Npc::init(std::move(npc_info));

  if (AnimEntity *anim_entity = get_anim_entity()) {
    anim_entity->loop_animation(_animation_names.idle);
  }
}
