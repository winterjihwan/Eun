#pragma once
#include "Enums.h"
#include "FontSpriteSheet.h"
#include "Types.h"
#include <string>

namespace TextBlitter {
void             add_font(const FontSpriteSheet &font);
Mesh2DData       blit_text(const std::string &text,
                           const std::string &fontName,
                           int                originX,
                           int                originY,
                           glm::ivec2         viewportSize,
                           UIAlignment        alignment,
                           float              scale,
                           uint32_t           baseVertex);
FontSpriteSheet *get_font_sprite_sheet(const std::string &name);
} // namespace TextBlitter
