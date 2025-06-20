#include "Renderer/RenderDataManager.h"
#include "Types/Game/AnimEntity.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _view;
extern glm::mat4                               _projection;

void anim_pass() {
  Shader shader = _shaders["Anim"];

  shader.use();
  shader.setMat4("projection", _projection);
  shader.setMat4("view", _view);

  // Anim Entities
  std::vector<AnimEntity *> anim_entities = RenderDataManager::get_anim_entities();
  for (AnimEntity *anim_entity : anim_entities) {
    std::vector<glm::mat4> transforms = anim_entity->get_global_blended_bone_transforms();

    for (uint i = 0; i < transforms.size(); i++) {
      if (i >= MAX_BONES) {
        std::cout << "INCREASE MAX_BONES" << std::endl;
        exit(1);
      }
      std::string name = "bones[" + std::to_string(i) + "]";
      shader.setMat4(name.c_str(), transforms[i]);
    }

    shader.setMat4("model", anim_entity->get_model_matrix());
    anim_entity->render();
  }
}
} // namespace OpenGLRenderer
