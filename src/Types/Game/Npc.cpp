#include "Npc.h"
#include "CreateInfo.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types/Game/AnimEntity.h"
#include "Types/UID/UID.h"
#include "Util/Util.h"
#include <glm/gtc/quaternion.hpp>

void Npc::init(NpcCreateInfo &&info) {
  _name = info.name;
  _uid  = UID::get_next();

  _anim_entity.set_name(info.name);
  _anim_entity.set_skinned_model(info.skinned_model);
  _anim_entity.set_position(info.position);
  _anim_entity.set_rotation(info.rotation);
  _anim_entity.set_scale(info.scale);

  // Physics
  JPH::ShapeRefC shape    = Util::generate_capsule_shape(info.capsule_height, info.capsule_radius);
  glm::vec3      position = info.capsule_position;
  glm::quat      rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  _body = Physics::register_kinematic_collider(shape, position, rotation, info.object_type, _uid);
  _aabb = Physics::get_aabb(_body);
}

void Npc::update(float delta_time) {
  _anim_entity.update(delta_time);
}

void Npc::set_velocity() {
  Physics::set_body_velocity(_body, glm::vec3(0.0f));
  _aabb = Physics::get_aabb(_body);
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

JPH::BodyID Npc::get_body() {
  return _body;
}
