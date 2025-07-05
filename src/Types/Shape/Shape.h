#pragma once
#include <variant>

struct Cube {
  float w, h, d;

  Cube() = default;
  Cube(float w, float h, float d) {
    this->w = w;
    this->h = h;
    this->d = d;
  }
};

struct Capsule {
  float r, h;

  Capsule() = default;
  Capsule(float r, float h) {
    this->r = r;
    this->h = h;
  }
};

using Shape = std::variant<Cube, Capsule>;
