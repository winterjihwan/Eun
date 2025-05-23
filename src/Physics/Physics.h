#pragma once

#include "Mesh.h"
#include "Model.h"
#include "Types/Animation/Assimp_utils.h"
#include <Jolt/Jolt.h>
#include <vector>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>

#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Constraints/SwingTwistConstraint.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/RegisterTypes.h>

using namespace JPH;

namespace Physics {
using PFN_ContactCallback = std::function<void(const JPH::BodyID &self, const JPH::BodyID &other)>;

void init();
void update(float delta_time);
void shutdown();

void register_static_mesh(const std::vector<Vertex>       &vertices,
                          const std::vector<unsigned int> &indices,
                          const glm::mat4                 &transform);

void register_on_contact(const JPH::BodyID &id, PFN_ContactCallback callback);

// Ragdoll
void             ComputeBonePositions(const AssimpNodeData                       &node,
                                      const glm::mat4                            &parentTransform,
                                      std::unordered_map<std::string, glm::vec3> &outPositions,
                                      std::unordered_map<std::string, glm::mat4> &outTransforms);
void             ExtractBonePairs(const AssimpNodeData                             &node,
                                  std::vector<std::pair<std::string, std::string>> &outPairs);
RagdollSettings *CreateRagdollFromModel(Model &model);

RagdollSettings *create_ragdoll();

PhysicsSystem                                   &get_physics_system();
std::unordered_map<BodyID, PFN_ContactCallback> &get_contact_callbacks();
TempAllocatorImpl                               *get_temp_allocator();

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
