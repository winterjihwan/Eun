#include "API/OpenGL/Renderer/GL_renderer.h"
#include "API/OpenGL/Renderer/GL_cubemapView.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Types/Game/AnimEntity.h"
#include "Types/Renderer/Shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

unsigned int load_cubemap(std::vector<std::string> faces);

namespace OpenGLRenderer {
std::unordered_map<std::string, Shader>            _shaders;
std::unordered_map<std::string, OpenGLCubemapView> _cubemap_views;
std::vector<AnimEntity>                            _anim_entities;

// HACK
glm::mat4 _projection;
glm::mat4 _view;

// HACK
unsigned int _sky_vao;

void init() {
  /* Shaders */
  _shaders["Model"]           = Shader("shaders/model.vert", "shaders/model.frag");
  _shaders["Sky"]             = Shader("shaders/sky.vert", "shaders/sky.frag");
  _shaders["Anim"]            = Shader("shaders/anim.vert", "shaders/anim.frag");
  _shaders["BloodVolumetric"] = Shader("shaders/blood.vert", "shaders/blood.frag");
  _shaders["Decal"]           = Shader("shaders/decal.vert", "shaders/decal.frag");
  _shaders["DecalBlood"]      = Shader("shaders/decal.vert", "shaders/decal_blood.frag");
  _shaders["UI"]              = Shader("shaders/ui.vert", "shaders/ui.frag");

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
  Shader &sky_shader         = _shaders["Sky"];

  sky_shader.use();
  sky_shader.setInt("skybox", 0);
}

void render_game() {
  Camera *camera = Game::get_camera();

  // Per Frame Transformations
  _projection = glm::perspective(
      glm::radians(camera->get_zoom()), (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT, NEAR, FAR);
  _view = camera->view_matrix();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Render Passes
  geometry_pass();
  skybox_pass();
  anim_pass();
  blood_volumetric_pass();
  decal_pass();
  ui_pass();
}

} // namespace OpenGLRenderer
