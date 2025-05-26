#include "Physics.h"

#include "Defines.h"

using namespace JPH;

class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface {
public:
  BPLayerInterfaceImpl() {
    mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    mObjectToBroadPhase[Layers::MOVING]     = BroadPhaseLayers::MOVING;
  }

  uint GetNumBroadPhaseLayers() const override {
    return BroadPhaseLayers::NUM_LAYERS;
  }

  BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override {
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
  BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter {
public:
  bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override {
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

class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter {
public:
  bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override {
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

class EunContactListener : public ContactListener {
  void OnContactAdded(const Body &body1,
                      const Body &body2,
                      const ContactManifold &,
                      ContactSettings &) override {
    auto it1 = Physics::get_contact_callbacks().find(body1.GetID());
    if (it1 != Physics::get_contact_callbacks().end())
      it1->second(body1.GetID(), body2.GetID());

    auto it2 = Physics::get_contact_callbacks().find(body2.GetID());
    if (it2 != Physics::get_contact_callbacks().end())
      it2->second(body2.GetID(), body1.GetID());
  }
};

namespace Physics {
static Factory                                 *_factory        = nullptr;
static TempAllocatorImpl                       *_temp_allocator = nullptr;
static JobSystemThreadPool                     *_job_system     = nullptr;
static BPLayerInterfaceImpl                     _broad_phase_if;
static ObjectVsBroadPhaseLayerFilterImpl        _obj_vs_broad_filter;
static ObjectLayerPairFilterImpl                _obj_layer_filter;
static EunContactListener                       _contact_listener;
std::unordered_map<BodyID, PFN_ContactCallback> _contact_callbacks;

PhysicsSystem _physics_system;

void init() {
  RegisterDefaultAllocator();

  _factory           = new Factory();
  Factory::sInstance = _factory;
  RegisterTypes();

  _temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);
  _job_system     = new JobSystemThreadPool(
      cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

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

  _physics_system.SetGravity(Vec3(0.0f, GRAVITY, 0.0f));

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

void register_static_mesh(const std::vector<Vertex>       &vertices,
                          const std::vector<unsigned int> &indices,
                          const glm::mat4                 &transform) {
  VertexList jph_vertices;
  jph_vertices.reserve(vertices.size());
  for (const Vertex &v : vertices)
    jph_vertices.push_back(Float3(v.position.x, v.position.y, v.position.z));

  Array<IndexedTriangle> jph_triangles;
  for (size_t i = 0; i < indices.size(); i += 3) {
    jph_triangles.push_back(IndexedTriangle(indices[i], indices[i + 1], indices[i + 2]));
  }

  MeshShapeSettings shape_settings(jph_vertices, jph_triangles);
  shape_settings.SetEmbedded();
  ShapeRefC shape = shape_settings.Create().Get();

  RVec3 pos(transform[3].x, transform[3].y, transform[3].z);
  Quat  rot = Quat::sIdentity();

  BodyCreationSettings settings(shape, pos, rot, EMotionType::Static, Layers::NON_MOVING);
  Body                *body = _physics_system.GetBodyInterface().CreateBody(settings);

  _physics_system.GetBodyInterface().AddBody(body->GetID(), EActivation::DontActivate);
}

void register_on_contact(const JPH::BodyID &id, PFN_ContactCallback callback) {
  _contact_callbacks[id] = std::move(callback);
}

PhysicsSystem &get_physics_system() {
  return _physics_system;
}

TempAllocatorImpl *get_temp_allocator() {
  return _temp_allocator;
}

std::unordered_map<BodyID, PFN_ContactCallback> &get_contact_callbacks() {
  return _contact_callbacks;
}

void a() {
}

} // namespace Physics
