#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "Enums.h"
#include "Renderer/RenderDataManager.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;

void decal_pass() {
  Shader            &shader             = _shaders["Decal"];
  Shader            &shader_decal_blood = _shaders["DecalBlood"]; // TODO: Neater code
  OpenGLFrameBuffer &g_buffer           = _frame_buffers["G_Buffer"];

  g_buffer.bind();
  g_buffer.draw_buffers({"Position", "Normal", "AlbedoSpec"});

  shader.use();
  shader.setMat4("u_view", _view);
  shader.setMat4("u_projection", _projection);

  shader_decal_blood.use();
  shader_decal_blood.setMat4("u_view", _view);
  shader_decal_blood.setMat4("u_projection", _projection);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);

  std::vector<Decal *> decals = RenderDataManager::get_decals();

  for (size_t i = 0; i < decals.size(); ++i) {
    Decal *decal = decals[i];

    if (decal->get_type() == DecalType::PLASTER) {
      shader.use();
    } else if (decal->get_type() == DecalType::BLOOD) {
      shader_decal_blood.use();
    }

    glm::mat4 model = decal->get_model_transform();
    shader.setMat4("u_model", model);

    Mesh *mesh = decal->get_mesh();
    mesh->draw(shader);
  }

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glBindVertexArray(0);

  g_buffer.unbind();
}

} // namespace OpenGLRenderer
