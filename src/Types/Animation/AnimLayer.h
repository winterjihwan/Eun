#pragma once

#include "AnimState.h"
#include <string>
#include <vector>

struct AnimLayer {
public:
  void                    update(float delta_time);
  void                    play_animation(const std::string &name, const AnimPlaybackParams &params);
  void                    loop_animation(const std::string &name, const AnimPlaybackParams &params);
  void                    set_skinned_model(const std::string &name);
  void                    clear_all_anim_states();
  std::vector<glm::mat4> &get_blended_bone_transforms();
  std::vector<AnimState> &get_anim_states();

private:
  std::string            _name;
  int                    _joint_count;
  std::vector<AnimState> _anim_states;
  int                    _skinned_model_index = -1;
  std::vector<glm::mat4> _blended_bone_transforms;
};
