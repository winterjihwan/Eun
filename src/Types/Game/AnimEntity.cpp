#include "AnimEntity.h"
#include "AssetManager/AssetManager.h"
#include "Renderer/RenderDataManager.h"
#include "Util/Util.h"
#include <glm/gtx/quaternion.hpp>

void AnimEntity::update(float delta_time) {
  if (!_skinned_model) {
    return;
  }

  _anim_layer.update(delta_time);
  _global_blended_bone_transforms = _anim_layer.get_blended_bone_transforms();

  if (_blending) {
    _blend_time += delta_time;
    float t = _blend_time / _blend_duration;

    if (t >= 1.0f) {
      _blending = false;
      t         = 1.0f;
    }

    const std::vector<glm::mat4> &prev_transforms = _prev_anim_layer.get_blended_bone_transforms();
    const std::vector<glm::mat4> &new_transforms  = _anim_layer.get_blended_bone_transforms();

    _global_blended_bone_transforms.resize(new_transforms.size());

    for (size_t i = 0; i < new_transforms.size(); ++i) {
      glm::mat4 blended                  = Util::lerp(prev_transforms[i], new_transforms[i], t);
      _global_blended_bone_transforms[i] = blended;
    }
  } else {
    _global_blended_bone_transforms = _anim_layer.get_blended_bone_transforms();
  }
}

void AnimEntity::render() {
  _skinned_model->render();
}

void AnimEntity::submit_render_item() {
  RenderDataManager::submit_anim_entity(this);
}

void AnimEntity::play_animation(const std::string &name, float speed) {
  AnimPlaybackParams params;
  params.speed = speed;

  _prev_anim_layer = _anim_layer;
  // TODO: Blend config
  _blend_duration = 0.00f;
  _blend_time     = 0.0f;
  _blending       = false;

  _anim_layer.play_animation(name, params);
}

void AnimEntity::play_animation(const std::vector<std::string> &names, float speed) {
  AnimPlaybackParams params;
  params.speed = speed;

  _prev_anim_layer = _anim_layer;
  _blend_duration  = 0.0f;
  _blend_time      = 0.0f;
  _blending        = false;

  int rand = std::rand() % names.size();
  _anim_layer.play_animation(names[rand], params);
}

void AnimEntity::loop_animation(const std::string &name, float speed) {
  AnimPlaybackParams params;
  params.speed = speed;
  _anim_layer.loop_animation(name, params);
}

void AnimEntity::set_skinned_model(const std::string &name) {
  _skinned_model = AssetManager::get_skinned_model_by_name(name);
  _anim_layer.set_skinned_model(name);
}

void AnimEntity::set_name(const std::string &name) {
  _name = name;
}

void AnimEntity::translate(const glm::vec3 &delta) {
  _position += delta;
}

void AnimEntity::set_position(const glm::vec3 &pos) {
  _position = pos;
}

void AnimEntity::set_rotation(const glm::vec3 &rot) {
  _rotation = rot;
}

void AnimEntity::set_scale(const glm::vec3 &scale) {
  _scale = scale;
}

void AnimEntity::set_quat(const glm::quat &quat) {
  _quat = quat;
}

const std::string &AnimEntity::get_name() {
  return _name;
}

glm::mat4 AnimEntity::get_model_matrix() {
  glm::mat4 model = glm::mat4(1.0f);
  model           = glm::translate(model, _position);
  model           = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
  model           = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
  model           = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
  model *= glm::toMat4(_quat);
  model = glm::scale(model, _scale);
  return model;
}

bool AnimEntity::all_anim_states_complete() {
  for (AnimState &anim_state : _anim_layer.get_anim_states()) {
    if (!anim_state.is_complete()) {
      return false;
    }
  }

  return true;
}

bool AnimEntity::animation_by_name_is_complete(const std::string &name) {
  int anim_index = AssetManager::get_animation_index_by_name(name);

  for (AnimState &anim_state : _anim_layer.get_anim_states()) {
    if (anim_state.get_anim_index() == anim_index) {
      bool complete = anim_state.is_complete();
      return complete;
    }
  }

  return true;
}

std::vector<glm::mat4> &AnimEntity::get_global_blended_bone_transforms() {
  return _global_blended_bone_transforms;
}
