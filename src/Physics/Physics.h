#pragma once

#include <Jolt/Jolt.h>

#include "Enums.h"
#include "Types.h"
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Constraints/SwingTwistConstraint.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/RegisterTypes.h>
#include <vector>

namespace Physics {
using PFN_ContactCallback = std::function<void(const JPH::BodyID &self, const JPH::BodyID &other)>;

void init();
void update(float delta_time);
void shutdown();

JPH::BodyID            register_static_mesh(std::vector<Vertex>       &vertices,
                                            std::vector<unsigned int> &indices,
                                            glm::mat4                 &transform,
                                            ObjectType                 object_type,
                                            uint64_t                   object_id);
JPH::BodyID            register_kinematic_collider(JPH::ShapeRefC shape,
                                                   glm::vec3     &position,
                                                   glm::quat     &rotation,
                                                   ObjectType     object_type,
                                                   uint64_t       object_id);
JPH::CharacterVirtual *create_character_virtual(
    glm::vec3 position, float height, float radius, ObjectType object_type, uint64_t object_id);
JPH::AABox                get_aabb(const JPH::BodyID &body_id);
std::optional<RayHitInfo> raycast(const glm::vec3 &origin, const glm::vec3 &dir, float dist);
std::optional<RayHitInfo> raycast(const JPH::Vec3 &origin, const JPH::Vec3 &dir, float dist);
void                      set_body_velocity(JPH::BodyID body_id, const glm::vec3 &velocity);
void                      register_on_contact(const JPH::BodyID &id, PFN_ContactCallback callback);
void                      submit_debug_lines();

JPH::PhysicsSystem                                   &get_physics_system();
std::unordered_map<JPH::BodyID, PFN_ContactCallback> &get_contact_callbacks();
JPH::TempAllocatorImpl                               *get_temp_allocator();

} // namespace Physics

namespace Layers {
static constexpr JPH::ObjectLayer NON_MOVING = 0;
static constexpr JPH::ObjectLayer MOVING     = 1;
static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}; // namespace Layers

namespace BroadPhaseLayers {
static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
static constexpr JPH::BroadPhaseLayer MOVING(1);
static constexpr uint                 NUM_LAYERS = 2;
}; // namespace BroadPhaseLayers
