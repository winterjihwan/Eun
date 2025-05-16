#pragma once

#include "Mesh.h"
#include <Jolt/Jolt.h>
#include <vector>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>

using namespace JPH;

namespace Physics {
using ContactCallback = std::function<void(const JPH::BodyID &self, const JPH::BodyID &other)>;

void init();
void update(float delta_time);
void shutdown();

void   register_static_mesh(const std::vector<Vertex>       &vertices,
                            const std::vector<unsigned int> &indices,
                            const glm::mat4                 &transform);
BodyID create_dynamic_body(glm::vec3 position, float radius, ObjectLayer layer);
void   register_on_contact(const JPH::BodyID &id, ContactCallback callback);

PhysicsSystem &get_physics_system();

} // namespace Physics

namespace Layers {
static constexpr ObjectLayer NON_MOVING = 0;
static constexpr ObjectLayer MOVING     = 1;
static constexpr ObjectLayer NUM_LAYERS = 2;
}; // namespace Layers

namespace BroadPhaseLayers {
static constexpr BroadPhaseLayer NON_MOVING(0);
static constexpr BroadPhaseLayer MOVING(1);
static constexpr uint            NUM_LAYERS = 2;
}; // namespace BroadPhaseLayers
