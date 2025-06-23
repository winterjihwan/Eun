#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "Types/Renderer/Shader.h"
#include <unordered_map>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;

const unsigned int     NR_LIGHTS = 32;
std::vector<glm::vec3> light_positions;
std::vector<glm::vec3> light_colors;

void init_light() {
  srand(13);
  for (unsigned int i = 0; i < NR_LIGHTS; i++) {
    float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
    float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
    float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
    light_positions.push_back(glm::vec3(xPos, yPos, zPos));

    float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
    float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
    float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
    light_colors.push_back(glm::vec3(rColor, gColor, bColor));
  }

  Shader &shader = _shaders["Light"];
  shader.use();
  shader.setInt("gPosition", 0);
  shader.setInt("gNormal", 1);
  shader.setInt("gAlbedoSpec", 2);
}

void light_pass() {
  Shader            &shader   = _shaders["Light"];
  OpenGLFrameBuffer &g_buffer = _frame_buffers["G_Buffer"];

  Camera *camera = Game::get_camera();
  Player *player = Game::get_player();

  shader.use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g_buffer.get_color_attachment_handle_by_name("Position"));
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, g_buffer.get_color_attachment_handle_by_name("Normal"));
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, g_buffer.get_color_attachment_handle_by_name("AlbedoSpec"));

  // Base
  shader.setVec3("viewPos", camera->get_pos());
  shader.setFloat("shininess", 32.0f);

  // Directional Light
  shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

  // Point Light
  for (unsigned int i = 0; i < light_positions.size(); i++) {
    shader.setVec3("pointLights[" + std::to_string(i) + "].position", light_positions[i]);
    shader.setVec3("pointLights[" + std::to_string(i) + "].color", light_colors[i]);
    shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.7f);
    shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 1.8f);
  }

  // FlashLight
  shader.setBool("flashLight.is_on", player->get_flashlight_on());
  shader.setVec3("flashLight.position", player->get_pos());
  shader.setVec3("flashLight.direction", camera->get_front());
  shader.setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
  shader.setVec3("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader.setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
  shader.setFloat("flashLight.constant", 1.0f);
  shader.setFloat("flashLight.linear", 0.09f);
  shader.setFloat("flashLight.quadratic", 0.032f);
  shader.setFloat("flashLight.cutOff", glm::cos(glm::radians(12.5f)));
  shader.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  Mesh *g_buffer_quad = AssetManager::get_mesh_by_name("G_Buffer");
  g_buffer_quad->draw(shader);
}

} // namespace OpenGLRenderer
