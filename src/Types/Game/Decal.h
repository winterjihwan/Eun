#pragma once

#include "CreateInfo.h"
#include "Enums.h"

struct Decal {
  Decal(const DecalCreateInfo &create_info);

  void submit_render_item();

  glm::mat4 get_model_transform();
  Mesh     *get_mesh();
  DecalType get_type();

private:
  Mesh     *_mesh;
  DecalType _type;
  glm::mat4 _model_transform;
};
