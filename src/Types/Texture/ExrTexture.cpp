#include "ExrTexture.h"

#include <assert.h>
#include <glad/glad.h>
#include <iostream>
#include <tinyexr/tinyexr.hpp>

void ExrTexture::load(const std::string &file_path) {
  const char *filename = file_path.c_str();

  // Load RGBA
  int         width, height;
  float      *out;
  const char *err = 0;
  int         ret = LoadEXR(&out, &width, &height, filename, &err);

  if (!out) {
    std::cerr << "ExrTexture::load, no image: " << file_path << std::endl;
    return;
  }

  if (err) {
    std::cout << "ExrTexture::load, just error: " << err << std::endl;
    FreeEXRErrorMessage(err);
    return;
  }

  if (ret != TINYEXR_SUCCESS) {
    std::cout << "ExrTexture::load, no success: " << err << std::endl;
  }

  // Load to GL
  glGenTextures(1, &_texture_id);
  glBindTexture(GL_TEXTURE_2D, _texture_id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, width, height, 0, GL_RGBA, GL_FLOAT, out);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "ExrTexture::load, gl error: " << err << std::endl;
  }

  free(out);
}

unsigned int &ExrTexture::get_handle() {
  return _texture_id;
}
