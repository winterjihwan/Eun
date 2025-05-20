#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

struct OpenGLCubemapView {
  // OpenGLCubemapView() = default;
  OpenGLCubemapView();

  void   create_cubemap();
  GLuint get_handle() const;

private:
  GLuint _handle;
};
