#include "API/OpenGL/Renderer/GL_cubemapView.h"
#include "Core/Game.h"
#include "Types/Renderer/Shader.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _projection;
extern glm::mat4                               _view;
unsigned int                                   _sky_vao;
OpenGLCubemapView                              _cubemap_view;

void init_skybox() {
  float skybox_vertices[] = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                             1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                             -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                             -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                             1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                             1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                             -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                             1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                             -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                             1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                             -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                             1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  // Skybox VAO
  unsigned int skyVBO;
  glGenVertexArrays(1, &_sky_vao);
  glGenBuffers(1, &skyVBO);
  glBindVertexArray(_sky_vao);
  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  _cubemap_view.init();
  Shader &sky_shader = _shaders["Sky"];

  sky_shader.use();
  sky_shader.setInt("skybox", 0);
}

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
  glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap_view.get_handle());
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}
} // namespace OpenGLRenderer
