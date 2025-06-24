#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "AssetManager/AssetManager.h"
#include "Backend/Backend.h"
#include "Core/Game.h"
#include "Types/Renderer/Mesh.h"
#include "Types/Renderer/Shader.h"
#include <unordered_map>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;
extern glm::mat4                                          _light_space;

void init_light() {
  Shader &shader = _shaders["Light"];
  shader.use();
  shader.setInt("gPosition", 0);
  shader.setInt("gNormal", 1);
  shader.setInt("gAlbedoSpec", 2);
  shader.setInt("gShadow", 3);
}

void light_pass() {
  Viewport           viewport      = Backend::get_viewport();
  Shader            &shader_light  = _shaders["Light"];
  OpenGLFrameBuffer &g_buffer      = _frame_buffers["G_Buffer"];
  OpenGLFrameBuffer &shadow_buffer = _frame_buffers["Shadow"];

  Camera *camera = Game::get_camera();
  Player *player = Game::get_player();

  /* Render */
  glViewport(0, 0, *viewport.width, *viewport.height);
  shader_light.use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g_buffer.get_color_attachment_handle_by_name("Position"));
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, g_buffer.get_color_attachment_handle_by_name("Normal"));
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, g_buffer.get_color_attachment_handle_by_name("AlbedoSpec"));
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, shadow_buffer.get_depth_attachment_handle());

  // Base
  shader_light.setVec3("viewPos", player->get_pos());
  shader_light.setVec3("lightPos", -2.0f, 4.0f, -1.0f);
  shader_light.setFloat("shininess", 32.0f);
  shader_light.setMat4("lightSpaceMatrix", _light_space);

  // Directional Light
  shader_light.setVec3("dirLight.direction", 0.2f, -1.0f, 0.3f);
  shader_light.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  shader_light.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
  shader_light.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

  // FlashLight
  shader_light.setBool("flashLight.is_on", player->get_flashlight_on());
  shader_light.setVec3("flashLight.position", player->get_pos());
  shader_light.setVec3("flashLight.direction", camera->get_front());
  shader_light.setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
  shader_light.setVec3("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader_light.setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
  shader_light.setFloat("flashLight.constant", 1.0f);
  shader_light.setFloat("flashLight.linear", 0.09f);
  shader_light.setFloat("flashLight.quadratic", 0.032f);
  shader_light.setFloat("flashLight.cutOff", glm::cos(glm::radians(12.5f)));
  shader_light.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  Mesh *g_buffer_quad = AssetManager::get_mesh_by_name("G_Buffer");
  g_buffer_quad->draw(shader_light);

  // Shader &shader_texture = _shaders["Texture"];
  // shader_texture.use();
  //
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, shadow_buffer.get_depth_attachment_handle());
  // shader_texture.setMat4("u_projection", _projection);
  // shader_texture.setMat4("u_view", _view);
  // glm::mat4 map = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
  // map           = glm::rotate(map, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  // shader_texture.setMat4("u_model", map);
  // shader_texture.setInt("sampler", 0);
  //
  // Mesh *quad = AssetManager::get_mesh_by_name("G_Buffer");
  // quad->draw(shader_texture);
}

} // namespace OpenGLRenderer
