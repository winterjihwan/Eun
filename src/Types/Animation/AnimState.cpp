#include "AnimState.h"
#include "AssetManager/AssetManager.h"
#include "Types/Animation/Animation.h"
#include "Types/Renderer/SkinnedModel.h"

void AnimState::update(int skinned_model_index, float delta_time) {
  Animation    *animation     = AssetManager::get_animation_by_index(_anim_index);
  SkinnedModel *skinned_model = AssetManager::get_skinned_model_by_index(skinned_model_index);

  if (animation) {
    if (!_paused) {
      _current_time += delta_time * _playback_params.speed;
    }
    float duration = animation->get_duration() / animation->get_ticks_per_second();
    if (_current_time >= duration) {
      if (!_loop) {
        _current_time = duration;
        _paused       = true;
        _complete     = true;
        return;
      } else {
        _current_time = 0;
      }
    }

    float time_ticks = animation->calc_animation_time_ticks(_current_time, 0);
    skinned_model->get_bone_transforms(animation, time_ticks, _bone_transforms);
  }
}

void AnimState::play_animation(const std::string &name, const AnimPlaybackParams &params) {
  int index        = AssetManager::get_animation_index_by_name(name);
  _anim_index      = index;
  _current_time    = 0;
  _loop            = false;
  _paused          = false;
  _complete        = false;
  _playback_params = params;
  _blend_factor    = params.blend_weight;
  return;
}

void AnimState::loop_animation(const std::string &name, const AnimPlaybackParams &params) {
  int index = AssetManager::get_animation_index_by_name(name);

  if (index != _anim_index) {
    _playback_params = params;
  }

  _anim_index                   = index;
  _current_time                 = 0;
  _loop                         = true;
  _paused                       = false;
  _complete                     = false;
  _playback_params.blend_weight = params.blend_weight;
  _blend_factor                 = params.blend_weight;
  return;
}

bool AnimState::is_complete() {
  return _complete;
}

bool AnimState::awaiting_removal() {
  if (_playback_params.remove_on_complete && is_complete()) {
    return true;
  }

  if (_blend_factor == 0) {
    return true;
  }

  return false;
}

int AnimState::get_anim_index() {
  return _anim_index;
}

std::vector<glm::mat4> &AnimState::get_bone_transforms() {
  return _bone_transforms;
}
