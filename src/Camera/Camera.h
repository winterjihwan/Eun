#pragma once

#include "Defines.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
         float     yaw      = YAW,
         float     pitch    = PITCH)
      : _front(glm::vec3(0.0f, 0.0f, -1.0f)), _mouse_sensitivity(SENSITIVITY), _zoom(ZOOM) {
    _position = position;
    _world_up = up;
    _yaw      = yaw;
    _pitch    = pitch;
    update_camera_vectors();
  }

  Camera(
      float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
      : _front(glm::vec3(0.0f, 0.0f, -1.0f)), _mouse_sensitivity(SENSITIVITY), _zoom(ZOOM) {
    _position = glm::vec3(posX, posY, posZ);
    _world_up = glm::vec3(upX, upY, upZ);
    _yaw      = yaw;
    _pitch    = pitch;
    update_camera_vectors();
  }

  void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true);
  void process_mouse_scroll(float yoffset);

  glm::vec3 get_front();
  glm::vec3 get_right();
  glm::vec3 get_up();
  float     get_zoom();

private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _right;
  glm::vec3 _world_up;
  glm::vec3 _up;

  float _yaw;
  float _pitch;

  float _mouse_sensitivity;
  float _zoom;

  void update_camera_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front  = glm::normalize(front);
    _right  = glm::normalize(glm::cross(_front, _world_up));
    _up     = glm::normalize(glm::cross(_right, _front));
  }
};
