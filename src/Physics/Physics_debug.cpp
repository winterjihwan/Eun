#include "Physics/Physics.h"

namespace Physics {
void submit_debug_lines() {
  JPH::BodyIDVector body_ids;
  Physics::get_physics_system().GetBodies(body_ids);

  const JPH::BodyLockInterface &body_lock_interface =
      Physics::get_physics_system().GetBodyLockInterface();

  for (const JPH::BodyID &id : body_ids) {
    const JPH::BodyLockRead lock(body_lock_interface, id);
    if (!lock.Succeeded())
      continue;

    const JPH::Body   &body      = lock.GetBody();
    const JPH::Shape  *shape     = body.GetShape();
    const JPH::RMat44 &transform = body.GetWorldTransform();

    JPH::RVec3 pos = transform.GetTranslation();
    JPH::Quat  rot = transform.GetRotation().GetQuaternion();

    JPH::TransformedShape transformed_shape(pos, rot, shape, body.GetID());

    JPH::Vec3  scale  = JPH::Vec3::sReplicate(1.0f);
    JPH::AABox bounds = shape->GetWorldSpaceBounds(transform, scale);

    JPH::Shape::GetTrianglesContext context;
    transformed_shape.GetTrianglesStart(context, bounds, JPH::RVec3::sZero());

    // TODO: Triangles are too expensive
    // constexpr int max_triangles = 256;
    // Float3        triangles[3 * max_triangles];
  }
}
} // namespace Physics
