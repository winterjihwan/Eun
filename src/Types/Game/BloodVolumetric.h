#pragma once

#include "CreateInfo.h"
#include "Model.h"
#include "Types.h"
#include "Types/Texture/ExrTexture.h"
#include <glm/glm.hpp>

struct BloodVolumetric {
public:
  float     _current_time = 0.0f;
  Transform _transform;
  glm::vec3 _front;

  BloodVolumetric(const BloodVolumetricCreateInfo create_info);
  void update(float deltaTime);

  void submit_render_item();

  Model      *get_model();
  ExrTexture *get_exr_texture_pos();
  ExrTexture *get_exr_texture_norm();
  glm::mat4   get_model_transform();

private:
  Model      *_model;
  ExrTexture *_exr_texture_pos;
  ExrTexture *_exr_texture_norm;
};
