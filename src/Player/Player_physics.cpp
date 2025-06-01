#include "Physics/Physics.h"
#include "Player.h"

JPH::CharacterVirtual *
Player::create_character_virtual(glm::vec3 position, float height, float radius) {
  float                half_cylinder = (height - 2.0f * radius) * 0.5f;
  CapsuleShapeSettings capsule_shape(half_cylinder, radius);
  ShapeRefC            shape = capsule_shape.Create().Get();

  CharacterVirtualSettings settings;
  settings.mShape            = shape;
  settings.mSupportingVolume = Plane(Vec3::sAxisY(), -radius);
  settings.mMaxSlopeAngle    = DegreesToRadians(60.0f);
  settings.mMass             = 70.0f;

  RVec3 pos(position.x, position.y + height * 0.5f, position.z);
  Quat  rot = Quat::sIdentity();

  CharacterVirtual *character =
      new CharacterVirtual(&settings, pos, rot, 0, &Physics::get_physics_system());

  return character;
}
