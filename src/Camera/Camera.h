#pragma once

#include "Defines.h"
#include "Enums.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
         float     yaw      = YAW,
         float     pitch    = PITCH) {
    _position = position;
    _world_up = up;
    _yaw      = yaw;
    _pitch    = pitch;
    update_camera_vectors();
  }

  Camera(
      float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    _position = glm::vec3(posX, posY, posZ);
    _world_up = glm::vec3(upX, upY, upZ);
    _yaw      = yaw;
    _pitch    = pitch;
    update_camera_vectors();
  }

  void update(const glm::vec3 &watch);
  void toggle_view(const glm::vec3 &watch);
  void first_person_view(const glm::vec3 &watch);
  void third_person_view(const glm::vec3 &watch);

  glm::mat4 view_matrix();
  glm::mat4 projection();

  void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true);
  void process_mouse_scroll(float yoffset);

  glm::vec3 get_pos();
  glm::vec3 get_front();
  glm::vec3 get_right();
  glm::vec3 get_up();
  float     get_yaw();
  float     get_zoom();

private:
  glm::vec3 _position;
  glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 _right;
  glm::vec3 _world_up;
  glm::vec3 _up;

  float _yaw;
  float _pitch;

  float _mouse_sensitivity = SENSITIVITY;
  float _zoom              = ZOOM;

  ViewMode  _view_mode = ViewMode::FIRST;
  glm::mat4 _view;

  void update_camera_vectors();
};
