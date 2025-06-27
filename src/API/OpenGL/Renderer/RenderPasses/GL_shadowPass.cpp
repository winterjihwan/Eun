#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Backend/Backend.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Game/AnimEntity.h"

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;
extern glm::mat4 _light_space;

void shadow_pass() {
  Shader            &shader_shadow      = get_shader("Shadow");
  Shader            &shader_shadow_anim = get_shader("Shadow_Anim");
  OpenGLFrameBuffer &shadow_buffer      = get_frame_buffer("Shadow");

  Viewport viewport = Backend::get_viewport();

  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  shadow_buffer.clear_bind();

  // Geometry
  shader_shadow.use();
  shader_shadow.setMat4("u_light_space", _light_space);

  std::vector<Entity *> entities = RenderDataManager::get_entities();
  for (Entity *entity : entities) {
    glm::mat4 &model = entity->get_transform();
    shader_shadow.setMat4("u_model", model);
    std::variant<Model *, Mesh *> renderable = entity->get_renderable();

    if (std::holds_alternative<Model *>(renderable)) {
      std::get<Model *>(renderable)->render();
    } else if (std::holds_alternative<Mesh *>(renderable)) {
      std::get<Mesh *>(renderable)->draw(shader_shadow);
    }
  }

  // Anim
  shader_shadow_anim.use();
  shader_shadow_anim.setMat4("u_light_space", _light_space);

  std::vector<AnimEntity *> anim_entities = RenderDataManager::get_anim_entities();
  for (AnimEntity *anim_entity : anim_entities) {
    std::vector<glm::mat4> transforms = anim_entity->get_global_blended_bone_transforms();

    for (uint i = 0; i < transforms.size(); i++) {
      if (i >= MAX_BONES) {
        std::cerr << "INCREASE MAX_BONES" << std::endl;
        assert(0);
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
