#include "Physics/Physics.h"

namespace Physics {
void submit_debug_lines() {
  JPH::BodyIDVector body_ids;
  Physics::get_physics_system().GetBodies(body_ids);

  const BodyLockInterface &body_lock_interface =
      Physics::get_physics_system().GetBodyLockInterface();

  for (const JPH::BodyID &id : body_ids) {
    const JPH::BodyLockRead lock(body_lock_interface, id);
    if (!lock.Succeeded())
      continue;

    const Body   &body      = lock.GetBody();
    const Shape  *shape     = body.GetShape();
    const RMat44 &transform = body.GetWorldTransform();

    RVec3 pos = transform.GetTranslation();
    Quat  rot = transform.GetRotation().GetQuaternion();

    TransformedShape transformed_shape(pos, rot, shape, body.GetID());

    Vec3  scale  = Vec3::sReplicate(1.0f);
    AABox bounds = shape->GetWorldSpaceBounds(transform, scale);

    Shape::GetTrianglesContext context;
    transformed_shape.GetTrianglesStart(context, bounds, RVec3::sZero());

    // TODO: Triangles are too expensive
    // constexpr int max_triangles = 256;
    // Float3        triangles[3 * max_triangles];
  }
}
} // namespace Physics
