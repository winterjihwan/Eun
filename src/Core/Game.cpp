#include "Game.h"
// #include "Audio/Audio.h"
#include "Backend/GLFW.h"
#include "World/World.h"
#include <GLFW/glfw3.h>

namespace Game {
Player _player;
float  _delta_time = 0.0f;
float  _last_frame = 0.0f;

// TODO: Move camera out of here
Camera _camera;
float  _lastX       = VIEWPORT_WIDTH / 2.0f;
float  _lastY       = VIEWPORT_HEIGHT / 2.0f;
bool   _first_mouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void init() {
  _player.init(PLAYER_SPAWN_POS);
  _camera = Camera(_player.get_pos());

  GLFW::register_mouse_callback(mouse_callback);
  GLFW::register_scroll_callback(scroll_callback);

  // Audio::loop_audio("Dark_ambient.mp3", 0.5f);
}

void update() {
  float currentFrame = static_cast<float>(glfwGetTime());
  _delta_time        = currentFrame - _last_frame;
  _last_frame        = currentFrame;

  _player.update(_delta_time, _camera);
  _camera.update(_player.get_pos());

  World::update(_delta_time);
}

void begin_frame() {
  // TODO: player begin frame
}

float get_delta_time() {
  return _delta_time;
}

Player *get_player() {
  return &_player;
}

Camera *get_camera() {
  return &_camera;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (_first_mouse) {
    _lastX       = xpos;
    _lastY       = ypos;
    _first_mouse = false;
  }

  float xoffset = xpos - _lastX;
  float yoffset = _lastY - ypos;

  _lastX = xpos;
  _lastY = ypos;

  _camera.process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  _camera.process_mouse_scroll(static_cast<float>(yoffset));
}
} // namespace Game
