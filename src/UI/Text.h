#pragma once
#include "Enums.h"
#include "Font.h"
#include <string>

namespace Text {
void init();
void add_font(const std::string &path, const std::string &name);

std::vector<Mesh2DData> blit_text(const std::string &text,
                                  const std::string &font_name,
                                  int                x,
                                  int                y,
                                  glm::ivec2         viewport_size,
                                  UIAlignment        alignment,
                                  float              scale,
                                  glm::vec4          color);
Font                   *get_font_by_name(const std::string &name);
} // namespace Text
