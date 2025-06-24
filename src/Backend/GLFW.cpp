#include "GLFW.h"
#include "Backend/Backend.h"
#include "Defines.h"
#include <iostream>

namespace GLFW {
GLFWwindow *_window = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

bool init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  _window = glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "Eun", NULL, NULL);
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(_window);
  glfwSwapInterval(0);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  Viewport viewport = Backend::get_viewport();

  glfwGetFramebufferSize(_window, viewport.width, viewport.height);
  glViewport(0, 0, *viewport.width, *viewport.height);
  glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return true;
}

void begin_frame() {
}

void end_frame() {
  glfwSwapBuffers(_window);
  glfwPollEvents();
}

void register_mouse_callback(PFN_InputCallback callback) {
  glfwSetCursorPosCallback(_window, callback);
}

void register_scroll_callback(PFN_InputCallback callback) {
  glfwSetScrollCallback(_window, callback);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void shutdown() {
  glfwTerminate();
}

void *get_window_pointer() {
  return _window;
}

bool is_window_open() {
  return !(glfwWindowShouldClose(_window));
}

} // namespace GLFW
