#include "Renderer.h"
#include "API/OpenGL/Renderer/GL_renderer.h"

namespace Renderer {
void init() {
  OpenGLRenderer::init();
}
void render_game() {
  OpenGLRenderer::render_game();
}
} // namespace Renderer
