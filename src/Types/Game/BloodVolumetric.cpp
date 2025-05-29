#include "BloodVolumetric.h"
#include "Renderer/RenderDataManager.h"

BloodVolumetric::BloodVolumetric(const BloodVolumetricCreateInfo create_info) {
  _model            = create_info.model;
  _exr_texture_pos  = create_info.exr_texture_pos;
  _exr_texture_norm = create_info.exr_texture_norm;

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

  // TODO: Apply different offsets for all blood volumetrics
  Transform blood_offset;
  blood_offset.position = glm::vec3(-0.05f, -0.25f, -0.13f);

  return blood_base.to_mat4() * blood_offset.to_mat4();
}
