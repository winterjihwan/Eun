#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Renderer/Shader.h"
#include "Util/Util.h"
#include <Jolt/Jolt.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <Jolt/Geometry/AABox.h>

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;

unsigned int _aabb_vao = 0;

void init_aabb() {
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
  };
  unsigned int indices[] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
  unsigned int vbo, ebo;
  glGenVertexArrays(1, &_aabb_vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(_aabb_vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

void render_aabb() {
  glBindVertexArray(_aabb_vao);
  glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void aabb_pass() {
  Shader &shader_aabb = get_shader("AABB");
  shader_aabb.use();
  shader_aabb.setMat4("u_projection", _projection);
  shader_aabb.setMat4("u_view", _view);

  std::vector<AABB *> aabbs = RenderDataManager::get_aabbs();
  for (AABB *aabb : aabbs) {
    glm::vec3 center = aabb->get_center();
    glm::vec3 extent = aabb->get_extent();

    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), extent * 2.0f);
    shader_aabb.setMat4("u_model", model);

    render_aabb();
  }
}

} // namespace OpenGLRenderer
