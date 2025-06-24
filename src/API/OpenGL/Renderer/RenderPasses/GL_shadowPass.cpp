#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "AssetManager/AssetManager.h"
#include "Backend/Backend.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Game/AnimEntity.h"
#include <glm/gtx/euler_angles.hpp>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader>            _shaders;
extern std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
extern glm::mat4                                          _view;
extern glm::mat4                                          _projection;
extern glm::mat4                                          _light_space;

void shadow_pass() {
  Shader            &shader_shadow      = _shaders["Shadow"];
  Shader            &shader_shadow_anim = _shaders["Shadow_Anim"];
  OpenGLFrameBuffer &shadow_buffer      = _frame_buffers["Shadow"];

  Viewport viewport = Backend::get_viewport();

  /* Shadow */
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  shadow_buffer.clear_bind();

  shader_shadow.use();
  shader_shadow.setMat4("u_light_space", _light_space);

  // Geometry
  glm::mat4 map = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
  map           = glm::rotate(map, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  shader_shadow.setMat4("u_model", map);
  Mesh *plane = AssetManager::get_mesh_by_name("Plane");
  plane->draw(shader_shadow);

  // Anim
  shader_shadow_anim.use();
  shader_shadow_anim.setMat4("u_light_space", _light_space);

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
      shader_shadow_anim.setMat4(name.c_str(), transforms[i]);
    }

    glm::mat4 model = anim_entity->get_model_matrix();
    shader_shadow_anim.setMat4("u_model", model);
    anim_entity->render();
  }

  shadow_buffer.unbind();
  glViewport(0, 0, *viewport.width, *viewport.height);
}
} // namespace OpenGLRenderer
