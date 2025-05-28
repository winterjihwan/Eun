#include "BloodVolumetric.h"
#include "Renderer/RenderDataManager.h"

void BloodVolumetric::init(BloodVolumetricCreateInfo create_info) {
  _model            = create_info.model;
  _exr_texture_pos  = create_info.exr_texture_pos;
  _exr_texture_norm = create_info.exr_texture_norm;
  _position         = create_info.position;
  _rotation         = create_info.rotation;
  _front            = create_info.front;
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
