#pragma once

#include "Animation.h"
#include "Bone.h"
#include "Types/Animation/Animdata.h"
#include <assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

class Animator {
public:
  Animator() {
    _current_animation  = 0;
    _previous_animation = 0;
    _current_time       = 0.0f;
    _previous_time      = 0.0f;
    _blend_time         = 0.0f;
    _blend_duration     = 0.0f;
    _blending           = false;

    _final_bone_matrices.resize(100, glm::mat4(1.0f));
  }

  void play_animation(Animation *new_animation, float blend_duration = 0.3f) {
    if (new_animation == _current_animation) {
      return;
    }

    _previous_animation = _current_animation;
    _current_animation  = new_animation;

    _previous_time = _current_time;
    _current_time  = 0.0f;

    _blend_duration = blend_duration;
    _blend_time     = 0.0f;
    _blending       = true;
  }

  void update_animation(float dt) {
    if (!_current_animation) {
      return;
    }

    _current_time += _current_animation->GetTicksPerSecond() * dt;
    _current_time = fmod(_current_time, _current_animation->GetDuration());

    if (_blending && _previous_animation) {
      _previous_time += _previous_animation->GetTicksPerSecond() * dt;
      _previous_time = fmod(_previous_time, _previous_animation->GetDuration());

      _blend_time += dt;
      float blend_factor = glm::clamp(_blend_time / _blend_duration, 0.0f, 1.0f);

      std::vector<glm::mat4> current_bones =
          calculate_bone_matrices(_current_animation, _current_time);
      std::vector<glm::mat4> previous_bones =
          calculate_bone_matrices(_previous_animation, _previous_time);

      for (size_t i = 0; i < _final_bone_matrices.size(); ++i) {
        _final_bone_matrices[i] = Util::lerp(previous_bones[i], current_bones[i], blend_factor);
      }

      if (blend_factor >= 1.0f) {
        _blending = false;
      }
    } else {
      _final_bone_matrices = calculate_bone_matrices(_current_animation, _current_time);
    }
  }

  std::vector<glm::mat4> get_final_bone_matrices() {
    return _final_bone_matrices;
  }

  glm::mat4 get_bone_global_transform(const std::string &boneName) const {
    auto it = _global_transforms.find(boneName);
    if (it != _global_transforms.end()) {
      return it->second;
    }

    std::cout << "Animator::GetBoneGlobalTransform fail, unknown bone name: " << boneName
              << std::endl;
    assert(0);
  }

  std::string &get_name() {
    return _name;
  }

private:
  std::vector<glm::mat4> calculate_bone_matrices(Animation *animation, float time) {
    std::vector<glm::mat4> transforms(100, glm::mat4(1.0f));

    std::function<void(const AssimpNodeData *, glm::mat4)> fn = [&](const AssimpNodeData *node,
                                                                    glm::mat4 parent_transform) {
      glm::mat4 node_transform = node->transformation;
      Bone     *bone           = animation->FindBone(node->name);

      if (bone) {
        bone->Update(time);
        node_transform = bone->GetLocalTransform();
      }

      glm::mat4 global_transform                    = parent_transform * node_transform;
      _global_transforms[node->name]                = global_transform;
      std::map<std::string, BoneInfo> bone_info_map = animation->GetBoneIDMap();

      if (bone_info_map.find(node->name) != bone_info_map.end()) {
        int       index   = bone_info_map[node->name].id;
        glm::mat4 offset  = bone_info_map[node->name].offset;
        transforms[index] = global_transform * offset;
      }

      for (int i = 0; i < node->childrenCount; i++) {
        fn(&node->children[i], global_transform);
      }
    };
    fn(&animation->GetRootNode(), glm::mat4(1.0f));

    return transforms;
  }

  std::string                      _name;
  std::vector<glm::mat4>           _final_bone_matrices;
  std::map<std::string, glm::mat4> _global_transforms;
  Animation                       *_current_animation;
  float                            _current_time;
  Animation                       *_previous_animation;
  float                            _previous_time;
  float                            _blend_time     = 0.0f;
  float                            _blend_duration = 0.0f;
  bool                             _blending       = false;
};
