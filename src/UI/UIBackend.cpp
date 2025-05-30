#include "UIBackend.h"
#include "Defines.h"
#include "Font.h"
#include "Renderer/RenderDataManager.h"
#include "Text.h"
#include "Types.h"

namespace UIBackend {

Mesh2D                    _ui_mesh;
std::vector<Vertex2D>     _vertices;
std::vector<UIRenderItem> _render_items;

void init() {
  Text::init();

  Text::add_font("res/fonts/NoScary.ttf", "NoScary");
}

void update() {
  _ui_mesh.update_vertex_buffer(_vertices);
  _vertices.clear();
}

void blit_text(const std::string &text,
               const std::string &font_name,
               int                x,
               int                y,
               UIAlignment        alignment,
               float              scale,
               glm::vec4          color) {
  Font *font = Text::get_font_by_name(font_name);
  if (!font) {
    std::cout << "UIBackEnd::BlitText() failed to find " << font_name << "\n";
    return;
  }
  int base_vertex = _vertices.size();

  std::vector<Mesh2DData> meshChunks = Text::blit_text(
      text, font_name, x, y, glm::ivec2(VIEWPORT_WIDTH, VIEWPORT_HEIGHT), alignment, scale, color);

  for (const auto &chunk : meshChunks) {
    _vertices.insert(_vertices.end(), chunk.vertices.begin(), chunk.vertices.end());

    UIRenderItem &render_item = _render_items.emplace_back();
    render_item.base_vertex   = base_vertex;
    render_item.vertex_count  = chunk.vertices.size();
    render_item.texture_id    = chunk.texture_id;

    base_vertex += chunk.vertices.size();
  }
}

void submit_render_items() {
  for (UIRenderItem &render_item : _render_items) {
    RenderDataManager::submit_ui_render_item(&render_item);
  }
}

void end_frame() {
  _render_items.clear();
}

Mesh2D &get_ui_mesh() {
  return _ui_mesh;
}

std::vector<UIRenderItem> &get_render_items() {
  return _render_items;
}
} // namespace UIBackend
