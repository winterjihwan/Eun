#include "UIBackend.h"
#include "AssetManager/AssetManager.h"
#include "Defines.h"
#include "Renderer/RenderDataManager.h"
#include "TextBlitter.h"
#include "Types.h"
#include "UI/FontSpriteSheet.h"

namespace UIBackend {

Mesh2D                    _ui_mesh;
std::vector<Vertex2D>     _vertices;
std::vector<uint32_t>     _indices;
std::vector<UIRenderItem> _render_items;

void init() {
  // Export
  std::string name = "StandardFont";
  std::string characters =
      R"(!"#$%&'*+,-./0123456789:;<=>?_ABCDEFGHIJKLMNOPQRSTUVWXYZ\^_`abcdefghijklmnopqrstuvwxyz )";
  std::string texture_source_path = "res/fonts/raw_images/standard_font/";
  std::string output_path         = "res/fonts/";
  FontSpriteSheetPacker::Export(name, characters, texture_source_path, output_path);

  // Import
  FontSpriteSheet standard_font = FontSpriteSheetPacker::Import("res/fonts/StandardFont.json");
  TextBlitter::add_font(standard_font);
}

void submit_render_items() {
  for (UIRenderItem &ui_render_item : _render_items) {
    RenderDataManager::submit_ui_render_item(&ui_render_item);
  }
}

void update() {
  _ui_mesh.update_vertex_buffer(_vertices, _indices);
  _vertices.clear();
  _indices.clear();
}

void end_frame() {
  _render_items.clear();
}

void blit_text(const std::string &text,
               const std::string &font_name,
               int                x,
               int                y,
               UIAlignment        alignment,
               float              scale) {
  FontSpriteSheet *font_sprite_sheet = TextBlitter::get_font_sprite_sheet(font_name);
  if (!font_sprite_sheet) {
    std::cout << "UIBackEnd::BlitText() failed to find " << font_name << "\n";
    return;
  }

  int        base_vertex = _vertices.size();
  Mesh2DData mesh_data   = TextBlitter::blit_text(text,
                                                font_name,
                                                x,
                                                y,
                                                glm::ivec2(VIEWPORT_WIDTH, VIEWPORT_HEIGHT),
                                                alignment,
                                                scale,
                                                base_vertex);

  _vertices.insert(
      std::end(_vertices), std::begin(mesh_data.vertices), std::end(mesh_data.vertices));
  _indices.insert(std::end(_indices), std::begin(mesh_data.indices), std::end(mesh_data.indices));

  UIRenderItem &renderItem = _render_items.emplace_back();
  renderItem.base_vertex   = 0;
  renderItem.base_index    = _indices.size() - mesh_data.indices.size();
  renderItem.index_count   = mesh_data.indices.size();
  renderItem.texture       = AssetManager::get_texture_by_name(font_name);
}

void blit_texture(const std::string &texture_name,
                  glm::ivec2         location,
                  UIAlignment        alignment,
                  glm::vec4          color_tint,
                  glm::ivec2         size) {
  Texture *texture = AssetManager::get_texture_by_name(texture_name);
  if (texture == 0) {
    std::cout << "BlitTexture() failed. Could not find texture '" << texture_name << "'\n";
    return;
  }

  float tex_width  = (size.x != -1) ? size.x : texture->width;
  float tex_height = (size.y != -1) ? size.y : texture->height;

  // Alignment
  switch (alignment) {
  case UIAlignment::CENTERED:
    location.x -= tex_width * 0.5f;
    location.y -= tex_height * 0.5f;
    break;
  case UIAlignment::TOP_LEFT:
    break;
  case UIAlignment::TOP_RIGHT:
    location.x -= tex_width;
    break;
  case UIAlignment::BOTTOM_LEFT:
    location.y -= tex_height;
    break;
  case UIAlignment::BOTTOM_RIGHT:
    location.x -= tex_width;
    location.y -= tex_height;
    break;
  case UIAlignment::CENTERED_HORIZONTAL:
    location.x -= tex_width * 0.5f;
    break;
  case UIAlignment::CENTERED_VERTICAL:
    location.y -= tex_height * 0.5f;
  }

  glm::ivec2 resolution(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

  float x0 = (location.x / static_cast<float>(resolution.x)) * 2.0f - 1.0f;
  float y0 = 1.0f - (location.y / static_cast<float>(resolution.y)) * 2.0f;
  float x1 = ((location.x + tex_width) / static_cast<float>(resolution.x)) * 2.0f - 1.0f;
  float y1 = 1.0f - ((location.y + tex_height) / static_cast<float>(resolution.y)) * 2.0f;

  int base_vertex = _vertices.size();
  int base_index  = _indices.size();

  _vertices.reserve(base_vertex + 4);
  _vertices.push_back({{x0, y0}, {0.0f, 0.0f}, color_tint});
  _vertices.push_back({{x1, y0}, {1.0f, 0.0f}, color_tint});
  _vertices.push_back({{x1, y1}, {1.0f, 1.0f}, color_tint});
  _vertices.push_back({{x0, y1}, {0.0f, 1.0f}, color_tint});

  _indices.reserve(base_index + 6);
  _indices.push_back(base_vertex + 0);
  _indices.push_back(base_vertex + 1);
  _indices.push_back(base_vertex + 2);
  _indices.push_back(base_vertex + 0);
  _indices.push_back(base_vertex + 2);
  _indices.push_back(base_vertex + 3);

  UIRenderItem &render_item = _render_items.emplace_back();
  render_item.base_vertex   = 0;
  render_item.base_index    = base_index;
  render_item.index_count   = 6;
  render_item.texture       = texture;
}

Mesh2D &get_ui_mesh() {
  return _ui_mesh;
}

std::vector<UIRenderItem> &get_render_items() {
  return _render_items;
}
} // namespace UIBackend
