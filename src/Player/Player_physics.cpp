#include "Physics/Physics.h"
#include "Player.h"

JPH::CharacterVirtual *
Player::create_character_virtual(glm::vec3 position, float height, float radius) {
  float                     half_cylinder = (height - 2.0f * radius) * 0.5f;
  JPH::CapsuleShapeSettings capsule_shape(half_cylinder, radius);
  JPH::ShapeRefC            shape = capsule_shape.Create().Get();

  JPH::CharacterVirtualSettings settings;
  settings.mShape            = shape;
  settings.mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -radius);
  settings.mMaxSlopeAngle    = JPH::DegreesToRadians(60.0f);
  settings.mMass             = 70.0f;

  JPH::RVec3 pos(position.x, position.y + height * 0.5f, position.z);
  JPH::Quat  rot = JPH::Quat::sIdentity();

  JPH::CharacterVirtual *character =
      new JPH::CharacterVirtual(&settings, pos, rot, 0, &Physics::get_physics_system());

  return character;
}
