#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Renderer/RenderDataManager.h"

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;

void geometry_pass() {
  Shader            &shader_model = get_shader("Model");
  OpenGLFrameBuffer &g_buffer     = get_frame_buffer("G_Buffer");

  g_buffer.bind();
  g_buffer.draw_buffers({"Position", "Normal", "AlbedoSpec"});

  shader_model.use();

  // Scene
  shader_model.setMat4("u_projection", _projection);
  shader_model.setMat4("u_view", _view);

  std::vector<Entity *> entities = RenderDataManager::get_entities();
  for (Entity *entity : entities) {
    glm::mat4 &model = entity->get_transform();
    shader_model.setMat4("u_model", model);
    std::variant<Model *, Mesh *> renderable = entity->get_renderable();

    if (std::holds_alternative<Model *>(renderable)) {
    } else if (std::holds_alternative<Mesh *>(renderable)) {
      std::get<Mesh *>(renderable)->draw(shader_model);
    }
  }

  g_buffer.unbind();
}
} // namespace OpenGLRenderer
