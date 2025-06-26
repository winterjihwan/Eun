#include "Physics.h"

#include "Defines.h"

class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
public:
  BPLayerInterfaceImpl() {
    mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    mObjectToBroadPhase[Layers::MOVING]     = BroadPhaseLayers::MOVING;
  }

  uint GetNumBroadPhaseLayers() const override {
    return BroadPhaseLayers::NUM_LAYERS;
  }

  JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
    JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
    return mObjectToBroadPhase[inLayer];
  }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
  const char *GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override {
    switch ((BroadPhaseLayer::Type)inLayer) {
    case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
      return "NON_MOVING";
    case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
      return "MOVING";
    default:
      JPH_ASSERT(false);
      return "INVALID";
    }
  }
#endif

private:
  JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
  bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
    switch (inLayer1) {
    case Layers::NON_MOVING:
      return inLayer2 == BroadPhaseLayers::MOVING;
    case Layers::MOVING:
      return true;
    default:
      JPH_ASSERT(false);
      return false;
    }
  }
};

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
public:
  bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
    switch (inObject1) {
    case Layers::NON_MOVING:
      return inObject2 == Layers::MOVING;
    case Layers::MOVING:
      return true;
    default:
      JPH_ASSERT(false);
      return false;
    }
  }
};

class EunContactListener : public JPH::ContactListener {
  void OnContactAdded(const JPH::Body &body1,
                      const JPH::Body &body2,
                      const JPH::ContactManifold &,
                      JPH::ContactSettings &) override {
    auto it1 = Physics::get_contact_callbacks().find(body1.GetID());
    if (it1 != Physics::get_contact_callbacks().end())
      it1->second(body1.GetID(), body2.GetID());

    auto it2 = Physics::get_contact_callbacks().find(body2.GetID());
    if (it2 != Physics::get_contact_callbacks().end())
      it2->second(body2.GetID(), body1.GetID());
  }
};

namespace Physics {
static JPH::Factory                                 *_factory        = nullptr;
static JPH::TempAllocatorImpl                       *_temp_allocator = nullptr;
static JPH::JobSystemThreadPool                     *_job_system     = nullptr;
static BPLayerInterfaceImpl                          _broad_phase_if;
static ObjectVsBroadPhaseLayerFilterImpl             _obj_vs_broad_filter;
static ObjectLayerPairFilterImpl                     _obj_layer_filter;
static EunContactListener                            _contact_listener;
std::unordered_map<JPH::BodyID, PFN_ContactCallback> _contact_callbacks;

JPH::PhysicsSystem _physics_system;

void init() {
  JPH::RegisterDefaultAllocator();

  _factory                = new JPH::Factory();
  JPH::Factory::sInstance = _factory;
  JPH::RegisterTypes();

  _temp_allocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
  _job_system     = new JPH::JobSystemThreadPool(
      JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

  const uint32_t c_max_bodies              = 1024;
  const uint32_t c_num_body_mutexes        = 0;
  const uint32_t c_max_body_pairs          = 1024;
  const uint32_t c_max_contact_constraints = 1024;

  _physics_system.Init(c_max_bodies,
                       c_num_body_mutexes,
                       c_max_body_pairs,
                       c_max_contact_constraints,
                       _broad_phase_if,
                       _obj_vs_broad_filter,
                       _obj_layer_filter);

  _physics_system.SetGravity(JPH::Vec3(0.0f, GRAVITY, 0.0f));

  _physics_system.SetContactListener(&_contact_listener);
}

void update(float delta_time) {
  _physics_system.Update(delta_time, 1, _temp_allocator, _job_system);
}

void shutdown() {
  using namespace JPH;

  UnregisterTypes();

  delete _job_system;
  _job_system = nullptr;

  delete _temp_allocator;
  _temp_allocator = nullptr;

  delete _factory;
  _factory           = nullptr;
  Factory::sInstance = nullptr;
}

JPH::BodyID register_static_mesh(const std::vector<Vertex>       &vertices,
                                 const std::vector<unsigned int> &indices,
                                 const glm::mat4                 &transform,
                                 const ObjectType                 object_type,
                                 const uint64_t                   object_id) {
  JPH::VertexList jph_vertices;
  jph_vertices.reserve(vertices.size());

  // << Vertices
  for (const Vertex &v : vertices)
    jph_vertices.push_back(JPH::Float3(v.position.x, v.position.y, v.position.z));

  // << Triangles
  JPH::Array<JPH::IndexedTriangle> jph_triangles;
  for (size_t i = 0; i < indices.size(); i += 3) {
    jph_triangles.push_back(JPH::IndexedTriangle(indices[i], indices[i + 1], indices[i + 2]));
  }

  JPH::MeshShapeSettings shape_settings(jph_vertices, jph_triangles);
  shape_settings.SetEmbedded();
  JPH::ShapeRefC shape = shape_settings.Create().Get();

  // Get Transform
  JPH::RVec3 pos(transform[3].x, transform[3].y, transform[3].z);
  glm::quat  rotation = glm::quat_cast(transform);
  JPH::Quat  rot      = JPH::Quat(rotation.x, rotation.y, rotation.z, rotation.w);

  JPH::BodyCreationSettings settings(shape, pos, rot, JPH::EMotionType::Static, Layers::NON_MOVING);
  JPH::Body                *body = _physics_system.GetBodyInterface().CreateBody(settings);

  // << User Data
  PhysicsUserData user_data;
  user_data.physics_type = PhysicsType::RIGID_STATIC;
  user_data.object_type  = object_type;
  user_data.object_id    = object_id;
  body->SetUserData(reinterpret_cast<JPH::uint64>(new PhysicsUserData(user_data)));

  // JPH::Body Register
  _physics_system.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::DontActivate);

  return body->GetID();
}

std::optional<RayHitInfo> raycast(const JPH::Vec3 &origin, const JPH::Vec3 &dir, float dist) {
  const JPH::NarrowPhaseQuery &query = get_physics_system().GetNarrowPhaseQuery();
  JPH::RRayCast                ray(origin, dir * dist);
  JPH::RayCastResult           result;

  if (!query.CastRay(ray, result))
    return std::nullopt;

  const JPH::Body *body =
      Physics::get_physics_system().GetBodyLockInterface().TryGetBody(result.mBodyID);

  if (!body)
    return std::nullopt;

  JPH::RVec3            pos   = body->GetPosition();
  JPH::Quat             rot   = body->GetRotation();
  const JPH::Shape     *shape = body->GetShape();
  JPH::TransformedShape ts(pos, rot, shape, body->GetID());

  JPH::Vec3 hit_pos    = origin + dir * (result.mFraction * dist);
  JPH::Vec3 hit_normal = ts.GetWorldSpaceSurfaceNormal(result.mSubShapeID2, hit_pos);

  RayHitInfo ray_hit_info;
  ray_hit_info.body       = body;
  ray_hit_info.user_data  = reinterpret_cast<PhysicsUserData *>(body->GetUserData());
  ray_hit_info.ts         = ts;
  ray_hit_info.hit_pos    = hit_pos;
  ray_hit_info.hit_normal = hit_normal;
  ray_hit_info.ts         = ts;

  return ray_hit_info;
}

JPH::AABox get_aabb(const JPH::BodyID &body_id) {
  const JPH::BodyLockInterface &lock_interface = _physics_system.GetBodyLockInterface();
  JPH::BodyLockRead             lock(lock_interface, body_id);

  const JPH::Body  *body  = &lock.GetBody();
  const JPH::Shape *shape = body->GetShape();
  JPH::Mat44 transform = JPH::Mat44::sRotationTranslation(body->GetRotation(), body->GetPosition());
  return shape->GetWorldSpaceBounds(transform, JPH::Vec3::sReplicate(1.0f));
}

void register_on_contact(const JPH::BodyID &id, PFN_ContactCallback callback) {
  _contact_callbacks[id] = std::move(callback);
}

JPH::PhysicsSystem &get_physics_system() {
  return _physics_system;
}

JPH::TempAllocatorImpl *get_temp_allocator() {
  return _temp_allocator;
}

std::unordered_map<JPH::BodyID, PFN_ContactCallback> &get_contact_callbacks() {
  return _contact_callbacks;
}

} // namespace Physics
