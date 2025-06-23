#include "API/OpenGL/Renderer/GL_renderer.h"
#include "API/OpenGL/Renderer/GL_cubemapView.h"
#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "Backend/Backend.h"
#include "Core/Game.h"
#include "Defines.h"
#include "Types.h"
#include "Types/Game/AnimEntity.h"
#include "Types/Renderer/Shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

unsigned int load_cubemap(std::vector<std::string> faces);

namespace OpenGLRenderer {
std::unordered_map<std::string, Shader>            _shaders;
std::unordered_map<std::string, OpenGLFrameBuffer> _frame_buffers;
std::unordered_map<std::string, OpenGLCubemapView> _cubemap_views;
std::vector<AnimEntity>                            _anim_entities;

// TODO: Remove Externs

// HACK
glm::mat4 _projection;
glm::mat4 _view;

void init() {
  Viewport viewport = Backend::get_viewport();

  // HACK
  glEnable(GL_DEPTH_TEST);

  /* Shaders */
  _shaders["Model"]           = Shader("shaders/model.vert", "shaders/model.frag");
  _shaders["Sky"]             = Shader("shaders/sky.vert", "shaders/sky.frag");
  _shaders["Anim"]            = Shader("shaders/anim.vert", "shaders/anim.frag");
  _shaders["BloodVolumetric"] = Shader("shaders/blood.vert", "shaders/blood.frag");
  _shaders["Decal"]           = Shader("shaders/decal.vert", "shaders/decal.frag");
  _shaders["DecalBlood"]      = Shader("shaders/decal.vert", "shaders/decal_blood.frag");
  _shaders["UI"]              = Shader("shaders/ui.vert", "shaders/ui.frag");
  _shaders["Light"]           = Shader("shaders/light.vert", "shaders/light.frag");

  /* Framebuffers */
  _frame_buffers["G_Buffer"] = OpenGLFrameBuffer("G_Buffer", *viewport.width, *viewport.height);
  _frame_buffers["G_Buffer"].bind();
  _frame_buffers["G_Buffer"].create_attachment("Position", GL_RGBA16F);
  _frame_buffers["G_Buffer"].create_attachment("Normal", GL_RGBA16F);
  _frame_buffers["G_Buffer"].create_attachment("AlbedoSpec", GL_RGBA);
  _frame_buffers["G_Buffer"].create_depth_attachment();
  // HACK
  _frame_buffers["G_Buffer"].sanitize_check();
  _frame_buffers["G_Buffer"].unbind();

  init_light();
  init_skybox();
}

void render_game() {
  OpenGLFrameBuffer &g_buffer = _frame_buffers["G_Buffer"];
  Camera            *camera   = Game::get_camera();

  // Per Frame Transformations
  _projection = glm::perspective(
      glm::radians(camera->get_zoom()), (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT, NEAR, FAR);
  _view = camera->view_matrix();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Deferred Pass
  g_buffer.clear_bind();
  geometry_pass();
  anim_pass();
  light_pass();

  // Forward Pass
  g_buffer.blit_and_bind_to_default_frame_buffer();
  blood_volumetric_pass();
  decal_pass();
  skybox_pass();
  ui_pass();
}

} // namespace OpenGLRenderer
