#include "Ally.h"
#include "CreateInfo.h"
#include "Types/Game/Ally/AllyManager.h"
#include "Util/Util.h"

void Ally::init(const std::string &name, glm::vec3 offset_position) {
  AllyInfo *info = AllyManager::get_ally_info_by_name(name);

  _damage          = info->damage;
  _animation_names = info->animation_names;

  NpcCreateInfo npc_info;
  npc_info.name             = info->name;
  npc_info.skinned_model    = info->skinned_model;
  npc_info.position         = info->base_position + offset_position;
  npc_info.scale            = info->base_scale;
  npc_info.object_type      = ObjectType::ALLY;
  npc_info.capsule_radius   = info->capsule_radius;
  npc_info.capsule_height   = info->capsule_height;
  npc_info.capsule_position = Util::to_capsule_position(
      npc_info.position, npc_info.capsule_height, npc_info.capsule_radius);

  Npc::init(std::move(npc_info));

  _anim_entity.loop_animation(_animation_names.idle);
}
