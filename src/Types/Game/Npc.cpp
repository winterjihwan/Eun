#include "Npc.h"
#include "CreateInfo.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types/Game/AnimEntity.h"
#include "Types/UID/UID.h"
#include "Util/Util.h"

void Npc::init(NpcCreateInfo &&info) {
  _name = info.name;
  _uid  = UID::get_next();

  _anim_entity.set_name(info.name);
  _anim_entity.set_skinned_model(info.skinned_model);
  _anim_entity.set_position(info.position);
  _anim_entity.set_rotation(info.rotation);
  _anim_entity.set_scale(info.scale);

  // Physics
  float                     capsule_radius   = info.capsule_radius;
  float                     capsule_height   = info.capsule_height;
  glm::vec3                 capsule_position = info.capsule_position;
  JPH::RVec3                position         = Util::to_jolt_vec3(capsule_position);
  JPH::Quat                 rotation         = JPH::Quat::sIdentity();
  JPH::CapsuleShapeSettings shape_settings(capsule_height / 2, capsule_radius);
  JPH::ShapeRefC            shape = shape_settings.Create().Get();

  _body = Physics::register_collider(shape, position, rotation, info.object_type, _uid);
  _aabb = Physics::get_aabb(_body);
}

void Npc::update(float delta_time) {
  _anim_entity.update(delta_time);
}

AnimEntity *Npc::get_anim_entity() {
  return &_anim_entity;
}

const std::string &Npc::get_name() {
  return _name;
}

uint64_t &Npc::get_id() {
  return _uid;
}

glm::vec3 Npc::get_position() {
  return _anim_entity.get_transform()[3];
}
