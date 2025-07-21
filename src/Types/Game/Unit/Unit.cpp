#include "Unit.h"
#include "Core/Game.h"
#include "CreateInfo.h"
#include "Player/Player.h"
#include "Types/Game/Unit/UnitManager.h"
#include "Util/Util.h"
#include "World/World.h"

void Unit::init(const std::string &name, glm::vec3 offset_position) {
  UnitInfo *info = UnitManager::get_unit_info_by_name(name);

  _damage          = info->damage;
  _animation_names = info->animation_names;

  NpcCreateInfo npc_info;
  npc_info.name            = info->name;
  npc_info.model_name      = info->model_name;
  npc_info.position        = info->base_position + offset_position;
  npc_info.scale           = info->base_scale;
  npc_info.object_type     = ObjectType::UNIT;
  npc_info.collider_shape  = info->collider_shape;
  npc_info.collider_offset = info->collider_offset;

  Npc::init(std::move(npc_info));

  if (AnimEntity *anim_entity = get_anim_entity()) {
    anim_entity->loop_animation(_animation_names.idle);
  }
}

void Unit::update(float delta_time) {
  Npc::update(delta_time);
}

void Unit::attack(Building &building) {
  if (AnimEntity *anim = get_anim_entity()) {
    anim->loop_animation(_animation_names.attack);
  }

  bool destroyed = building.take_damage(_damage);
  if (destroyed) {
    Game::get_player()->add_minerals(building.get_reward());
  }
}

void Unit::idle() {
  if (AnimEntity *anim_entity = get_anim_entity()) {
    anim_entity->loop_animation(_animation_names.idle);
  }
}

void Unit::move_xz(glm::vec2 xz) {
  Npc::move_xz(xz);

  if (AnimEntity *anim_entity = get_anim_entity()) {
    anim_entity->loop_animation(_animation_names.walk);
  }
}

void Unit::stop() {
  set_velocity(glm::vec3(0.0f));

  // Auto Attack
  for (Building &building : World::get_buildings()) {
    if (building.get_aabb()->expanded_contains(get_position(), ATTACK_RANGE)) {
      attack(building);
      return;
    }
  }

  idle();
}
