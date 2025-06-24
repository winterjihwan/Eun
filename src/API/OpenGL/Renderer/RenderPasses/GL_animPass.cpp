#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Game/AnimEntity.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;

void anim_pass() {
  Shader             shader_anim = _shaders["Anim"];
  OpenGLFrameBuffer &g_buffer    = _frame_buffers["G_Buffer"];

  g_buffer.bind();
  g_buffer.draw_buffers({"Position", "Normal", "AlbedoSpec"});

  shader_anim.use();
  shader_anim.setMat4("u_projection", _projection);
  shader_anim.setMat4("u_view", _view);

  // Anim Entities
  std::vector<AnimEntity *> anim_entities = RenderDataManager::get_anim_entities();
  for (AnimEntity *anim_entity : anim_entities) {
    std::vector<glm::mat4> transforms = anim_entity->get_global_blended_bone_transforms();

    // Bones
    for (uint i = 0; i < transforms.size(); i++) {
      if (i >= MAX_BONES) {
        std::cout << "INCREASE MAX_BONES" << std::endl;
        exit(1);
      }
      std::string name = "u_bones[" + std::to_string(i) + "]";
      shader_anim.setMat4(name.c_str(), transforms[i]);
    }

    glm::mat4 model = anim_entity->get_model_matrix();
    shader_anim.setMat4("u_model", model);
    anim_entity->render();
  }

  g_buffer.unbind();
}
} // namespace OpenGLRenderer
