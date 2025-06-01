#pragma once

#include "Animation.h"
#include "Bone.h"
#include <__assert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

class Animator {
public:
  Animator() = default;

  Animator(Animation *animation) {
    m_Name = animation->_name;

    m_CurrentTime      = 0.0f;
    m_CurrentAnimation = animation;
    m_ClipStart        = 0.0f;
    m_ClipEnd          = animation->GetDuration();

    m_FinalBoneMatrices.resize(100, glm::mat4(1.0f));
  }

  void UpdateAnimation(float dt) {
    m_DeltaTime = dt;
    if (m_CurrentAnimation) {
      float clipLength = m_ClipEnd - m_ClipStart;

      m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;

      if (m_CurrentTime < m_ClipStart) {
        m_CurrentTime = m_ClipStart;
      }

      if (m_CurrentTime > m_ClipEnd) {
        if (m_Loop) {
          m_CurrentTime = m_ClipStart + fmod(m_CurrentTime - m_ClipStart, clipLength);
        } else {
          m_Done        = true;
          m_CurrentTime = m_ClipEnd - 1; // HACK
        }
      }

      CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
  }

  void PlayAnimation() {
    m_CurrentTime = 0.0f;
  }

  void CalculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform) {
    const std::string &nodeName      = node->name;
    glm::mat4          nodeTransform = node->transformation;

    Bone *Bone = m_CurrentAnimation->FindBone(nodeName);

    if (Bone) {
      Bone->Update(m_CurrentTime);
      nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    m_GlobalTransforms[nodeName] = globalTransformation;

    const auto &boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
      int       index            = boneInfoMap.at(nodeName).id;
      glm::mat4 offset           = boneInfoMap.at(nodeName).offset;
      m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
      CalculateBoneTransform(&node->children[i], globalTransformation);
  }

  void SetClip(float clipStart, float clipEnd) {
    m_ClipStart = clipStart * 1000.0f;
    m_ClipEnd   = clipEnd * 1000.0f;
  }

  std::vector<glm::mat4> GetFinalBoneMatrices() {
    return m_FinalBoneMatrices;
  }

  glm::mat4 GetBoneGlobalTransform(const std::string &boneName) const {
    auto it = m_GlobalTransforms.find(boneName);
    if (it != m_GlobalTransforms.end()) {
      return it->second;
    }

    std::cout << "Animator::GetBoneGlobalTransform fail, unknown bone name: " << boneName
              << std::endl;
    assert(0);
  }

  std::string &GetName() {
    return m_Name;
  }

  void SetIsLoop(bool loop) {
    m_Loop = loop;
  }

  bool IsDone() {
    if (m_Done) {
      // TODO: Reset in a separate function
      m_CurrentTime = 0.0f;
      m_Done        = false;
      return true;
    }
    return false;
  }

private:
  std::string                      m_Name;
  bool                             m_Loop = true;
  bool                             m_Done = false;
  std::vector<glm::mat4>           m_FinalBoneMatrices;
  std::map<std::string, glm::mat4> m_GlobalTransforms;
  Animation                       *m_CurrentAnimation;
  float                            m_CurrentTime;
  float                            m_DeltaTime;
  float                            m_ClipStart;
  float                            m_ClipEnd;
};
