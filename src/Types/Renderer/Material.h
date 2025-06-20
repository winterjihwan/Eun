#pragma once

#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Material {
public:
  std::string m_name;

  glm::vec4 ambient_color  = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
  glm::vec4 diffuse_color  = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
  glm::vec4 specular_color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

  Texture *diffuse           = 0;
  Texture *normal            = 0;
  Texture *specular_exponent = 0;

  float transparency_factor = 1.0f;
  float alpha_test          = 0.0f;

  ~Material() {
    if (diffuse) {
      delete diffuse;
    }

    if (specular_exponent) {
      delete specular_exponent;
    }
  }
};
