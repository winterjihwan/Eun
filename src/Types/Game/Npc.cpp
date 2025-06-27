#include "Npc.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types/Game/AnimEntity.h"
#include "Types/UID/UID.h"
#include "Util/Util.h"

void Npc::init(NpcCreateInfo &&npc_create_info) {
  _name           = npc_create_info.name;
  _npc_animations = npc_create_info.animations;
  _npc_state      = npc_create_info.npc_state;

  _npc_entity.set_name(npc_create_info.name);
  _npc_entity.set_skinned_model(npc_create_info.skinned_model);
  _npc_entity.set_position(npc_create_info.position);
  _npc_entity.set_rotation(npc_create_info.rotation);
  _npc_entity.set_scale(npc_create_info.scale);
  _npc_entity.loop_animation(_npc_animations.idle);

  _uid = UID::get_next();

  // Physics
  float                     capsule_radius   = npc_create_info.capsule_radius;
  float                     capsule_height   = npc_create_info.capsule_height;
  glm::vec3                 capsule_position = npc_create_info.capsule_position;
  JPH::RVec3                pos              = Util::to_jolt_vec3(capsule_position);
  JPH::Quat                 rot              = JPH::Quat::sIdentity();
  JPH::CapsuleShapeSettings shape_settings(capsule_height / 2, capsule_radius);
  JPH::ShapeRefC            shape = shape_settings.Create().Get();

  _body = Physics::register_collider(shape, pos, rot, ObjectType::NPC, _uid);
  _aabb = Physics::get_aabb(_body);
}

void Npc::update(float delta_time) {
  _npc_entity.update(delta_time);
}

AnimEntity *Npc::get_anim_entity() {
  return &_npc_entity;
}

const std::string &Npc::get_name() {
  return _name;
}

glm::vec3 Npc::get_position() {
  return _npc_entity.get_transform()[3];
}
