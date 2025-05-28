#include "Renderer/RenderDataManager.h"
#include "Types/Game/BloodVolumetric.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _view;
extern glm::mat4                               _projection;

void blood_volumetric_pass() {
  Shader                        &shader            = _shaders["BloodVolumetric"];
  std::vector<BloodVolumetric *> blood_volumetrics = RenderDataManager::get_blood_volumetrics();

  printf("blood_volumetric_pass(): count = %zu\n", blood_volumetrics.size());

  for (BloodVolumetric *blood_volumetric : blood_volumetrics) {
    printf("  [BloodVolumetric] time = %.3f\n", blood_volumetric->_current_time);

    GLuint texPos  = blood_volumetric->get_exr_texture_pos()->get_handle();
    GLuint texNorm = blood_volumetric->get_exr_texture_norm()->get_handle();
    printf("    TexPos = %u, TexNorm = %u\n", texPos, texNorm);

    for (Mesh &mesh : blood_volumetric->get_model()->meshes) {
      printf("    Mesh VAO = %u, index count = %zu\n", mesh.VAO, mesh.indices.size());

      shader.use();
      shader.setMat4("u_view", _view);
      shader.setMat4("u_projection", _projection);
      shader.setMat4("u_model", glm::mat4(1.0f));
      shader.setFloat("u_Time", blood_volumetric->_current_time);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texPos);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texNorm);

      mesh.draw(shader);

      // glBindVertexArray(mesh.VAO);
      // glDrawElements(
      //     GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
    }
  }
}
} // namespace OpenGLRenderer
