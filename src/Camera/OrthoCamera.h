#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrthoCamera {
public:
  OrthoCamera(float left, float right, float bottom, float top);

  void set_position(const glm::vec3 &position);
  void move(const glm::vec3 &delta);

  glm::mat4 view() const;
  glm::mat4 projection() const;
  glm::vec3 get_position() const;

private:
  glm::vec3 _position = glm::vec3(0.0f, 10.0f, 0.0f);
  glm::vec3 _target   = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 _up       = glm::vec3(0.0f, 0.0f, -1.0f);

  float _zoom_level   = 10.0f;
  float _aspect_ratio = 1.777f; // 16 : 9
  float _near         = -100.0f;
  float _far          = 100.0f;
  float _view_width   = 20.0f;
};
