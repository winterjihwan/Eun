#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Renderer/RenderDataManager.h"
#include "Shader.h"
#include "Types.h"
#include "UI/Mesh2D.h"
#include "UI/UIBackend.h"
#include <string>
#include <unordered_map>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;

void ui_pass() {
  Shader shader = _shaders["UI"];

  glDisable(GL_CULL_FACE);

  std::vector<UIRenderItem *> ui_render_items = RenderDataManager::get_ui_render_items();
  Mesh2D                     &ui_mesh         = UIBackend::get_ui_mesh();

  shader.use();

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindVertexArray(ui_mesh.get_vao());

  for (UIRenderItem *ui_render_item : ui_render_items) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ui_render_item->texture->get_handle());
    glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                      ui_render_item->index_count,
                                      GL_UNSIGNED_INT,
                                      (void *)(sizeof(unsigned int) * ui_render_item->base_index),
                                      1,
                                      ui_render_item->base_vertex);
  }

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

} // namespace OpenGLRenderer
