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

  Texture() = default;

  Texture(std::string name, std::string path, TextureType type) {
    this->name = name;
    this->path = path;
    this->type = type;

    load_texture(path);
  }

  void load_texture(std::string &path) {
    glGenTextures(1, &_texture_id);

    unsigned char *data = stbi_load(path.data(), &width, &height, &nrComponents, 0);
    if (data) {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
        format = GL_RGB;
      else
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, _texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
    }
  }

  unsigned int get_handle() {
    return _texture_id;
  }

private:
  unsigned int _texture_id;
};
