#include "Backend.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Backend/GLFW.h"
#include "Core/Debug.h"
#include "Core/Game.h"
#include "HotReload/HotReload.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "Renderer/RenderDataManager.h"
#include "Renderer/Renderer.h"
#include "UI/UIBackend.h"
#include "Weapon/WeaponManager.h"
#include "World/World.h"

namespace Backend {
bool init() {
  if (!GLFW::init()) {
    return false;
  }

  Renderer::init();
  HotReload::init();
  Physics::init();
  AssetManager::init();
  UIBackend::init();
  Audio::init();
  Input::init(get_window_pointer());
  WeaponManager::init();

  return true;
}

void begin_frame() {
  GLFW::begin_frame();
  RenderDataManager::begin_frame();
  Game::begin_frame();
}

void update_game() {
  float delta_time = Game::get_delta_time();

  // HACK
  if (Input::key_pressed(EUN_KEY_0)) {
    HotReload::hot_reload();
  }

  Game::update();
  Physics::update(delta_time);

  World::submit_render_items();

  Debug::update();
  UIBackend::update();
  RenderDataManager::update();
}

void update_subsystems() {
  Input::update();
}

void end_frame() {
  UIBackend::end_frame();
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
