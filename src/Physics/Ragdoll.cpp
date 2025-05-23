#include "Physics.h"

void Physics::ComputeBonePositions(const AssimpNodeData                       &node,
                                   const glm::mat4                            &parentTransform,
                                   std::unordered_map<std::string, glm::vec3> &outPositions,
                                   std::unordered_map<std::string, glm::mat4> &outTransforms) {
  glm::mat4 globalTransform = parentTransform * node.transformation;
  outPositions[node.name]   = glm::vec3(globalTransform[3]);
  outTransforms[node.name]  = globalTransform;
  for (const auto &child : node.children) {
    ComputeBonePositions(child, globalTransform, outPositions, outTransforms);
  }
}

void Physics::ExtractBonePairs(const AssimpNodeData                             &node,
                               std::vector<std::pair<std::string, std::string>> &outPairs) {
  for (const auto &child : node.children) {
    outPairs.emplace_back(node.name, child.name);
    ExtractBonePairs(child, outPairs);
  }
}

RagdollSettings *Physics::CreateRagdollFromModel(Model &model) {
  auto &root = model.GetRootNode();

  std::unordered_map<std::string, glm::vec3> positions;
  std::unordered_map<std::string, glm::mat4> transforms;
  ComputeBonePositions(root, glm::mat4(1.0f), positions, transforms);

  std::vector<std::pair<std::string, std::string>> bonePairs;
  ExtractBonePairs(root, bonePairs);

  Ref<Skeleton>                                    skeleton = new Skeleton;
  std::unordered_map<std::string, int>             boneNameToIndex;
  std::function<void(const AssimpNodeData &, int)> BuildSkeleton;
  BuildSkeleton = [&](const AssimpNodeData &node, int parent) {
    int idx                    = skeleton->AddJoint(node.name, parent);
    boneNameToIndex[node.name] = idx;
    for (const auto &child : node.children)
      BuildSkeleton(child, idx);
  };
  BuildSkeleton(root, -1);

  auto *settings      = new RagdollSettings;
  settings->mSkeleton = skeleton;
  settings->mParts.resize(skeleton->GetJointCount());

  for (auto &[parent, child] : bonePairs) {
    if (positions[parent] == positions[child])
      continue;
    int idx = boneNameToIndex[child];

    glm::vec3 p0   = positions[parent];
    glm::vec3 p1   = positions[child];
    glm::vec3 mid  = 0.5f * (p0 + p1);
    glm::vec3 axis = glm::normalize(p1 - p0);
    JPH::Vec3 jph_axis(axis.x, axis.y, axis.z);
    float     len = glm::length(p1 - p0);

    if (glm::length(p1 - p0) < 1e-5f)
      continue;

    auto shape = new CapsuleShape(len * 0.5f, len * 0.1f);
    settings->mParts[idx].SetShape(shape);
    settings->mParts[idx].mPosition = JPH::RVec3(mid.x, mid.y, mid.z);
    settings->mParts[idx].mRotation =
        JPH::Quat::sFromTo(JPH::Vec3::sAxisY(), jph_axis).Normalized();
    settings->mParts[idx].mMotionType  = EMotionType::Dynamic;
    settings->mParts[idx].mObjectLayer = Layers::MOVING;

    auto *constraint       = new SwingTwistConstraintSettings();
    constraint->mPosition1 = constraint->mPosition2 = JPH::Vec3(mid.x, mid.y, mid.z);
    constraint->mTwistAxis1                         = constraint->mTwistAxis2 =
        JPH::Vec3(axis.x, axis.y, axis.z).Normalized();
    constraint->mPlaneAxis1 = constraint->mPlaneAxis2 =
        constraint->mTwistAxis1.GetNormalizedPerpendicular();
    constraint->mTwistMinAngle       = -JPH::DegreesToRadians(45.0f);
    constraint->mTwistMaxAngle       = JPH::DegreesToRadians(45.0f);
    constraint->mNormalHalfConeAngle = JPH::DegreesToRadians(45.0f);

    settings->mParts[idx].mToParent = constraint;
  }

  for (const auto &[name, idx] : boneNameToIndex) {
    if (settings->mParts[idx].GetShape() != nullptr)
      continue;

    glm::vec3 pos    = positions[name];
    float     radius = 0.05f;
    float     height = 0.2f;
    auto      shape  = new CapsuleShape(height * 0.5f, radius);

    settings->mParts[idx].SetShape(shape);
    settings->mParts[idx].mPosition    = JPH::RVec3(pos.x, pos.y, pos.z);
    settings->mParts[idx].mRotation    = JPH::Quat::sIdentity().Normalized();
    settings->mParts[idx].mMotionType  = EMotionType::Dynamic;
    settings->mParts[idx].mObjectLayer = Layers::MOVING;
  }

  settings->Stabilize();
  settings->DisableParentChildCollisions();
  settings->CalculateBodyIndexToConstraintIndex();
  return settings;
}
