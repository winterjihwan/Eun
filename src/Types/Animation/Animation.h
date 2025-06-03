#pragma once

#include "Bone.h"
#include "Model.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>

class Animation {
public:
  std::string _path;
  std::string _name;
  std::string _right_hand_name;

  Animation() = default;

  Animation(const std::string &name, const std::string &animationPath, Model *model) {
    _name = name;
    _path = animationPath;
    Assimp::Importer _importer;
    const aiScene   *scene = _importer.ReadFile(animationPath, aiProcess_Triangulate);
    if (!scene) {
      std::cout << "Animation::Animation() failed, no scene with animation path " << animationPath
                << std::endl;
      assert(0);
    }
    aiAnimation *animation = scene->mAnimations[0];
    m_Duration             = animation->mDuration;
    m_TicksPerSecond       = animation->mTicksPerSecond;

    ReadHierarchyData(m_RootNode, scene->mRootNode);
    ReadMissingBones(animation, *model);
    FindRightHandBoneName();
  }

  ~Animation() {
  }

  Bone *FindBone(const std::string &name) {
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone &Bone) {
      return Bone.GetBoneName() == name;
    });
    if (iter == m_Bones.end())
      return nullptr;
    else
      return &(*iter);
  }

  inline float GetTicksPerSecond() {
    return m_TicksPerSecond;
  }
  inline float GetDuration() {
    return m_Duration;
  }
  inline const AssimpNodeData &GetRootNode() {
    return m_RootNode;
  }
  inline const std::map<std::string, BoneInfo> &GetBoneIDMap() {
    return m_BoneInfoMap;
  }

private:
  void ReadMissingBones(const aiAnimation *animation, Model &model) {
    int size = animation->mNumChannels;

    auto &boneInfoMap = model.get_bone_info_map();
    int  &boneCount   = model.get_bone_count();

    for (int i = 0; i < size; i++) {
      auto        channel  = animation->mChannels[i];
      std::string boneName = channel->mNodeName.data;

      if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
        boneInfoMap[boneName].id = boneCount;
        boneCount++;
      }
      m_Bones.push_back(
          Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
  }

  void FindRightHandBoneName() {
    for (const Bone &bone : m_Bones) {
      std::string name  = bone.GetBoneName();
      std::string lower = name;
      std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

      if (lower.find("right") != std::string::npos && lower.find("hand") != std::string::npos) {
        _right_hand_name = name;
        return;
      }
    }

    std::cout << "Animation::FindLeftHandBoneName(): failed to find right hand bone" << std::endl;
    assert(0);
  }

  void ReadHierarchyData(AssimpNodeData &dest, const aiNode *src) {
    assert(src);

    dest.name           = src->mName.data;
    dest.transformation = AssimpUtils::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount  = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++) {
      AssimpNodeData newData;
      ReadHierarchyData(newData, src->mChildren[i]);
      dest.children.push_back(newData);
    }
  }
  float                           m_Duration;
  int                             m_TicksPerSecond;
  std::vector<Bone>               m_Bones;
  AssimpNodeData                  m_RootNode;
  std::map<std::string, BoneInfo> m_BoneInfoMap;
};
