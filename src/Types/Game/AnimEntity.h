#pragma once

#include "Model.h"
#include "Types/Animation/Animator.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

struct AnimEntity {
  AnimEntity() = default;

  void init(const std::string &name, Model *model, Animator *animator, glm::mat4 &model_transform);
  void update(float delta_time);
  void submit_render_item();

  void add_anim_entity(AnimEntity &&anim_entity);

  void set_model(Model *model);
  void set_animator(Animator *animator);
  void set_model_transform(glm::mat4 model_transform);
  void set_collider(JPH::BodyID *collider);

  const std::string &get_name();
  Model             *get_model();
  Animator          *get_animator();
  glm::mat4         &get_model_transform();

private:
  std::string  _name;
  Model       *_model;
  Animator    *_current_animator = 0;
  glm::mat4    _model_transform;
  JPH::BodyID *_collider = 0;

  // TODO: Render Item
};
