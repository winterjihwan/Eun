#pragma once

#include <string>

struct ExrTexture {
  std::string name;
  std::string path;

  ExrTexture(const std::string &path, const std::string &name) {
    this->path = path;
    this->name = name;

    load(path);
  };

  void load(const std::string &path);
  bool get_exr_layers(std::string &file_name);
  bool load_exr_rgba(float **rgba, int *w, int *h, const char *filename, const char *layername);

  unsigned int &get_handle();

  std::string _filename;
  std::string _filetype;

private:
  unsigned int _texture_id;
};
