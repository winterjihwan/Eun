#pragma once

#include <glm/glm.hpp>

namespace RTS {
void update(float dt);
void submit_render_items();

glm::mat4 projection();
glm::mat4 view();
glm::vec3 get_camera_position();

void update_hover();
void update_selection();
void update_move();

void open_rts();
void close_rts();
bool is_open();
} // namespace RTS
