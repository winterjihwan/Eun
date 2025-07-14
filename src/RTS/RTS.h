#pragma once

#include <glm/glm.hpp>

namespace RTS {
void update(float dt);

glm::mat4 projection();
glm::mat4 view();
glm::vec3 get_camera_position();

void update_hover();
void update_selection();

void open_rts();
void close_rts();
bool is_open();
} // namespace RTS
