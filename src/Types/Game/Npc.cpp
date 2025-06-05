#include "Npc.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types.h"
#include "Types/Game/AnimEntity.h"
#include "Util/Util.h"

// TODO: Npc Create Info
void Npc::init(NpcCreateInfo &&npc_create_info) {
  _name            = npc_create_info.name;
  _model           = npc_create_info.model;
  _npc_animations  = npc_create_info.animations;
  _model_transform = npc_create_info.model_transform;
  _npc_state       = npc_create_info.npc_state;

  AnimEntityCreateInfo npc_entity_create_info;
  npc_entity_create_info.name      = _name;
  npc_entity_create_info.model     = _model;
  npc_entity_create_info.animator  = &_npc_animator;
  npc_entity_create_info.transform = _model_transform;

  _npc_entity.init(std::move(npc_entity_create_info));

  // Physics
  float     capsule_radius   = npc_create_info.capsule_radius;
  float     capsule_height   = npc_create_info.capsule_height;
  glm::vec3 capsule_position = npc_create_info.capsule_position;
  RVec3     pos              = Util::to_jolt_vec3(capsule_position);
  Quat      rot              = Quat::sIdentity();

  CapsuleShapeSettings shape_settings(capsule_height / 2, capsule_radius);
  ShapeRefC            shape = shape_settings.Create().Get();

  // TODO: Make motion type dynamic please
  BodyCreationSettings settings(shape, pos, rot, EMotionType::Static, Layers::MOVING);
  BodyInterface       &bi   = Physics::get_physics_system().GetBodyInterface();
  Body                *body = bi.CreateBody(settings);

  PhysicsUserData user_data;
  user_data.physics_type = PhysicsType::RIGID_DYNAMIC;
  user_data.object_type  = ObjectType::NPC;
  body->SetUserData(reinterpret_cast<uint64>(new PhysicsUserData(user_data)));
  bi.AddBody(body->GetID(), EActivation::Activate);

  _collider = body->GetID();
  _npc_entity.set_collider(&_collider);
}

void Npc::update(float delta_time) {
  switch (_npc_state) {
  case NpcState::IDLE:
    _npc_entity.play_animation(_npc_animations.idle);
    break;
  case NpcState::WALK:
    _npc_entity.play_animation(_npc_animations.walk);
    break;
  case NpcState::DEATH:
    _npc_entity.play_animation(_npc_animations.death);
    break;
  }

  _npc_entity.update(delta_time);
}

AnimEntity *Npc::get_anim_entity() {
  return &_npc_entity;
}

const std::string &Npc::get_name() {
  return _name;
}

glm::vec3 Npc::get_position() {
  return _npc_entity.get_model_transform()[3];
}
