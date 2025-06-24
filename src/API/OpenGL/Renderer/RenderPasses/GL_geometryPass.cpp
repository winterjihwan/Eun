#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "AssetManager/AssetManager.h"
#include "Renderer/RenderDataManager.h"
#include <glm/gtx/euler_angles.hpp>

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

  // TODO: Move below to World.cpp
  // TODO: Use custom Map
  // Map
  glm::mat4 map = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
  map           = glm::rotate(map, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  shader_model.setMat4("u_model", map);
  Mesh *plane = AssetManager::get_mesh_by_name("Plane");
  plane->draw(shader_model);

  std::vector<Entity *> entities = RenderDataManager::get_entities();
  // TODO: Render entities

  g_buffer.unbind();
}
} // namespace OpenGLRenderer
