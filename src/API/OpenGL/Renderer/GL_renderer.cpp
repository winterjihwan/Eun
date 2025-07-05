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

// Uniforms
constexpr glm::vec3 LIGHT_POS  = glm::vec3(-5.0f, 5.0f, -5.0f);
constexpr float     LIGHT_NEAR = 1.0f;
constexpr float     LIGHT_FAR  = 75.0f;
glm::mat4           _projection;
glm::mat4           _view;
glm::mat4           _light_space;

void init() {
  Viewport viewport = Backend::get_viewport();

  // HACK
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  /* Shaders */
  _shaders["Model"]           = Shader("shaders/model.vert", "shaders/model.frag");
  _shaders["Sky"]             = Shader("shaders/sky.vert", "shaders/sky.frag");
  _shaders["Anim"]            = Shader("shaders/anim.vert", "shaders/anim.frag");
  _shaders["BloodVolumetric"] = Shader("shaders/blood.vert", "shaders/blood.frag");
  _shaders["Decal"]           = Shader("shaders/decal.vert", "shaders/decal.frag");
  _shaders["DecalBlood"]      = Shader("shaders/decal.vert", "shaders/decal_blood.frag");
  _shaders["UI"]              = Shader("shaders/ui.vert", "shaders/ui.frag");
  _shaders["Light"]           = Shader("shaders/light.vert", "shaders/light.frag");
  _shaders["Shadow"]          = Shader("shaders/shadow.vert", "shaders/shadow.frag");
  _shaders["Shadow_Anim"]     = Shader("shaders/shadow_anim.vert", "shaders/shadow.frag");
  _shaders["Texture"]         = Shader("shaders/texture.vert", "shaders/texture.frag");
  _shaders["AABB"]            = Shader("shaders/aabb.vert", "shaders/aabb.frag");

  /* Framebuffers */
  _frame_buffers["G_Buffer"] = OpenGLFrameBuffer("G_Buffer", *viewport.width, *viewport.height);
  _frame_buffers["G_Buffer"].bind();
  _frame_buffers["G_Buffer"].create_color_attachment("Position", GL_RGBA16F);
  _frame_buffers["G_Buffer"].create_color_attachment("Normal", GL_RGBA16F);
  _frame_buffers["G_Buffer"].create_color_attachment("AlbedoSpec", GL_RGBA);
  _frame_buffers["G_Buffer"].create_render_buffer_attachment();
  _frame_buffers["G_Buffer"].sanitize_check();
  _frame_buffers["G_Buffer"].unbind();

  _frame_buffers["Shadow"] = OpenGLFrameBuffer("Shadow", SHADOW_WIDTH, SHADOW_HEIGHT);
  _frame_buffers["Shadow"].bind();
  _frame_buffers["Shadow"].create_depth_attachment();
  _frame_buffers["Shadow"].draw_none();
  _frame_buffers["Shadow"].read_none();
  _frame_buffers["Shadow"].sanitize_check();
  _frame_buffers["Shadow"].unbind();

  /* Base Uniforms */
  glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, LIGHT_NEAR, LIGHT_FAR);
  glm::mat4 light_view       = glm::lookAt(LIGHT_POS, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
  _light_space               = light_projection * light_view;

  init_light();
  init_skybox();
  init_aabb();
}

void render_game() {
  OpenGLFrameBuffer &g_buffer = _frame_buffers["G_Buffer"];
  Camera            *camera   = Game::get_camera();

  // Per Frame Uniforms
  _projection = camera->projection();
  _view       = camera->view_matrix();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Shadow Pass
  shadow_pass();

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
  aabb_pass();
}

Shader &get_shader(const std::string &name) {
  auto it = _shaders.find(name);
  if (it != _shaders.end()) {
    return it->second;
  }

  std::cerr << "GL_renderer::get_shader() fail for shader: " << name << '\n';
  assert(0);
}

OpenGLFrameBuffer &get_frame_buffer(const std::string &name) {
  auto it = _frame_buffers.find(name);
  if (it != _frame_buffers.end()) {
    return it->second;
  }

  std::cerr << "GL_renderer::get_frame_buffer() fail for framebuffer: " << name << '\n';
  assert(0);
}

} // namespace OpenGLRenderer
