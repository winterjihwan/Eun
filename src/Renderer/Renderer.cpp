#include "Renderer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"
#include "UI/UIBackend.h"

namespace Renderer {
void init() {
  OpenGLRenderer::init();
}

void render_game() {
  OpenGLRenderer::render_game();
}

void render_loading_screen() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  UIBackend::update();
  OpenGLRenderer::ui_pass();
  UIBackend::end_frame();
}
} // namespace Renderer
