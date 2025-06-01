#include "ExrTexture.h"

#include <assert.h>
#include <glad/glad.h>
#include <iostream>
#include <tinyexr/tinyexr.hpp>

void ExrTexture::load(const std::string &file_path) {
  _path                = file_path;
  const char *filename = file_path.c_str();
  const char *err      = nullptr;

  float *out = 0;
  int    ret = LoadEXR(&out, &_width, &_height, filename, &err);

  if (ret != TINYEXR_SUCCESS || !out) {
    std::cerr << "ExrTexture::load_cpu failed: " << err << std::endl;
    if (err) {
      FreeEXRErrorMessage(err);
    }
    assert(0);
  }

  _pixel_data.reset(out);
}

std::future<void> ExrTexture::load_async(const std::string &path) {
  return std::async(std::launch::async, [this, path]() { this->load(path); });
}

void ExrTexture::upload_to_gpu() {
  if (_texture_id != 0) {
    std::cerr << "ExrTexture::upload_to_gpu, already has texture id" << std::endl;
    assert(0);
  }

  if (!_pixel_data) {
    std::cerr << "ExrTexture::upload_to_gpu, missing pixel data" << std::endl;
    assert(0);
  }

  glGenTextures(1, &_texture_id);
  glBindTexture(GL_TEXTURE_2D, _texture_id);

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB16, _width, _height, 0, GL_RGBA, GL_FLOAT, _pixel_data.get());

  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
    std::cerr << "ExrTexture::upload_to_gpu OpenGL error: " << err << std::endl;

  _pixel_data.reset();
}

unsigned int &ExrTexture::get_handle() {
  return _texture_id;
}

std::string &ExrTexture::get_name() {
  return _name;
}
