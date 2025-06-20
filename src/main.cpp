#include "AssetManager/AssetManager.h"
#include "Backend/Backend.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Renderer/Renderer.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>
#define TINYEXR_IMPLEMENTATION
#include <zlib.h>
#define TINYEXR_USE_MINIZ 0
#include <tinyexr/tinyexr.hpp>

int main(void) {
  std::cout << "Eunnnnnnnnni" << std::endl;

  if (!Backend::init()) {
    std::cout << "Backend::init() failed" << std::endl;
    return -1;
  }

  // TODO: UI FPS
  while (Backend::is_window_open()) {
    Backend::update_subsystems();
    Backend::begin_frame();

    if (!AssetManager::loading_complete()) {
      AssetManager::update_loading();
      Renderer::render_loading_screen();
    }

    else {
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
