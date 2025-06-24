#pragma once

#include <glad/glad.h>
#include <string>

class BaseTexture {
protected:
  BaseTexture() {
  }

  GLenum _texture_target = 0;
  GLuint _texture_obj    = 0;
};

class Texture : public BaseTexture {
public:
  Texture(GLenum texture_target, const std::string &file_name, const std::string &name);

  bool load();
  void load(const std::string &filename);
  void load(uint32_t buffer_size, void *image_data);
  void load_raw(int width, int height, int bpp, const unsigned char *image_data);
  void bind(GLenum texture_unit);

  void get_image_size(int &image_width, int &image_height) {
    image_width  = _image_width;
    image_height = _image_height;
  }
  const std::string &get_name() {
    return _name;
  }
  GLuint get_texture() const {
    return _texture_obj;
  }

private:
  void load_internal(const void *image_data);

  std::string _file_name;
  std::string _name;
  int         _image_width  = 0;
  int         _image_height = 0;
  int         _image_bpp    = 0;
  GLuint      _texture_obj;
};
