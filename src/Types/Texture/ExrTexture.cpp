#include "ExrTexture.h"

#include <assert.h>
#include <glad/glad.h>
#include <iostream>
#include <tinyexr/tinyexr.hpp>

void ExrTexture::load(const std::string &file_path) {
  const char *filename = file_path.c_str();

  // EXR Layers
  const char  *err         = nullptr;
  const char **layer_names = nullptr;
  const char  *layername   = nullptr;
  int          num_layers  = 0;
  bool         status      = EXRLayers(filename, &layer_names, &num_layers, &err);

  if (err) {
    std::cout << "ExrTexture::load, just error: " << err << std::endl;
  }

  if (status != 0) {
    std::cout << "ExrTexture::load, invalid status err: " << err << std::endl;
  }

  if (num_layers > 0) {
    std::cout << "EXR has layers of count " << num_layers << std::endl;
    for (int i = 0; i < (int)num_layers; ++i) {
      std::cout << "\tLayer " << i << ": " << layer_names[i] << std::endl;
    }
  }

  free(layer_names);

  // Load RGBA
  int    width, height;
  float *image;
  status = LoadEXRWithLayer(&image, &width, &height, filename, layername, &err);

  if (!image) {
    std::cerr << "ExrTexture::load, no image: " << file_path << std::endl;
    return;
  }

  if (err) {
    std::cout << "ExrTexture::load, just error: " << err << std::endl;
  }

  if (status != 0) {
    std::cout << "ExrTexture::load, invalid status err: " << err << std::endl;
  }

  // Load to GL
  glGenTextures(1, &_texture_id);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, width, height, 0, GL_RGBA, GL_FLOAT, image);
}

unsigned int &ExrTexture::get_handle() {
  return _texture_id;
}
