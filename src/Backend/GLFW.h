#pragma once

#include "glad/glad.h"

#include <GLFW/glfw3.h>

typedef void (*PFN_InputCallback)(GLFWwindow *, double, double);

namespace GLFW {
bool init();
void begin_frame();
void end_frame();

void register_mouse_callback(PFN_InputCallback callback);
void register_scroll_callback(PFN_InputCallback callback);

void shutdown();

void *get_window_pointer();
bool  is_window_open();
} // namespace GLFW
