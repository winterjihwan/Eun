#include "Backend.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Backend/GLFW.h"
#include "Core/Game.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "Weapon/WeaponManager.h"

namespace Backend {
bool init() {
  if (!GLFW::init()) {
    return false;
  }

  Physics::init();
  AssetManager::init();
  Audio::init();
  Input::init(get_window_pointer());
  WeaponManager::init();

  return true;
}

void begin_frame() {
  GLFW::begin_frame();
  Game::begin_frame();
}

void update_game() {
  float delta_time = Game::get_delta_time();

  Game::update();
  Physics::update(delta_time);
}

void update_subsystems() {
  Input::update();
}

void end_frame() {
  GLFW::end_frame();
}

void clean_up() {
  AssetManager::shutdown();
  Physics::shutdown();

  GLFW::shutdown();
}

void *get_window_pointer() {
  return GLFW::get_window_pointer();
}

bool is_window_open() {
  return GLFW::is_window_open();
}

void *get_window_pointer();
bool  is_window_open();

} // namespace Backend
