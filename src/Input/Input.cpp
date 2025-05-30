#include "Input.h"
#include "Defines.h"

#include <GLFW/glfw3.h>

namespace Input {
bool        g_keyPressed[372];
bool        g_keyDown[372];
bool        g_keyDownLastFrame[372];
double      g_mouseX                             = 0;
double      g_mouseY                             = 0;
double      g_mouseOffsetX                       = 0;
double      g_mouseOffsetY                       = 0;
int         g_mouseWheelValue                    = 0;
int         g_sensitivity                        = 100;
bool        g_cursorVisible                      = false;
bool        g_mouseWheelUp                       = false;
bool        g_mouseWheelDown                     = false;
bool        g_leftMouseDown                      = false;
bool        g_rightMouseDown                     = false;
bool        g_leftMousePressed                   = false;
bool        g_rightMousePressed                  = false;
bool        g_leftMouseDownLastFrame             = false;
bool        g_rightMouseDownLastFrame            = false;
bool        g_middleMouseDown                    = false;
bool        g_middleMousePressed                 = false;
bool        g_middleMouseDownLastFrame           = false;
bool        g_preventRightMouseHoldTillNextClick = false;
int         g_mouseScreenX                       = 0;
int         g_mouseScreenY                       = 0;
int         g_scrollWheelYOffset                 = 0;
int         g_mouseXPreviousFrame                = 0;
int         g_mouseYPreviousFrame                = 0;
GLFWwindow *g_window;

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void init(void *glfwWindow) {
  double x, y;
  g_window = static_cast<GLFWwindow *>(glfwWindow);
  glfwSetScrollCallback(g_window, mouse_scroll_callback);
  glfwGetCursorPos(g_window, &x, &y);
  disable_cursor();
  g_mouseOffsetX = x;
  g_mouseOffsetY = y;
  g_mouseX       = x;
  g_mouseY       = y;
}

void update() {
  // Wheel
  g_mouseWheelUp    = false;
  g_mouseWheelDown  = false;
  g_mouseWheelValue = g_scrollWheelYOffset;
  if (g_mouseWheelValue < 0)
    g_mouseWheelDown = true;
  if (g_mouseWheelValue > 0)
    g_mouseWheelUp = true;
  g_scrollWheelYOffset = 0;

  // Keyboard
  for (int i = 32; i < 349; i++) {
    // down
    if (glfwGetKey(g_window, i) == GLFW_PRESS)
      g_keyDown[i] = true;
    else
      g_keyDown[i] = false;

    // press
    if (g_keyDown[i] && !g_keyDownLastFrame[i])
      g_keyPressed[i] = true;
    else
      g_keyPressed[i] = false;
    g_keyDownLastFrame[i] = g_keyDown[i];
  }

  // Mouse
  g_mouseXPreviousFrame = g_mouseX;
  g_mouseYPreviousFrame = g_mouseY;
  double x, y;
  glfwGetCursorPos(g_window, &x, &y);
  g_mouseOffsetX  = x - g_mouseX;
  g_mouseOffsetY  = y - g_mouseY;
  g_mouseX        = x;
  g_mouseY        = y;
  g_cursorVisible = glfwGetInputMode(g_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;

  // Left mouse down/pressed
  g_leftMouseDown = glfwGetMouseButton(g_window, GLFW_MOUSE_BUTTON_LEFT);
  if (g_leftMouseDown == GLFW_PRESS && !g_leftMouseDownLastFrame)
    g_leftMousePressed = true;
  else
    g_leftMousePressed = false;
  g_leftMouseDownLastFrame = g_leftMouseDown;

  // Right mouse down/pressed
  g_rightMouseDown = glfwGetMouseButton(g_window, GLFW_MOUSE_BUTTON_RIGHT);
  if (g_rightMouseDown == GLFW_PRESS && !g_rightMouseDownLastFrame)
    g_rightMousePressed = true;
  else
    g_rightMousePressed = false;
  g_rightMouseDownLastFrame = g_rightMouseDown;

  if (g_rightMousePressed)
    g_preventRightMouseHoldTillNextClick = false;

  // Middle button
  g_middleMouseDown = glfwGetMouseButton(g_window, GLFW_MOUSE_BUTTON_MIDDLE);
  if (g_middleMouseDown == GLFW_PRESS && !g_middleMouseDownLastFrame)
    g_middleMousePressed = true;
  else
    g_middleMousePressed = false;
  g_middleMouseDownLastFrame = g_middleMouseDown;
}

bool key_pressed(unsigned int keycode) {
  return g_keyPressed[keycode];
}

bool key_down(unsigned int keycode) {
  return g_keyDown[keycode];
}

float get_mouse_offset_x() {
  return (float)g_mouseOffsetX;
}

float get_mouse_offset_y() {
  return (float)g_mouseOffsetY;
}

bool left_mouse_down() {
  return g_leftMouseDown;
}

bool middle_mouse_down() {
  return g_middleMouseDown;
}

bool right_mouse_down() {
  return g_rightMouseDown && !g_preventRightMouseHoldTillNextClick;
}

bool left_mouse_pressed() {
  return g_leftMousePressed;
}

bool middle_mouse_pressed() {
  return g_middleMousePressed;
}

bool right_mouse_pressed() {
  return g_rightMousePressed;
}

bool mouse_wheel_down() {
  return g_mouseWheelDown;
}

int get_mouse_wheel_value() {
  return g_mouseWheelValue;
}

bool mouse_wheel_up() {
  return g_mouseWheelUp;
}

void prevent_right_mouse_hold() {
  g_preventRightMouseHoldTillNextClick = true;
}

int get_mouse_x() {
  return (int)g_mouseX;
}

int get_mouse_y() {
  return (int)g_mouseY;
}

int get_mouse_x_previous_frame() {
  return (int)g_mouseXPreviousFrame;
}

int get_mouse_y_previous_frame() {
  return (int)g_mouseYPreviousFrame;
}

void disable_cursor() {
  glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void hide_cursor() {
  glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void show_cursor() {
  glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool cursor_visible() {
  return g_cursorVisible;
}

void center_mouse_cursor() {
  set_cursor_position(VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2);
}

int get_cursor_screen_x() {
  return g_mouseScreenX;
}

int get_cursor_screen_y() {
  return g_mouseScreenY;
}

void set_cursor_position(int x, int y) {
  glfwSetCursorPos(g_window, static_cast<double>(x), static_cast<double>(y));
  g_mouseX              = x;
  g_mouseY              = y;
  g_mouseOffsetX        = 0;
  g_mouseOffsetY        = 0;
  g_mouseXPreviousFrame = x;
  g_mouseYPreviousFrame = y;
}

void mouse_scroll_callback(GLFWwindow * /*window*/, double /*xoffset*/, double yoffset) {
  g_scrollWheelYOffset = (int)yoffset;
}
} // namespace Input
