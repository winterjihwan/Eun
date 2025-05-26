#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include <glm/gtx/string_cast.hpp>

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
  Mesh &test_sphere = AssetManager::get_meshes().back();
  test_sphere.Draw(_shaders["Model"]);
}
} // namespace OpenGLRenderer
