#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Defines.h"
#include "Types/Renderer/Shader.h"
#include "UI/Mesh2D.h"
#include "UI/UIBackend.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace OpenGLRenderer {

void ui_pass() {
  Shader shader = get_shader("UI");

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  Mesh2D &mesh = UIBackend::get_ui_mesh();
  glBindVertexArray(mesh.get_vao());

  shader.use();
  glm::mat4 ortho = glm::ortho(
      0.0f, static_cast<float>(VIEWPORT_WIDTH), 0.0f, static_cast<float>(VIEWPORT_HEIGHT));
  shader.setMat4("u_Projection", ortho);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (UIRenderItem &render_item : UIBackend::get_render_items()) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render_item.texture_id);
    glDrawArrays(GL_TRIANGLES, render_item.base_vertex, render_item.vertex_count);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

} // namespace OpenGLRenderer
