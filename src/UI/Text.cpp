#include "Text.h"
#include "Font.h"
#include "Types.h"
#include <iostream>
#include <unordered_map>

namespace Text {
FT_Library                                _ft_lib;
std::vector<Font>                         _fonts;
std::unordered_map<std::string, uint32_t> _font_indices;

void init() {
  if (FT_Init_FreeType(&_ft_lib)) {
    std::cout << "Font::init() fail" << std::endl;
    assert(0);
  }
}

void add_font(const std::string &path, const std::string &name) {
  Font &font = _fonts.emplace_back();
  font.init(&_ft_lib, path, name);
}

std::vector<Mesh2DData> blit_text(const std::string &text,
                                  const std::string &font_name,
                                  int                x,
                                  int                y,
                                  glm::ivec2         viewport_size,
                                  UIAlignment        alignment, // TODO: Do Alignment
                                  float              scale,
                                  glm::vec4          color // TODO: Do Color
) {
  std::vector<Mesh2DData> meshChunks;

  Font                      &font            = *get_font_by_name(font_name);
  std::map<char, Character> &font_characters = font.get_characters();

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = font_characters[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    Mesh2DData chunk;
    chunk.texture_id = ch.texture_id;

    chunk.vertices.push_back({glm::vec4(xpos, ypos + h, 0.0f, 0.0f)});
    chunk.vertices.push_back({glm::vec4(xpos, ypos, 0.0f, 1.0f)});
    chunk.vertices.push_back({glm::vec4(xpos + w, ypos, 1.0f, 1.0f)});
    chunk.vertices.push_back({glm::vec4(xpos, ypos + h, 0.0f, 0.0f)});
    chunk.vertices.push_back({glm::vec4(xpos + w, ypos, 1.0f, 1.0f)});
    chunk.vertices.push_back({glm::vec4(xpos + w, ypos + h, 1.0f, 0.0f)});

    meshChunks.push_back(std::move(chunk));

    x += (ch.advance >> 6) * scale;
  }

  return meshChunks;
}

Font *get_font_by_name(const std::string &name) {
  for (Font &font : _fonts) {
    if (font.get_name() == name) {
      return &font;
    }
  }

  std::cout << "Font::GetFontByName() failed, no font with name " << name << std::endl;
  assert(0);
}

} // namespace Text
