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
  _animators       = npc_create_info.animators;
  _model_transform = npc_create_info.model_transform;
  _animation_state = npc_create_info.animation_state;

  AnimEntityCreateInfo anim_entity_create_info;
  anim_entity_create_info.name      = _name;
  anim_entity_create_info.model     = _model;
  anim_entity_create_info.animator  = &_animators.idle;
  anim_entity_create_info.transform = _model_transform;

  _anim_entity.init(std::move(anim_entity_create_info));

  // Physics
  float     capsule_radius   = npc_create_info.capsule_radius;
  float     capsule_height   = npc_create_info.capsule_height;
  glm::vec3 capsule_position = npc_create_info.capsule_position;
  RVec3     pos              = Util::to_jolt_vec3(capsule_position);
  Quat      rot              = Quat::sIdentity();

  CapsuleShapeSettings shape_settings(capsule_height / 2, capsule_radius);
  ShapeRefC            shape = shape_settings.Create().Get();

  BodyCreationSettings settings(shape, pos, rot, EMotionType::Static, Layers::NON_MOVING);
  BodyInterface       &bi   = Physics::get_physics_system().GetBodyInterface();
  Body                *body = bi.CreateBody(settings);

  PhysicsUserData user_data;
  user_data.physics_type = PhysicsType::RIGID_DYNAMIC;
  user_data.object_type  = ObjectType::NPC;
  body->SetUserData(reinterpret_cast<uint64>(new PhysicsUserData(user_data)));
  // TODO: Activate upon self created map
  bi.AddBody(body->GetID(), EActivation::DontActivate);

  _collider = body->GetID();
  _anim_entity.set_collider(&_collider);
}

AnimEntity *Npc::get_anim_entity() {
  return &_anim_entity;
}

void Npc::set_animation_state(NpcAnimationState npc_animation_state) {
  _animation_state = npc_animation_state;

  switch (_animation_state) {
  case NpcAnimationState::IDLE:
    _anim_entity.set_animator(&_animators.idle);
    break;
  case NpcAnimationState::WALK:
    _anim_entity.set_animator(&_animators.walk);
    break;
  case NpcAnimationState::DEATH:
    _anim_entity.set_animator(&_animators.death);
    break;
  }

  _anim_entity.get_animator()->PlayAnimation();
}

const std::string &Npc::get_name() {
  return _name;
}
