#pragma once
#include "Enums.h"
#include "Mesh2D.h"
#include "Types.h"
#include <string>

enum class TextureFilter { NEAREST, LINEAR, LINEAR_MIPMAP, UNDEFINED };

namespace UIBackend {
void                       init();
void                       update();
void                       submit_render_items();
void                       blit_text(const std::string &text,
                                     const std::string &fontName,
                                     int                locationX,
                                     int                locationY,
                                     UIAlignment        alignment,
                                     float              scale);
void                       blit_texture(const std::string &textureName,
                                        glm::ivec2         location,
                                        UIAlignment        alignment,
                                        glm::vec4          colorTint = glm::vec4(1, 1, 1, 1),
                                        glm::ivec2         size      = glm::ivec2(-1, -1));
void                       end_frame();
Mesh2D                    &get_ui_mesh();
std::vector<UIRenderItem> &get_render_items();

} // namespace UIBackend
