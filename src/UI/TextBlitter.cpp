#include "TextBlitter.h"
#include "AssetManager/AssetManager.h"
#include "FontSpriteSheet.h"
#include "Types.h"
#include <iostream>
#include <unordered_map>

namespace TextBlitter {
std::vector<FontSpriteSheet>              _font_sprite_sheets;
std::unordered_map<std::string, uint32_t> _font_indices;

glm::vec4 parse_color_tag(const std::string &tag);

Mesh2DData blit_text(const std::string &text,
                     const std::string &font_name,
                     int                x,
                     int                y,
                     glm::ivec2         viewport_size,
                     UIAlignment        alignment,
                     float              scale,
                     uint32_t           baseVertex) {
  FontSpriteSheet *sprite_sheet = get_font_sprite_sheet(font_name);
  Mesh2DData       mesh_data;

  Texture *texture = AssetManager::get_texture_by_name(font_name);

  if (!sprite_sheet || texture == 0) {
    std::cout << "TextBlitter::blit_text() failed, no sprite sheet or texture for text " << text
              << " with font name" << font_name << std::endl;
    return mesh_data;
  }

  float     cursor_x           = static_cast<float>(x);
  float     cursor_y           = static_cast<float>(viewport_size.y - y);
  float     reach_x            = cursor_x;
  float     reach_y            = cursor_y;
  float     inv_texture_width  = 1.0f / static_cast<float>(sprite_sheet->m_textureWidth);
  float     inv_texture_height = 1.0f / static_cast<float>(sprite_sheet->m_textureHeight);
  float     half_pixel_u       = 0.5f * inv_texture_width;
  float     half_pixel_v       = 0.5f * inv_texture_height;
  glm::vec4 color              = glm::vec4(1.0f);

  mesh_data.vertices.reserve(text.length() * 4);
  mesh_data.indices.reserve(text.length() * 6);

  for (size_t i = 0; i < text.length();) {
    // Parse Color
    if (text.compare(i, 5, "[COL=") == 0) {
      size_t end = text.find("]", i);
      if (end != std::string::npos) {
        color = parse_color_tag(text.substr(i, end - i + 1));
        i     = end;
        continue;
      }
    }

    char c = text[i];

    // Space
    if (c == ' ') {
      size_t spaceIndex = sprite_sheet->m_characters.find(' ');
      int    spaceWidth =
          (spaceIndex != std::string::npos) ? sprite_sheet->m_charDataList[spaceIndex].width : 0;
      cursor_x += spaceWidth * scale;
      reach_x = std::max(reach_x, cursor_x);
      i++;
      continue;
    }

    // Newline
    if (c == '\n') {
      cursor_x = static_cast<float>(x);
      cursor_y -= (sprite_sheet->m_lineHeight + 1) * scale;
      reach_y = std::min(reach_y, cursor_y);
      i++;
      continue;
    }

    int char_index = sprite_sheet->m_characters.find(c);

    if (char_index != std::string::npos) {
      const FontSpriteSheet::CharData &char_data = sprite_sheet->m_charDataList[char_index];

      // UV
      float u0 = (char_data.offsetX + half_pixel_u) * inv_texture_width;
      float v0 = (char_data.offsetY + char_data.height - half_pixel_v) * inv_texture_height;
      float u1 = (char_data.offsetX + char_data.width - half_pixel_u) * inv_texture_width;
      float v1 = (char_data.offsetY + half_pixel_v) * inv_texture_height;

      // Pos
      float x0 = (cursor_x / viewport_size.x) * 2.0f - 1.0f;
      float y0 = (cursor_y / viewport_size.y) * 2.0f - 1.0f;
      float x1 = ((cursor_x + char_data.width * scale) / viewport_size.x) * 2.0f - 1.0f;
      float y1 = ((cursor_y - char_data.height * scale) / viewport_size.y) * 2.0f - 1.0f;

      // Vertex
      mesh_data.vertices.push_back({{x0, y0}, {u0, v1}, color}); // BL
      mesh_data.vertices.push_back({{x1, y0}, {u1, v1}, color}); // BR
      mesh_data.vertices.push_back({{x1, y1}, {u1, v0}, color}); // TR
      mesh_data.vertices.push_back({{x0, y1}, {u0, v0}, color}); // TL

      // Index
      uint32_t vertexOffset = static_cast<uint32_t>(mesh_data.vertices.size()) - 4;
      mesh_data.indices.push_back(baseVertex + vertexOffset + 0);
      mesh_data.indices.push_back(baseVertex + vertexOffset + 1);
      mesh_data.indices.push_back(baseVertex + vertexOffset + 2);
      mesh_data.indices.push_back(baseVertex + vertexOffset + 0);
      mesh_data.indices.push_back(baseVertex + vertexOffset + 2);
      mesh_data.indices.push_back(baseVertex + vertexOffset + 3);

      cursor_x += char_data.width * scale;
      reach_x = std::max(reach_x, cursor_x);
      reach_y = std::min(reach_y, cursor_y - char_data.height * scale);
    }
    i++;
  }

  // Post Alignment
  if (alignment != UIAlignment::TOP_LEFT) {
    float text_width  = reach_x - x;
    float text_height = reach_y - (viewport_size.y - y);
    float offset_x    = (text_width / viewport_size.x) * 2.0f;
    float offset_y    = (text_height / viewport_size.y) * 2.0f;

    for (Vertex2D &vertex : mesh_data.vertices) {
      switch (alignment) {
      case UIAlignment::CENTERED:
        vertex.position.x -= offset_x * 0.5f;
        vertex.position.y -= offset_y * 0.5f;
        break;

      case UIAlignment::CENTERED_HORIZONTAL:
        vertex.position.x -= offset_x * 0.5f;
        break;

      case UIAlignment::CENTERED_VERTICAL:
        vertex.position.y -= offset_y * 0.5f;
        break;

      case UIAlignment::TOP_RIGHT:
        vertex.position.x -= offset_x;
        break;

      case UIAlignment::BOTTOM_LEFT:
        vertex.position.y -= offset_y;
        break;

      case UIAlignment::BOTTOM_RIGHT:
        vertex.position.x -= offset_x;
        vertex.position.y -= offset_y;
        break;

      default:
        break;
      }
    }
  }

  return mesh_data;
}

void add_font(const FontSpriteSheet &sprite_sheet) {
  if (_font_indices.find(sprite_sheet.m_name) != _font_indices.end()) {
    std::cout << "Font already exists: " << sprite_sheet.m_name << "\n";
    return;
  }
  uint32_t index                     = _font_sprite_sheets.size();
  _font_indices[sprite_sheet.m_name] = index;
  _font_sprite_sheets.push_back(sprite_sheet);
}

FontSpriteSheet *get_font_sprite_sheet(const std::string &name) {
  for (FontSpriteSheet &font_sprite_sheet : _font_sprite_sheets) {
    if (font_sprite_sheet.m_name == name) {
      return &font_sprite_sheet;
    }
  }

  std::cout << "TextBlitter::get_font_sprite_sheet() failed, no font sprite sheet with name: "
            << name << std::endl;
  assert(0);
}

glm::vec4 parse_color_tag(const std::string &tag) {
  glm::vec4 color(1.0f);
  size_t    start = tag.find("[COL=") + 5;
  if (start == std::string::npos) {
    return color;
  }
  size_t end = tag.find("]", start);
  if (end == std::string::npos) {
    return color;
  }

  // Color
  const char *c_str = tag.c_str() + start;
  char       *end_ptr;
  color.r = std::strtof(c_str, &end_ptr);
  if (*end_ptr != ',') {
    return color;
  }

  color.g = std::strtof(end_ptr + 1, &end_ptr);
  if (*end_ptr != ',') {
    return color;
  }

  color.b = std::strtof(end_ptr + 1, &end_ptr);
  if (*end_ptr != ',') {
    return color;
  }

  color.a = std::strtof(end_ptr + 1, &end_ptr);
  return color;
}

} // namespace TextBlitter
