#include "Npc.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types.h"
#include "Types/Game/AnimEntity.h"
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

  // Physics
  float      capsule_radius   = npc_create_info.capsule_radius;
  float      capsule_height   = npc_create_info.capsule_height;
  glm::vec3  capsule_position = npc_create_info.capsule_position;
  JPH::RVec3 pos              = Util::to_jolt_vec3(capsule_position);
  JPH::Quat  rot              = JPH::Quat::sIdentity();

  JPH::CapsuleShapeSettings shape_settings(capsule_height / 2, capsule_radius);
  JPH::ShapeRefC            shape = shape_settings.Create().Get();

  // TODO: Make motion type dynamic please
  JPH::BodyCreationSettings settings(shape, pos, rot, JPH::EMotionType::Static, Layers::MOVING);
  JPH::BodyInterface       &bi   = Physics::get_physics_system().GetBodyInterface();
  JPH::Body                *body = bi.CreateBody(settings);

  PhysicsUserData user_data;
  user_data.physics_type = PhysicsType::RIGID_DYNAMIC;
  user_data.object_type  = ObjectType::NPC;
  body->SetUserData(reinterpret_cast<JPH::uint64>(new PhysicsUserData(user_data)));
  bi.AddBody(body->GetID(), JPH::EActivation::Activate);

  _collider = body->GetID();
  _aabb     = shape->GetLocalBounds();
}

void Npc::update(float delta_time) {
  // Animation
  if (_npc_state == NpcState::IDLE) {
    _npc_entity.loop_animation(_npc_animations.idle);
  }

  _npc_entity.update(delta_time);

  // Physics
  const JPH::Body *body =
      Physics::get_physics_system().GetBodyLockInterface().TryGetBody(_collider);
  if (body) {
    // Collider
    JPH::RVec3 pos = body->GetCenterOfMassPosition();

    // HACK: Subtract capsule height by half
    _model_transform =
        glm::translate(glm::mat4(1.0f), glm::vec3(pos.GetX(), pos.GetY() - 0.9f, pos.GetZ()));

    // AABB
    JPH::Mat44 transform =
        JPH::Mat44::sRotationTranslation(body->GetRotation(), body->GetPosition());
    _aabb = body->GetShape()->GetWorldSpaceBounds(transform, JPH::Vec3::sReplicate(1.0f));
  } else {
    std::cout << "AnimEntity::update(), No body found for body_id: " << _collider.GetIndex()
              << std::endl;
    assert(0);
  }
}

AnimEntity *Npc::get_anim_entity() {
  return &_npc_entity;
}

const std::string &Npc::get_name() {
  return _name;
}

glm::vec3 Npc::get_position() {
  return _npc_entity.get_model_matrix()[3];
}
