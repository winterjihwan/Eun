#include "Renderer/RenderDataManager.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _view;
extern glm::mat4                               _projection;

void decal_pass() {
  Shader &shader = _shaders["Decal"];
  shader.use();

  shader.setMat4("u_view", _view);
  shader.setMat4("u_projection", _projection);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);

  std::vector<Decal *> decals = RenderDataManager::get_decals();
  for (Decal *decal : decals) {
    glm::mat4 model = decal->get_model_transform();
    shader.setMat4("u_model", model);

    Mesh *mesh = decal->get_mesh();
    mesh->draw(shader);
  }

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glBindVertexArray(0);
}

} // namespace OpenGLRenderer
