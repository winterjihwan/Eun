#pragma once

#include "Model.h"
#include "Types/Animation/Animator.h"

struct AnimEntity {
  AnimEntity() = default;

  void init(Model *model, Animator *animator, glm::mat4 &model_transform, const std::string &name);
  void update(float delta_time);
  void submit_render_item();

  void add_anim_entity(AnimEntity &&anim_entity);

  void set_model(Model *model);
  void set_animator(Animator *animator);
  void set_model_transform(glm::mat4 model_transform);

  const std::string &get_name();
  Model             *get_model();
  Animator          *get_animator();
  glm::mat4         &get_model_transform();

private:
  std::string _name;
  Model      *_model;
  Animator   *_current_animator = 0;
  glm::mat4   _model_transform;

  // TODO: Render Item
};
