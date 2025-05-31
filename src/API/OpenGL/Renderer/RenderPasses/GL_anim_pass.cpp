#include "Renderer/RenderDataManager.h"
#include "Types/Game/AnimEntity.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _view;
extern glm::mat4                               _projection;

void anim_pass() {
  _shaders["Anim"].use();
  _shaders["Anim"].setMat4("projection", _projection);
  _shaders["Anim"].setMat4("view", _view);

  // Anim Entities
  std::vector<AnimEntity *> anim_entities = RenderDataManager::get_anim_entities();
  for (AnimEntity *anim_entity : anim_entities) {
    Model     *model           = anim_entity->get_model();
    Animator  *animator        = anim_entity->get_animator();
    glm::mat4 &model_transform = anim_entity->get_model_transform();

    _shaders["Anim"].use();
    std::vector<glm::mat4> transforms = animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i) {
      _shaders["Anim"].setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
    }

    _shaders["Anim"].setMat4("model", model_transform);
    model->draw(_shaders["Anim"]);
  }
}
} // namespace OpenGLRenderer
