#include "Npc.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types/Game/AnimEntity.h"
#include "Types/UID/UID.h"
#include "Util/Util.h"

void Npc::init(NpcCreateInfo &&info) {
  _name           = info.name;
  _npc_animations = info.animations;
  _npc_state      = info.npc_state;
  _uid            = UID::get_next();

  _npc_entity.set_name(info.name);
  _npc_entity.set_skinned_model(info.skinned_model);
  _npc_entity.set_position(info.position);
  _npc_entity.set_rotation(info.rotation);
  _npc_entity.set_scale(info.scale);
  _npc_entity.loop_animation(_npc_animations.idle);

  // Game
  _health         = info.health;
  _current_health = _health;
  _reward         = info.reward;

  // Physics
  float                     capsule_radius   = info.capsule_radius;
  float                     capsule_height   = info.capsule_height;
  glm::vec3                 capsule_position = info.capsule_position;
  JPH::RVec3                position         = Util::to_jolt_vec3(capsule_position);
  JPH::Quat                 rotation         = JPH::Quat::sIdentity();
  JPH::CapsuleShapeSettings shape_settings(capsule_height / 2, capsule_radius);
  JPH::ShapeRefC            shape = shape_settings.Create().Get();

  _body = Physics::register_collider(shape, position, rotation, ObjectType::NPC, _uid);
  _aabb = Physics::get_aabb(_body);
}

void Npc::update(float delta_time) {
  _npc_entity.update(delta_time);
}

bool Npc::take_damage(float damage) {
  _current_health -= damage;
  if (_current_health <= 0.0f) {
    _current_health = _health;
    return true;
  }
  return false;
}

AnimEntity *Npc::get_anim_entity() {
  return &_npc_entity;
}

const std::string &Npc::get_name() {
  return _name;
}

uint64_t &Npc::get_id() {
  return _uid;
}

int &Npc::get_reward() {
  return _reward;
}

glm::vec3 Npc::get_position() {
  return _npc_entity.get_transform()[3];
}
