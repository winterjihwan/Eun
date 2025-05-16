#include "Physics/Physics.h"
#include "Player.h"

BodyID
Player::create_player_physics(glm::vec3 position, float height, float radius, ObjectLayer layer) {
  CapsuleShapeSettings capsule_shape(height * 0.5f, radius);
  ShapeRefC            shape = capsule_shape.Create().Get();

  RVec3 pos(position.x, position.y + height * 0.5f + radius, position.z);
  Quat  rot = Quat::sIdentity();

  BodyCreationSettings settings(shape, pos, rot, EMotionType::Dynamic, layer);
  Body                *body = Physics::get_physics_system().GetBodyInterface().CreateBody(settings);
  Physics::get_physics_system().GetBodyInterface().AddBody(body->GetID(), EActivation::Activate);

  return body->GetID();
}
