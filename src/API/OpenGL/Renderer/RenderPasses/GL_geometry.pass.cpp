#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "Renderer/RenderDataManager.h"
#include <glm/gtx/euler_angles.hpp>
#include <variant>

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
  glm::mat4 map = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
  map           = glm::rotate(map, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  shader.setMat4("model", map);
  Mesh *plane = AssetManager::get_mesh_by_name("Plane");
  plane->draw(shader);
  // Model *scene = AssetManager::get_model_by_name("Map");
  // scene->draw(shader);

  std::vector<Entity *> entities = RenderDataManager::get_entities();
  for (Entity *entity : entities) {
    std::variant<Model *, Mesh *> renderable = entity->get_renderable();
    glm::mat4                     transform  = entity->get_transform();

    shader.setMat4("model", transform);
    std::visit([&](auto *renderable) { renderable->draw(shader); }, renderable);
  }
}
} // namespace OpenGLRenderer
