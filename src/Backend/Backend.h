#pragma once

#include "Types.h"
#include <glm/glm.hpp>

namespace Backend {
bool init();
void begin_frame();
void update_game();
void update_subsystems();
void end_frame();
void clean_up();

void    *get_window_pointer();
Viewport get_viewport();
bool     is_window_open();
} // namespace Backend
