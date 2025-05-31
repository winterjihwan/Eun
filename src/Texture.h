#pragma once

#include "Camera/Camera.h"
#include "Enums.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image/stb_image.h>

struct Texture {
  std::string name;
  std::string path;
  TextureType type;

  int width        = 0;
  int height       = 0;
  int nrComponents = 0;

  // TODO: Can do better than this
  std::shared_ptr<std::vector<unsigned char>> pixel_data =
      std::make_shared<std::vector<unsigned char>>();

  Texture() = default;

  Texture(std::string name, std::string path, TextureType type) {
    this->name = name;
    this->path = path;
    this->type = type;

    load_texture(this->path);
  }

  void load_texture(const std::string &path) {
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
      size_t size = width * height * nrComponents;
      pixel_data->assign(data, data + size);
      stbi_image_free(data);
    } else {
      std::cout << "Texture::load_texture failed, with path: " << path << std::endl;
    }
  }

  void upload_to_gpu() {
    if (_texture_id != 0 || pixel_data->empty())
      return;

    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    GLenum format = GL_RGBA;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;

    glTexImage2D(
        GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data->data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    pixel_data->clear();
  }

  unsigned int get_handle() {
    return _texture_id;
  }

private:
  unsigned int _texture_id = 0;
};
