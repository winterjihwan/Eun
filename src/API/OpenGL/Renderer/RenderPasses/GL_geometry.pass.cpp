#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "AssetManager/AssetManager.h"
#include "Renderer/RenderDataManager.h"
#include <glm/gtx/euler_angles.hpp>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;

void geometry_pass() {
  Shader            &shader   = _shaders["Model"];
  OpenGLFrameBuffer &g_buffer = _frame_buffers["G_Buffer"];

  g_buffer.bind();
  g_buffer.draw_buffers({"Position", "Normal", "AlbedoSpec"});
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // HACK: First g_buffer bind in the frame

  shader.use();

  // Scene
  shader.setMat4("u_projection", _projection);
  shader.setMat4("u_view", _view);

  // TODO: Move below to World.cpp
  // TODO: Use custom Map
  // Map
  glm::mat4 map = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
  map           = glm::rotate(map, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  shader.setMat4("u_model", map);
  Mesh *plane = AssetManager::get_mesh_by_name("Plane");
  plane->draw(shader);

  std::vector<Entity *> entities = RenderDataManager::get_entities();
  // TODO: Render entities

  g_buffer.unbind();
}
} // namespace OpenGLRenderer
