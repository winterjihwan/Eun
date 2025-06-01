#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include <glm/gtx/euler_angles.hpp>

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _view;
extern glm::mat4                               _projection;

void geometry_pass() {
  Camera *camera = Game::get_camera();
  Player *player = Game::get_player();

  Shader &shader = _shaders["Model"];

  // Scene
  shader.use();

  // Light Uniforms
  // Directional
  shader.setVec3("viewPos", player->get_pos());
  shader.setFloat("shininess", 32.0f);
  shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

  // FlashLight
  shader.setBool("flashLight.is_on", player->get_flashlight_on());
  shader.setVec3("flashLight.position", player->get_pos());
  shader.setVec3("flashLight.direction", camera->get_front());
  shader.setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
  shader.setVec3("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader.setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
  shader.setFloat("flashLight.constant", 1.0f);
  shader.setFloat("flashLight.linear", 0.09f);
  shader.setFloat("flashLight.quadratic", 0.032f);
  shader.setFloat("flashLight.cutOff", glm::cos(glm::radians(12.5f)));
  shader.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  // Model Shader
  shader.setMat4("projection", _projection);
  shader.setMat4("view", _view);

  // TODO: Move below to World.cpp
  // TODO: Use custom Map

  // Map
  glm::mat4 model_scene = glm::mat4(1.0f);
  shader.setMat4("model", model_scene);
  Model *scene = AssetManager::get_model_by_name("Map");
  scene->draw(shader);

  // // HACK
  // // Glock
  // Animator *animator       = AssetManager::get_animator_by_name("Brian_Idle");
  // glm::mat4 hand_transform = animator->GetBoneGlobalTransform("mixamorig12_LeftHand");
  //
  // glm::mat4 pete_transform = glm::translate(glm::mat4(1.0f), glm::vec3(13.0f, 0, -5.0f));
  //
  // glm::mat4 local_offset = glm::mat4(1.0f);
  // local_offset           = glm::translate(local_offset, glm::vec3(0.0f, 15.0f, 0.1f));
  // local_offset *=
  //     glm::eulerAngleXYZ(glm::radians(0.0f), glm::radians(180.0f), glm::radians(270.0f));
  // local_offset = glm::scale(local_offset, glm::vec3(100.0f));
  //
  // glm::mat4 model_glock = pete_transform * hand_transform * local_offset;
  // model_glock           = pete_transform * hand_transform * local_offset;
  //
  // shader.setMat4("model", model_glock);
  // Model *glock = AssetManager::get_model_by_name("Glock");
  // glock->draw(shader);
}
} // namespace OpenGLRenderer
