#pragma once
#include "Enums.h"
#include "Mesh2D.h"
#include "Types.h"
#include <string>

namespace UIBackend {
void init();
void update();
void blit_text(const std::string &text,
               const std::string &font_name,
               int                x,
               int                y,
               UIAlignment        alignment,
               float              scale = 1.0f,
               glm::vec4          color = glm::vec4(1.0f));
void submit_render_items();
void end_frame();

Mesh2D                    &get_ui_mesh();
std::vector<UIRenderItem> &get_render_items();

} // namespace UIBackend
