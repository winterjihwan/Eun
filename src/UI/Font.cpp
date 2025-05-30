#include "Font.h"
#include <ft2build.h>
#include <glad/glad.h>
#include <iostream>
#include FT_FREETYPE_H
#include <assert.h>
#include <glm/glm.hpp>

void Font::init(FT_Library *_ft_lib, const std::string &path, const std::string &name) {
  _name = name;

  FT_Face face;
  if (FT_New_Face(*_ft_lib, path.c_str(), 0, &face)) {
    std::cout << "Font::add_font() failed to load face" << std::endl;
    assert(0);
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (unsigned char c = 0; c < 128; c++) {

    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 face->glyph->bitmap.width,
                 face->glyph->bitmap.rows,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {texture,
                           glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                           glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                           static_cast<unsigned int>(face->glyph->advance.x)};
    _characters.insert(std::pair<char, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(*_ft_lib);
}

std::map<char, Character> &Font::get_characters() {
  return _characters;
}

std::string &Font::get_name() {
  return _name;
}
