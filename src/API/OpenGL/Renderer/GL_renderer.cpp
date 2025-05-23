#include "API/OpenGL/Renderer/GL_cubemapView.h"
#include "AssetManager/AssetManager.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Mesh.h"

unsigned int load_cubemap(vector<std::string> faces);

namespace OpenGLRenderer {
std::unordered_map<std::string, Shader>            _shaders;
std::unordered_map<std::string, OpenGLCubemapView> _cubemap_views;

// HACK
unsigned int _sky_vao;

void init() {
  /* Shaders */
  _shaders["Model"] = Shader("shaders/model.vert", "shaders/model.frag");
  _shaders["Sky"]   = Shader("shaders/sky.vert", "shaders/sky.frag");
  _shaders["Anim"]  = Shader("shaders/anim.vert", "shaders/anim.frag");

  // Skybox
  float skybox_vertices[] = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                             1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                             -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                             -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                             1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                             1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                             -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                             1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                             -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                             1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                             -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                             1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  // Skybox VAO
  unsigned int skyVBO;
  glGenVertexArrays(1, &_sky_vao);
  glGenBuffers(1, &skyVBO);
  glBindVertexArray(_sky_vao);
  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  _cubemap_views["NightSky"] = OpenGLCubemapView();

  _shaders["Sky"].use();
  _shaders["Sky"].setInt("skybox", 0);
}

void render_game() {
  Camera *camera = Game::get_camera();
  Player *player = Game::get_player();

  // Per Frame Transformations
  glm::mat4 projection = glm::perspective(
      glm::radians(camera->get_zoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  glm::mat4 view =
      glm::lookAt(player->get_pos(), player->get_pos() + camera->get_front(), camera->get_up());

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
  _shaders["Model"].setMat4("projection", projection);
  _shaders["Model"].setMat4("view", view);

  // Scene
  glm::mat4 model_scene = glm::mat4(1.0f);
  _shaders["Model"].setMat4("model", model_scene);
  Model *scene = AssetManager::get_model_by_name("Map");
  scene->Draw(_shaders["Model"]);

  // Test Sphere
  glm::mat4 model_test_sphere = glm::mat4(1.0f);
  model_test_sphere = glm::translate(model_test_sphere, glm::vec3(13.0f, PLAYER_HEIGHT, 0.0f));
  model_test_sphere = glm::translate(model_test_sphere, glm::vec3(0.0f, 0.0f, -3.0f));
  _shaders["Model"].setMat4("model", model_test_sphere);
  Mesh *test_sphere = &AssetManager::get_meshes().front();
  test_sphere->Draw(_shaders["Model"]);

  // Human
  glm::mat4 model_human = glm::mat4(1.0f);
  model_human           = glm::translate(model_human, glm::vec3(13.0f, PLAYER_HEIGHT, 0.0f));
  model_human           = glm::translate(model_human, glm::vec3(0.0f, 0.0f, -3.0f));
  _shaders["Model"].setMat4("model", model_human);
  Model *human = AssetManager::get_model_by_name("Human");
  human->Draw(_shaders["Model"]);

  // Animation Shader
  _shaders["Anim"].use();
  _shaders["Anim"].setMat4("projection", projection);
  _shaders["Anim"].setMat4("view", view);

  // Vampire
  Animator *vampire_dance_animator = AssetManager::get_animator_by_name("Vampire");
  auto      transforms             = vampire_dance_animator->GetFinalBoneMatrices();
  for (int i = 0; i < transforms.size(); ++i)
    _shaders["Anim"].setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

  glm::mat4 model_vampire = glm::mat4(1.0f);
  model_vampire           = glm::translate(model_vampire, glm::vec3(13.0f, PLAYER_HEIGHT, -5.0f));
  model_vampire           = glm::scale(model_vampire, glm::vec3(.5f, .5f, .5f));
  _shaders["Anim"].setMat4("model", model_vampire);

  Model *vampire = AssetManager::get_model_by_name("Vampire");
  vampire->Draw(_shaders["Anim"]);

  // Weapon View
  // HACK
  Animator *weapon_view_animator = Game::get_player()->get_weapon_view_animator();
  transforms                     = weapon_view_animator->GetFinalBoneMatrices();
  for (int i = 0; i < transforms.size(); ++i)
    _shaders["Anim"].setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

  glm::mat4 model_weapon = glm::mat4(1.0f);
  model_weapon           = glm::translate(model_weapon, Game::get_player()->get_pos());
  model_weapon           = glm::translate(model_weapon, glm::vec3(0.0f, -0.3f, 0.0f));
  model_weapon           = glm::translate(model_weapon, Game::get_camera()->get_front() * 0.5f);
  glm::quat rot = glm::quatLookAt(-Game::get_camera()->get_front(), Game::get_camera()->get_up());
  model_weapon *= glm::toMat4(rot);
  model_weapon = glm::scale(model_weapon, glm::vec3(0.5f));
  _shaders["Anim"].setMat4("model", model_weapon);

  const std::string &current_weapon_name = Game::get_player()->get_current_weapon_info()->name;
  Model             *weapon_model        = AssetManager::get_model_by_name(current_weapon_name);
  weapon_model->Draw(_shaders["Anim"]);

  // Cubemap
  glDepthFunc(GL_LEQUAL);
  _shaders["Sky"].use();
  view = glm::lookAt(Game::get_player()->get_pos(),
                     Game::get_player()->get_pos() + Game::get_camera()->get_front(),
                     Game::get_camera()->get_up());
  view = glm::mat4(glm::mat3(view));
  _shaders["Sky"].setMat4("view", view);
  _shaders["Sky"].setMat4("projection", projection);

  glBindVertexArray(_sky_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap_views["NightSky"].get_handle());
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}
} // namespace OpenGLRenderer
