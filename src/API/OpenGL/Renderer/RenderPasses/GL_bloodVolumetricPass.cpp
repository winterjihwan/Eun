#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Game/BloodVolumetric.h"

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;

void blood_volumetric_pass() {
  Shader &shader = get_shader("BloodVolumetric");

  std::vector<BloodVolumetric *> blood_volumetrics = RenderDataManager::get_blood_volumetrics();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glDisable(GL_BLEND);

  shader.use();
  shader.setMat4("u_view", _view);
  shader.setMat4("u_projection", _projection);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  for (BloodVolumetric *blood_volumetric : blood_volumetrics) {
    GLuint    texPos          = blood_volumetric->get_exr_texture_pos()->get_handle();
    GLuint    texNorm         = blood_volumetric->get_exr_texture_norm()->get_handle();
    glm::mat4 model_transform = blood_volumetric->get_model_transform();

    shader.setMat4("u_model", model_transform);
    shader.setFloat("u_time", blood_volumetric->_current_time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPos);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texNorm);

    blood_volumetric->get_model()->render();
  }

  glDisable(GL_CULL_FACE);
  glBindVertexArray(0);
}
} // namespace OpenGLRenderer
