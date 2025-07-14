#include "Npc.h"
#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Game/AnimEntity.h"
#include "Types/UID/UID.h"
#include "Util/Util.h"
#include <glm/gtc/quaternion.hpp>

void Npc::init(NpcCreateInfo &&info) {
  _name = info.name;
  _uid  = UID::get_next();

  // TODO: Clean this shit
  // AnimEntity
  if (info.is_animated) {
    _entity.emplace<AnimEntity>();
    AnimEntity &anim_entity = std::get<AnimEntity>(_entity);
    anim_entity.set_skinned_model(info.model_name);
    anim_entity.set_name(info.name);
    anim_entity.set_position(info.position);
    anim_entity.set_rotation(info.rotation);
    anim_entity.set_scale(info.scale);

    // Entity
  } else {
    EntityCreateInfo entity_info;
    entity_info.name         = info.name;
    entity_info.renderable   = AssetManager::get_model_by_name(info.model_name);
    entity_info.position     = info.position;
    entity_info.rotation     = info.rotation;
    entity_info.scale        = info.scale;
    entity_info.object_type  = info.object_type;
    entity_info.skip_physics = true;

    _entity.emplace<Entity>(std::move(entity_info));
  }

  // Physics
  _collider_offset        = info.collider_offset;
  JPH::ShapeRefC shape    = Util::to_jph_shape(info.collider_shape);
  glm::vec3      position = info.position + _collider_offset;
  glm::quat      rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  _body = Physics::register_kinematic_collider(shape, position, rotation, info.object_type, _uid);
  _aabb = AABB(Physics::get_aabb(_body));
}

void Npc::update(float delta_time) {
  std::visit([delta_time](auto &entity) { entity.update(delta_time); }, _entity);

  // Position
  if (AnimEntity *anim = std::get_if<AnimEntity>(&_entity)) {
    glm::vec3 position = Physics::get_body_position(_body);
    anim->set_position(position - _collider_offset);
  }

  // Physics
  _aabb = AABB(Physics::get_aabb(_body));
}

void Npc::submit_render_item() {
  if (AnimEntity *anim_entity = std::get_if<AnimEntity>(&_entity)) {
    RenderDataManager::submit_anim_entity(anim_entity);
  } else if (Entity *entity = std::get_if<Entity>(&_entity)) {
    RenderDataManager::submit_entity(entity);
  }
  RenderDataManager::submit_aabb(&_aabb);
}

void Npc::set_velocity(glm::vec3 velocity) {
  Physics::set_body_velocity(_body, velocity);
  _aabb = AABB(Physics::get_aabb(_body));
}

AnimEntity *Npc::get_anim_entity() {
  return std::get_if<AnimEntity>(&_entity);
}

const std::string &Npc::get_name() {
  return _name;
}

uint64_t &Npc::get_id() {
  return _uid;
}

JPH::BodyID *Npc::get_body() {
  return &_body;
}

AABB *Npc::get_aabb() {
  return &_aabb;
}
