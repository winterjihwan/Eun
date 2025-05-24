#include "RagdollBuilder.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr uint INVALID_JOINT = ~0u;

RagdollSettings *RagdollBuilder::Create(const Model &model, const std::string &ragdollJsonPath) {
  std::ifstream file(ragdollJsonPath);
  if (!file.is_open()) {
    std::cerr << "Failed to open ragdoll config: " << ragdollJsonPath << std::endl;
    return nullptr;
  }

  json j;
  file >> j;

  RagdollSettings *settings = new RagdollSettings();
  Ref<Skeleton>    skeleton = new Skeleton();

  std::map<std::string, int> boneIndexMap;

  for (const auto &bone : j["bones"]) {
    std::string name     = bone["name"];
    std::string parent   = bone["parent"];
    uint        parentId = parent.empty() ? INVALID_JOINT : skeleton->GetJointIndex(parent);
    uint        jointId  = skeleton->AddJoint(name, parentId);
    boneIndexMap[name]   = jointId;
  }

  settings->mSkeleton = skeleton;
  settings->mParts.resize(skeleton->GetJointCount());

  for (const auto &bone : j["bones"]) {
    std::string name  = bone["name"];
    int         index = boneIndexMap[name];

    auto  offset = bone["offset"];
    RVec3 pos(offset[0], offset[1], offset[2]);

    float radius   = bone["radius"];
    float length   = bone["length"];
    auto  rotation = bone["rotation"];

    Ref<Shape> shape = new CapsuleShape(length * 0.5f, radius);
    Quat       rot(rotation[0], rotation[1], rotation[2], rotation[3]);
    std::cout << "Quatyy (" << name << "): x=" << rot.GetX() << ", y=" << rot.GetY()
              << ", z=" << rot.GetZ() << ", w=" << rot.GetW() << std::endl;

    rot = rot.Normalized();

    auto &part = settings->mParts[index];
    part.SetShape(shape);
    part.mPosition    = pos;
    part.mRotation    = rot;
    part.mMotionType  = EMotionType::Dynamic;
    part.mObjectLayer = Layers::MOVING;

    std::string parent           = bone["parent"];
    auto        parentIt         = boneIndexMap.find(parent);
    bool        has_valid_parent = parentIt != boneIndexMap.end();

    if (has_valid_parent) {
      SwingTwistConstraintSettings *constraint = new SwingTwistConstraintSettings();
      constraint->mDrawConstraintSize          = 0.1f;
      constraint->mPosition1 = constraint->mPosition2 = pos;
      constraint->mTwistAxis1 = constraint->mTwistAxis2 = Vec3::sAxisY().Normalized();
      constraint->mPlaneAxis1 = constraint->mPlaneAxis2 = Vec3::sAxisZ().Normalized();

      constraint->mTwistMinAngle       = -DegreesToRadians((float)bone["twist_limit"]);
      constraint->mTwistMaxAngle       = DegreesToRadians((float)bone["twist_limit"]);
      constraint->mNormalHalfConeAngle = DegreesToRadians((float)bone["swing1"]);
      constraint->mPlaneHalfConeAngle  = DegreesToRadians((float)bone["swing2"]);

      part.mToParent = constraint;
    }
  }

  settings->Stabilize();
  settings->DisableParentChildCollisions();
  settings->CalculateBodyIndexToConstraintIndex();

  return settings;
}
