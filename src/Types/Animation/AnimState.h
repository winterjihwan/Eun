#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct AnimPlaybackParams {
  float speed                      = 1.0f;
  float blend_weight               = 1.0f;
  bool  remove_existing_animations = true;
  bool  remove_on_complete         = false;
};

struct AnimState {
public:
  void                    update(int skinned_model_index, float delta_time);
  void                    play_animation(const std::string &name, const AnimPlaybackParams &params);
  void                    loop_animation(const std::string &name, const AnimPlaybackParams &params);
  void                    pause_animation();
  int                     get_anim_index();
  bool                    is_complete();
  bool                    awaiting_removal();
  std::vector<glm::mat4> &get_bone_transforms();

private:
  AnimPlaybackParams     _playback_params;
  float                  _blend_factor = 0;
  bool                   _loop         = false;
  bool                   _paused       = false;
  bool                   _complete     = false;
  int                    _anim_index   = -1;
  float                  _current_time = 0;
  std::vector<glm::mat4> _bone_transforms;
};
