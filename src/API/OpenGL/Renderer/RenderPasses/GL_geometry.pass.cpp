#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "AssetManager/AssetManager.h"
#include "Backend/Backend.h"
#include "Renderer/RenderDataManager.h"
#include <glm/gtx/euler_angles.hpp>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;
extern glm::mat4                                          _light_space;

void geometry_pass() {
  Shader            &shader_model  = _shaders["Model"];
  Shader            &shader_shadow = _shaders["Shadow"];
  OpenGLFrameBuffer &g_buffer      = _frame_buffers["G_Buffer"];
  OpenGLFrameBuffer &shadow_buffer = _frame_buffers["Shadow"];

  Viewport viewport = Backend::get_viewport();

  /* G_Buffer */
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

  /* Shadow */
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  shadow_buffer.clear_bind();

  shader_shadow.use();
  shader_shadow.setMat4("u_light_space", _light_space);
  shader_shadow.setMat4("u_model", map);

  plane->draw(shader_shadow);

  shadow_buffer.unbind();
  glViewport(0, 0, *viewport.width, *viewport.height);
}
} // namespace OpenGLRenderer
