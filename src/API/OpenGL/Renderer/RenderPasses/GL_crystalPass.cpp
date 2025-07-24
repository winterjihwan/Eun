#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Core/Game.h"
#include "Renderer/RenderDataManager.h"

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;

void crystal_pass() {
  Shader &shader_crystal = get_shader("Crystal");

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);

  shader_crystal.use();
  shader_crystal.setMat4("u_projection", _projection);
  shader_crystal.setMat4("u_view", _view);
  shader_crystal.setVec3("u_camera_pos", Game::get_camera()->get_position());

  std::vector<Entity *> crystal_entities = RenderDataManager::get_crystal_entities();
  for (Entity *entity : crystal_entities) {
    glm::mat4 &model = entity->get_transform();
    shader_crystal.setMat4("u_model", model);

    std::variant<Model *, Mesh *> renderable = entity->get_renderable();
    if (std::holds_alternative<Model *>(renderable)) {
      std::get<Model *>(renderable)->render();
    } else if (std::holds_alternative<Mesh *>(renderable)) {
      std::get<Mesh *>(renderable)->draw(shader_crystal);
    }
  }

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}
} // namespace OpenGLRenderer
