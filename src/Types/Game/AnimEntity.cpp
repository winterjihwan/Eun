#include "AnimEntity.h"
#include "Renderer/RenderDataManager.h"

void AnimEntity::init(Model             *model,
                      Animator          *animator,
                      glm::mat4         &model_transform,
                      const std::string &name) {
  _model            = model;
  _current_animator = animator;
  _model_transform  = model_transform;
  _name             = name;
}

void AnimEntity::update(float delta_time) {
  if (_current_animator != 0) {
    _current_animator->UpdateAnimation(delta_time);
  }
}

void AnimEntity::submit_render_item() {
  // TODO: Actually submit render item
  RenderDataManager::submit_anim_entity(this);
}

void AnimEntity::set_animator(Animator *animator) {
  _current_animator = animator;
}

const std::string &AnimEntity::get_name() {
  return _name;
}

Model *AnimEntity::get_model() {
  return _model;
}

Animator *AnimEntity::get_animator() {
  return _current_animator;
}

glm::mat4 &AnimEntity::get_model_transform() {
  return _model_transform;
}
