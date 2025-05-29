#include "BloodVolumetric.h"
#include "AssetManager/AssetManager.h"
#include "Renderer/RenderDataManager.h"

BloodVolumetric::BloodVolumetric(const BloodVolumetricCreateInfo create_info) {
  _model             = create_info.model;
  _exr_texture_index = create_info.exr_texture_index;
  _exr_texture_pos =
      AssetManager::get_exr_texture_by_name(std::format("blood_pos_{}", _exr_texture_index));
  _exr_texture_norm =
      AssetManager::get_exr_texture_by_name(std::format("blood_norm_{}", _exr_texture_index));

  _transform.position = create_info.position;
  _transform.rotation = create_info.rotation;
  _transform.scale    = glm::vec3(5.0f);
  _front              = create_info.front;
}

void BloodVolumetric::update(float deltaTime) {
  _current_time += deltaTime * 1.5f;
}

void BloodVolumetric::submit_render_item() {
  RenderDataManager::submit_blood_volumetric(this);
}

Model *BloodVolumetric::get_model() {
  return _model;
}

ExrTexture *BloodVolumetric::get_exr_texture_pos() {
  return _exr_texture_pos;
}

ExrTexture *BloodVolumetric::get_exr_texture_norm() {
  return _exr_texture_norm;
}

glm::mat4 BloodVolumetric::get_model_transform() {
  Transform blood_base;
  blood_base.position = _transform.position;
  blood_base.scale    = _transform.scale;

  Transform blood_offset;
  if (_exr_texture_index == 1) {
    blood_offset.position = glm::vec3(-0.08f, -0.23f, -0.16f);
  } else if (_exr_texture_index == 2) {
    blood_offset.position = glm::vec3(-0.05f, -0.41f, -0.19f);
  } else if (_exr_texture_index == 3) {
    blood_offset.position = glm::vec3(-0.08f, -0.38f, -0.15f);
  } else if (_exr_texture_index == 4) {
    blood_offset.position = glm::vec3(-0.23f, -0.5f, -0.22f);
  } else if (_exr_texture_index == 5) {
    blood_offset.position = glm::vec3(-0.17f, -0.23f, -0.18f);
  } else if (_exr_texture_index == 6) {
    blood_offset.position = glm::vec3(-0.05f, -0.25f, -0.13f);
  }

  return blood_base.to_mat4() * blood_offset.to_mat4();
}
