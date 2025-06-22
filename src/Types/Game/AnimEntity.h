#pragma once

#include "Types/Animation/AnimLayer.h"
#include "Types/Renderer/SkinnedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

struct AnimEntity {
  AnimEntity() = default;

  void                    update(float delta_time);
  void                    render();
  void                    submit_render_item();
  void                    play_animation(const std::string &name, float speed = 1.0f);
  void                    play_animation(const std::vector<std::string> &names, float speed = 1.0f);
  void                    loop_animation(const std::string &name, float speed = 1.0f);
  void                    set_skinned_model(const std::string &name);
  void                    set_name(const std::string &name);
  bool                    all_anim_states_complete();
  std::vector<glm::mat4> &get_global_blended_bone_transforms();
  bool                    animation_by_name_is_complete(const std::string &name);
  void                    translate(const glm::vec3 &delta);
  void                    set_position(const glm::vec3 &pos);
  void                    set_rotation(const glm::vec3 &rot);
  void                    set_scale(const glm::vec3 &scale);
  void                    set_quat(const glm::quat &quat);
  const std::string      &get_name();
  glm::mat4               get_model_matrix();

private:
  SkinnedModel          *_skinned_model;
  std::string            _name = "UNSET";
  AnimLayer              _anim_layer;
  AnimLayer              _prev_anim_layer;
  float                  _blend_duration = 0.0f;
  bool                   _blending       = false;
  float                  _blend_time     = 0.0f;
  std::vector<glm::mat4> _global_blended_bone_transforms;
  glm::vec3              _position = glm::vec3(0.0f);
  glm::vec3              _rotation = glm::vec3(0.0f);
  glm::vec3              _scale    = glm::vec3(1.0f);
  glm::quat              _quat     = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};
