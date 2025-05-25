#include "Physics.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using namespace JPH;
using json = nlohmann::json;

static Vec3 Vec3FromJSONArray(const json &arr) {
  return Vec3(arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>());
}

static Quat QuatFromJSONArray(const json &arr) {
  return Quat(arr[3].get<float>(), arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>());
}

static Mat44 Mat4FromJSONArray(const json &arr) {
  Mat44 m;
  for (int i = 0; i < 16; ++i)
    m(i % 4, i / 4) = arr[i].get<float>();
  return m;
}

Ref<RagdollSettings> Physics::LoadRagdollFromJSON(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open ragdoll file: " << filename << std::endl;
    return nullptr;
  }

  json doc;
  file >> doc;

  auto settings       = new RagdollSettings();
  settings->mSkeleton = new Skeleton();

  std::unordered_map<int, int>         id_to_joint;
  std::unordered_map<int, std::string> id_to_name;
  std::unordered_map<std::string, int> name_to_index;
  std::unordered_map<int, json>        id_to_rigid;

  for (auto &[id_str, entity] : doc["entities"].items()) {
    int   id    = std::stoi(id_str);
    auto &comps = entity["components"];
    if (!comps.contains("RigidComponent"))
      continue;

    std::string name        = comps["NameComponent"]["members"]["value"];
    std::string path        = comps["NameComponent"]["members"]["path"];
    std::string parent_name = path.substr(path.rfind("|") + 1);

    int parent_index = -1;
    if (name_to_index.count(parent_name)) {
      parent_index = name_to_index[parent_name];
    }

    int joint_idx       = settings->mSkeleton->AddJoint(name, parent_index);
    id_to_joint[id]     = joint_idx;
    id_to_name[id]      = name;
    name_to_index[name] = joint_idx;
    id_to_rigid[id]     = entity;
  }

  settings->mParts.resize(settings->mSkeleton->GetJointCount());

  for (auto &[id, joint_index] : id_to_joint) {
    auto &entity = id_to_rigid[id];
    auto &comps  = entity["components"];
    auto &geom   = comps["GeometryDescriptionComponent"]["members"];

    RagdollSettings::Part &part = settings->mParts[joint_index];

    Vec3 offset   = Vec3FromJSONArray(geom["offset"]["values"]);
    Quat rotation = QuatFromJSONArray(geom["rotation"]["values"]).Normalized();
    std::cout << "rotation: (" << rotation.GetX() << ", " << rotation.GetY() << ", "
              << rotation.GetZ() << ", " << rotation.GetW() << ")\n";
    Vec3 pos =
        Mat4FromJSONArray(comps["RestComponent"]["members"]["matrix"]["values"]).GetTranslation();

    part.mPosition    = pos;
    part.mRotation    = rotation;
    part.mMotionType  = EMotionType::Dynamic;
    part.mObjectLayer = 0;

    std::string shape_type = geom["type"];
    if (shape_type == "Capsule") {
      float radius = geom["radius"];
      float length = geom["length"];
      part.SetShape(new CapsuleShape(length * 0.5f, radius));
    } else if (shape_type == "Box") {
      Vec3 extents = Vec3FromJSONArray(geom["extents"]["values"]);
      part.SetShape(new BoxShape(extents * 0.5f));
    }
  }

  for (auto &[id_str, entity] : doc["entities"].items()) {
    auto &comps = entity["components"];
    if (!comps.contains("JointComponent"))
      continue;

    int parent_id = comps["JointComponent"]["members"]["parent"]["value"];
    int child_id  = comps["JointComponent"]["members"]["child"]["value"];

    if (!id_to_joint.count(parent_id) || !id_to_joint.count(child_id))
      continue;

    int parent_joint = id_to_joint[parent_id];
    int child_joint  = id_to_joint[child_id];

    SwingTwistConstraintSettings *constraint = new SwingTwistConstraintSettings();
    constraint->mDrawConstraintSize          = 0.1f;

    auto childFrame = Mat4FromJSONArray(comps["JointComponent"]["members"]["childFrame"]["values"]);
    auto parentFrame =
        Mat4FromJSONArray(comps["JointComponent"]["members"]["parentFrame"]["values"]);

    constraint->mPosition1  = parentFrame.GetTranslation();
    constraint->mPosition2  = childFrame.GetTranslation();
    constraint->mTwistAxis1 = Vec3::sAxisX();
    constraint->mPlaneAxis1 = Vec3::sAxisY();
    constraint->mTwistAxis2 = Vec3::sAxisX();
    constraint->mPlaneAxis2 = Vec3::sAxisY();

    auto &limits                     = comps["LimitComponent"]["members"];
    constraint->mTwistMinAngle       = -DegreesToRadians(limits["twist"]);
    constraint->mTwistMaxAngle       = DegreesToRadians(limits["twist"]);
    constraint->mNormalHalfConeAngle = DegreesToRadians(limits["swing1"]);
    constraint->mPlaneHalfConeAngle  = DegreesToRadians(limits["swing2"]);

    settings->mParts[child_joint].mToParent = constraint;
  }

  settings->DisableParentChildCollisions();
  settings->Stabilize();
  settings->CalculateBodyIndexToConstraintIndex();

  return settings;
}
