#include "Decal.h"
#include "Enums.h"
#include "Renderer/RenderDataManager.h"

Decal::Decal(const DecalCreateInfo &create_info) {
  const glm::vec3 &hit_position = create_info.hit_position;
  const glm::vec3 &hit_normal   = create_info.hit_normal;
  Mesh            *mesh         = create_info.mesh;

  glm::vec3 pos = hit_position + 0.01f * hit_normal;
  glm::vec3 up  = glm::vec3(0, 1, 0);

  glm::vec3 tangent   = glm::normalize(glm::cross(up, hit_normal));
  glm::vec3 bitangent = glm::normalize(glm::cross(hit_normal, tangent));
  glm::mat4 rotation  = glm::mat4(glm::vec4(tangent, 0.0f),
                                 glm::vec4(bitangent, 0.0f),
                                 glm::vec4(hit_normal, 0.0f),
                                 glm::vec4(0, 0, 0, 1));
  glm::mat4 scale     = glm::mat4(1.0f);

  if (create_info.type == DecalType::PLASTER) {
    scale = glm::scale(scale, glm::vec3(0.2f));
  }

  _model_transform = glm::translate(glm::mat4(1.0f), pos) * rotation * scale;
  _mesh            = mesh;
  _type            = create_info.type;
}

void Decal::submit_render_item() {
  RenderDataManager::submit_decal(this);
}

glm::mat4 Decal::get_model_transform() {
  return _model_transform;
}

Mesh *Decal::get_mesh() {
  return _mesh;
}

DecalType Decal::get_type() {
  return _type;
}
