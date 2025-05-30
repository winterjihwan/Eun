#pragma once

#include "Types.h"
#include <ft2build.h>
#include <glad/glad.h>
#include FT_FREETYPE_H
#include <assert.h>
#include <glm/glm.hpp>
#include <map>

struct Font {
private:
  std::map<char, Character> _characters;
  std::string               _name;

public:
  void init(FT_Library *_ft_lib, const std::string &path, const std::string &name);

  std::map<char, Character> &get_characters();
  std::string               &get_name();
};
