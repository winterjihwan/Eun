#pragma once

#include "CreateInfo.h"

struct Decal {
  Decal(const DecalCreateInfo &create_info);

  void submit_render_item();

  glm::mat4 get_model_transform();
  Mesh     *get_mesh();

private:
  Mesh     *_mesh;
  glm::mat4 _model_transform;
};
