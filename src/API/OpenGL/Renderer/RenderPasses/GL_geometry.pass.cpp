#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Physics/Physics.h"

namespace OpenGLRenderer {
extern std::unordered_map<std::string, Shader> _shaders;
extern glm::mat4                               _projection;
extern glm::mat4                               _view;

void geometry_pass() {
  Camera *camera = Game::get_camera();
  Player *player = Game::get_player();

  // Scene
  _shaders["Model"].use();

  // Light Uniforms
  // Directional
  _shaders["Model"].setVec3("viewPos", player->get_pos());
  _shaders["Model"].setFloat("shininess", 32.0f);
  _shaders["Model"].setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  _shaders["Model"].setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  _shaders["Model"].setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  _shaders["Model"].setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

  // FlashLight
  _shaders["Model"].setBool("flashLight.is_on", player->get_flashlight_on());
  _shaders["Model"].setVec3("flashLight.position", player->get_pos());
  _shaders["Model"].setVec3("flashLight.direction", camera->get_front());
  _shaders["Model"].setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
  _shaders["Model"].setVec3("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
  _shaders["Model"].setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
  _shaders["Model"].setFloat("flashLight.constant", 1.0f);
  _shaders["Model"].setFloat("flashLight.linear", 0.09f);
  _shaders["Model"].setFloat("flashLight.quadratic", 0.032f);
  _shaders["Model"].setFloat("flashLight.cutOff", glm::cos(glm::radians(12.5f)));
  _shaders["Model"].setFloat("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  // Model Shader
  _shaders["Model"].setMat4("projection", _projection);
  _shaders["Model"].setMat4("view", _view);

  // TODO: Move below to World.cpp

  // Map
  glm::mat4 model_scene = glm::mat4(1.0f);
  _shaders["Model"].setMat4("model", model_scene);
  Model *scene = AssetManager::get_model_by_name("Map");
  scene->Draw(_shaders["Model"]);

  // Test Sphere
  glm::mat4 model_test_sphere = glm::mat4(1.0f);
  model_test_sphere = glm::translate(model_test_sphere, glm::vec3(13.0f, PLAYER_HEIGHT, 0.0f));
  model_test_sphere = glm::translate(model_test_sphere, glm::vec3(0.0f, 0.0f, -3.0f));
  _shaders["Model"].setMat4("model", model_test_sphere);
  Mesh *test_sphere = AssetManager::get_mesh_by_name("Test_Sphere");
  test_sphere->Draw(_shaders["Model"]);

  // Brian Collider Capsule
  glm::mat4   brian_collider = glm::mat4(1.0f);
  BodyID     *id             = AssetManager::get_collider_by_name("Brian");
  const Body *body           = Physics::get_physics_system().GetBodyLockInterface().TryGetBody(*id);
  if (body) {
    RVec3 pos      = body->GetCenterOfMassPosition();
    brian_collider = glm::translate(brian_collider, glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ()));
  }
  _shaders["Model"].setMat4("model", brian_collider);
  Mesh *test_collider = AssetManager::get_mesh_by_name("Test_Capsule");
  test_collider->Draw(_shaders["Model"]);
}
} // namespace OpenGLRenderer
