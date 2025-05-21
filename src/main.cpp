#include "Backend/Backend.h"
#include "Core/Game.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Keycodes.h"
#include "stb_image/stb_image.h"
#include <iostream>

int main(void) {
  std::cout << "Eunnnnnnnnni" << std::endl;

  if (!Backend::init()) {
    std::cout << "Backend::init() failed" << std::endl;
    return -1;
  }

  // HACK
  Game::init();

  while (Backend::is_window_open()) {
    Backend::update_subsystems();
    Backend::begin_frame();
    // TODO: Loading

    {
      Backend::update_game();
      Renderer::render_game();
    }
    Backend::end_frame();

    // HACK
    if (Input::key_pressed(EUN_KEY_ESCAPE)) {
      return 0;
    }
  }

  return 0;
}
