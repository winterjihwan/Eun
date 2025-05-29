#include "Renderer/RenderDataManager.h"
#include "Types/Game/BloodVolumetric.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _view;
extern glm::mat4                               _projection;

void blood_volumetric_pass() {
  Shader                        &shader            = _shaders["BloodVolumetric"];
  std::vector<BloodVolumetric *> blood_volumetrics = RenderDataManager::get_blood_volumetrics();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  glCullFace(GL_BACK);

  shader.use();
  shader.setMat4("u_View", _view);
  shader.setMat4("u_Projection", _projection);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  for (BloodVolumetric *blood_volumetric : blood_volumetrics) {
    Mesh     &mesh            = blood_volumetric->get_model()->meshes[0];
    GLuint    texPos          = blood_volumetric->get_exr_texture_pos()->get_handle();
    GLuint    texNorm         = blood_volumetric->get_exr_texture_norm()->get_handle();
    glm::mat4 model_transform = blood_volumetric->get_model_transform();

    shader.setMat4("u_Model", model_transform);
    shader.setFloat("u_Time", blood_volumetric->_current_time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPos);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texNorm);

    mesh.draw(shader);
  }

  glDepthMask(GL_TRUE);
  glDisable(GL_CULL_FACE);
  glBindVertexArray(0);
}
} // namespace OpenGLRenderer
