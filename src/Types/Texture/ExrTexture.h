#pragma once

#include <future>
#include <string>

struct ExrTexture {
  ExrTexture(const std::string &name) {
    _name = name;
  };

  void              load(const std::string &path);
  std::future<void> load_async(const std::string &path);
  void              upload_to_gpu();

  unsigned int &get_handle();
  std::string  &get_name();

private:
  std::string              _name;
  std::string              _path;
  int                      _width  = 0;
  int                      _height = 0;
  std::unique_ptr<float[]> _pixel_data;
  unsigned int             _texture_id = 0;

  bool get_exr_layers(std::string &file_name);
  bool load_exr_rgba(float **rgba, int *w, int *h, const char *filename, const char *layername);
};
