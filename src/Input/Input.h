#pragma once

#include <glad/glad.h>

namespace Input {
void  init(void *glfwWinodw);
void  update();
bool  key_pressed(unsigned int keycode);
bool  key_down(unsigned int keycode);
float get_mouse_offset_x();
float get_mouse_offset_y();
bool  left_mouse_down();
bool  right_mouse_down();
bool  middle_mouse_down();
bool  left_mouse_pressed();
bool  middle_mouse_pressed();
bool  right_mouse_pressed();
bool  mouse_wheel_up();
bool  mouse_wheel_down();
int   get_mouse_wheel_value();
int   get_mouse_x();
int   get_mouse_y();
void  prevent_right_mouse_hold();
void  disable_cursor();
void  hide_cursor();
void  show_cursor();
void  center_mouse_cursor();
int   get_cursor_screen_x();
int   get_cursor_screen_y();
void  set_cursor_position(int x, int y);
int   get_mouse_x_previous_frame();
int   get_mouse_y_previous_frame();
} // namespace Input
