#include "Animation.h"
#include "Defines.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Animation::init(std::string name, std::string path) {
  _name     = name;
  _importer = std::make_unique<Assimp::Importer>();
  _scene    = _importer->ReadFile(path, ASSIMP_LOAD_FLAGS_ANIMATION);

  // if (_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
  //   std::cout << "Model::load(), AI_SCENE_FLAGS_INCOMPLETE set: " << _importer->GetErrorString()
  //             << std::endl;
  //   return;
  // };

  if (!_scene) {
    std::cout << "Model::load(), no scene: " << _importer->GetErrorString() << std::endl;
    return;
  }

  // HACK: 0 Animation Index
  int animation_index = 0;
  if (animation_index >= _scene->mNumAnimations) {
    std::cout << "SkinnedModel::get_bone_transforms() error, no animation index " << animation_index
              << ". Max:  " << _scene->mNumAnimations << std::endl;
    assert(0);
  }

  // TODO: Take index as an argument
  const aiAnimation *animation = _scene->mAnimations[0];

  _node_anim_map.clear();
  for (uint i = 0; i < animation->mNumChannels; ++i) {
    aiNodeAnim *node_anim = animation->mChannels[i];

    _node_anim_map[node_anim->mNodeName.C_Str()] = node_anim;
  }

  _duration         = animation->mDuration;
  _ticks_per_second = animation->mTicksPerSecond;
}

aiNodeAnim *Animation::find_node_anim(const std::string &name) {
  return _node_anim_map.contains(name) ? _node_anim_map[name] : 0;
}

float Animation::calc_animation_time_ticks(float time_seconds, unsigned int animation_index) {
  float ticks_per_second = (float)(_scene->mAnimations[animation_index]->mTicksPerSecond != 0
                                       ? _scene->mAnimations[animation_index]->mTicksPerSecond
                                       : 25.0f);
  float time_ticks       = time_seconds * ticks_per_second;

  float duration             = (float)_scene->mAnimations[animation_index]->mDuration;
  float animation_time_ticks = fmod(time_ticks, duration);
  return animation_time_ticks;
}
