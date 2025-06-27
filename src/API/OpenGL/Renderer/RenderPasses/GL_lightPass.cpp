#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "AssetManager/AssetManager.h"
#include "Backend/Backend.h"
#include "Core/Game.h"
#include "Types/Renderer/Mesh.h"
#include "Types/Renderer/Shader.h"

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;
extern glm::mat4 _light_space;

void init_light() {
  Shader &shader = get_shader("Light");
  shader.use();
  shader.setInt("gPosition", 0);
  shader.setInt("gNormal", 1);
  shader.setInt("gAlbedoSpec", 2);
  shader.setInt("gShadow", 3);
}

void light_pass() {
  Viewport           viewport      = Backend::get_viewport();
  Shader            &shader_light  = get_shader("Light");
  OpenGLFrameBuffer &g_buffer      = get_frame_buffer("G_Buffer");
  OpenGLFrameBuffer &shadow_buffer = get_frame_buffer("Shadow");

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
  shader_light.setVec3("u_viewPos", player->get_pos());
  shader_light.setVec3("u_lightPos", -2.0f, 4.0f, -1.0f);
  shader_light.setFloat("u_shininess", 32.0f);
  shader_light.setMat4("u_light_space", _light_space);

  // Directional Light
  shader_light.setVec3("u_dirLight.direction", 0.2f, -1.0f, 0.3f);
  shader_light.setVec3("u_dirLight.ambient", 0.2f, 0.2f, 0.2f);
  shader_light.setVec3("u_dirLight.diffuse", 0.8f, 0.8f, 0.8f);
  shader_light.setVec3("u_dirLight.specular", 0.5f, 0.5f, 0.5f);

  // FlashLight
  shader_light.setBool("u_flashLight.is_on", player->get_flashlight_on());
  shader_light.setVec3("u_flashLight.position", player->get_pos());
  shader_light.setVec3("u_flashLight.direction", camera->get_front());
  shader_light.setVec3("u_flashLight.ambient", 0.0f, 0.0f, 0.0f);
  shader_light.setVec3("u_flashLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader_light.setVec3("u_flashLight.specular", 1.0f, 1.0f, 1.0f);
  shader_light.setFloat("u_flashLight.constant", 1.0f);
  shader_light.setFloat("u_flashLight.linear", 0.09f);
  shader_light.setFloat("u_flashLight.quadratic", 0.032f);
  shader_light.setFloat("u_flashLight.cutOff", glm::cos(glm::radians(12.5f)));
  shader_light.setFloat("u_flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  Mesh *g_buffer_quad = AssetManager::get_mesh_by_name("G_Buffer");
  g_buffer_quad->draw(shader_light);
}

} // namespace OpenGLRenderer
