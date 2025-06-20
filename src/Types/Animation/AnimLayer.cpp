#include "AnimLayer.h"
#include "AnimState.h"
#include "AssetManager/AssetManager.h"

void AnimLayer::update(float delta_time) {
  // Clean up
  for (int i = 0; i < _anim_states.size(); i++) {
    if (_anim_states[i].awaiting_removal()) {
      _anim_states.erase(_anim_states.begin() + i);
      i--;
    }
  }

  for (AnimState &anim_state : _anim_states) {
    anim_state.update(_skinned_model_index, delta_time);
  }

  if (_anim_states.size()) {
    _blended_bone_transforms = _anim_states[0].get_bone_transforms();
  }

  // TODO: Blend animation states
}

void AnimLayer::play_animation(const std::string &name, const AnimPlaybackParams &params) {
  int anim_index = AssetManager::get_animation_index_by_name(name);

  if (params.remove_existing_animations) {
    _anim_states.clear();
  }

  // Replay
  for (AnimState &anim_state : _anim_states) {
    if (anim_state.get_anim_index() == anim_index) {
      anim_state.play_animation(name, params);
      return;
    }
  }

  AnimState &anim_state = _anim_states.emplace_back();
  anim_state.play_animation(name, params);
}

void AnimLayer::loop_animation(const std::string &name, const AnimPlaybackParams &params) {
  int anim_index = AssetManager::get_animation_index_by_name(name);

  if (params.remove_existing_animations) {
    _anim_states.erase(std::remove_if(_anim_states.begin(),
                                      _anim_states.end(),
                                      [&](AnimState &anim_state) {
                                        return anim_state.get_anim_index() != anim_index;
                                      }),
                       _anim_states.end());
  }

  // Already Playing
  for (AnimState &anim_state : _anim_states) {
    if (anim_state.get_anim_index() == anim_index) {
      return;
    }
  }

  AnimState &anim_state = _anim_states.emplace_back();
  anim_state.loop_animation(name, params);
}

void AnimLayer::set_skinned_model(const std::string &name) {
  int index            = AssetManager::get_skinned_model_index_by_name(name);
  _skinned_model_index = index;
  // _joint_count =
  //     AssetManager::get_skinned_model_by_index(_skinned_model_index)->get_bone_infos().size();
}

std::vector<glm::mat4> &AnimLayer::get_blended_bone_transforms() {
  return _blended_bone_transforms;
}

std::vector<AnimState> &AnimLayer::get_anim_states() {
  return _anim_states;
}
