#include "Texture.h"
#include <assert.h>
#include <iostream>
#include <stb_image/stb_image.h>

Texture::Texture(GLenum texture_target, const std::string &file_name, const std::string &name) {
  _texture_target = texture_target;
  _file_name      = file_name;
  _name           = name;
}

void Texture::load(uint32_t buffer_size, void *image_data) {
  void *stbi_data = stbi_load_from_memory(
      (const stbi_uc *)image_data, buffer_size, &_image_width, &_image_height, &_image_bpp, 0);

  load_internal(stbi_data);
  stbi_image_free(stbi_data);
}

bool Texture::load() {
  unsigned char *stbi_data = nullptr;

  stbi_set_flip_vertically_on_load(0);
  stbi_data = stbi_load(_file_name.c_str(), &_image_width, &_image_height, &_image_bpp, 0);

  if (!stbi_data) {
    std::cout << "Texture::load() error, file: " << _file_name
              << " , err: " << stbi_failure_reason() << std::endl;
    // exit(0);
  }

  std::cout << "Width " << _image_width << ", Height " << _image_height << ", bpp " << _image_bpp
            << std::endl;

  load_internal(stbi_data);
  return true;
}

void Texture::load(const std::string &filename) {
  _file_name = filename;

  if (!load()) {
    exit(0);
  }
}

void Texture::load_raw(int width, int height, int bpp, const unsigned char *image_data) {
  _image_width  = width;
  _image_height = height;
  _image_bpp    = bpp;

  load_internal(image_data);
}

int is_gl_version_higher(int major_ver, int minor_ver) {
  return false;
}

void Texture::load_internal(const void *image_data) {
  glGenTextures(1, &_texture_obj);
  glBindTexture(_texture_target, _texture_obj);

  if (_texture_target == GL_TEXTURE_2D) {
    switch (_image_bpp) {
    case 1: {
      glTexImage2D(_texture_target,
                   0,
                   GL_RED,
                   _image_width,
                   _image_height,
                   0,
                   GL_RED,
                   GL_UNSIGNED_BYTE,
                   image_data);
      GLint swizzle[] = {GL_RED, GL_RED, GL_RED, GL_RED};
      glTexParameteriv(_texture_target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    } break;

    case 2:
      glTexImage2D(_texture_target,
                   0,
                   GL_RG,
                   _image_width,
                   _image_height,
                   0,
                   GL_RG,
                   GL_UNSIGNED_BYTE,
                   image_data);
      break;

    case 3:
      glTexImage2D(_texture_target,
                   0,
                   GL_RGB,
                   _image_width,
                   _image_height,
                   0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE,
                   image_data);
      break;

    case 4:
      glTexImage2D(_texture_target,
                   0,
                   GL_RGBA,
                   _image_width,
                   _image_height,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   image_data);
      break;

    default:;
      // assert(0);
    }
  } else {
    std::cout << "Texture::load_internal() error, " << std::hex << _texture_target
              << " is not implemented" << std::endl;
    exit(1);
  }

  glTexParameteri(_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(_texture_target, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(_texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(_texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenerateMipmap(_texture_target);
  glBindTexture(_texture_target, 0);
}

void Texture::bind(GLenum texture_unit) {
  glActiveTexture(texture_unit);
  glBindTexture(_texture_target, _texture_obj);
}
