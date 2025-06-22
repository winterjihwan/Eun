#include "API/OpenGL/Renderer/GL_cubemapView.h"
#include "Core/Game.h"
#include "Types/Renderer/Shader.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern glm::mat4                                          _projection;
extern glm::mat4                                          _view;
extern unsigned int                                       _sky_vao;
extern std::unordered_map<std::string, OpenGLCubemapView> _cubemap_views;

void skybox_pass() {
  Shader &shader = _shaders["Sky"];

  // Cubemap
  glDepthFunc(GL_LEQUAL);
  shader.use();
  glm::mat4 view = glm::lookAt(Game::get_player()->get_pos(),
                               Game::get_player()->get_pos() + Game::get_camera()->get_front(),
                               Game::get_camera()->get_up());
  view           = glm::mat4(glm::mat3(view));
  shader.setMat4("u_view", view);
  shader.setMat4("u_projection", _projection);

  glBindVertexArray(_sky_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap_views["Sky"].get_handle());
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}
} // namespace OpenGLRenderer
