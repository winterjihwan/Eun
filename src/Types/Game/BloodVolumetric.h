#pragma once

#include "CreateInfo.h"
#include "Model.h"
#include "Types/Texture/ExrTexture.h"
#include <glm/glm.hpp>

struct BloodVolumetric {
public:
  float     _current_time = 0.0f;
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _front;

  BloodVolumetric() = default;

  void init(BloodVolumetricCreateInfo create_info);
  void update(float deltaTime);

  void submit_render_item();

  Model      *get_model();
  ExrTexture *get_exr_texture_pos();
  ExrTexture *get_exr_texture_norm();

private:
  Model      *_model;
  ExrTexture *_exr_texture_pos;
  ExrTexture *_exr_texture_norm;
};
