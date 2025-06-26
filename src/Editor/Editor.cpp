#include "Editor.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Backend/GLFW.h"
#include "Core/Game.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "Types/Renderer/Mesh.h"

namespace Editor {
using glm::mat4;

Mesh *_editor_mesh;
bool  _is_open = false;

void init() {
}

void update(float delta_time) {
  if (Input::key_pressed(EUN_KEY_TAB)) {
    Audio::play_audio("UI_Select.wav", 1.0f);
    toggle_editor_open_state();
  }

  if (!is_open()) {
    return;
  }

  update_object_hover();
}

void update_object_hover() {
  if (Input::left_mouse_pressed()) {
    // Cursor
    double mouseX, mouseY;
    glfwGetCursorPos(static_cast<GLFWwindow *>(GLFW::get_window_pointer()), &mouseX, &mouseY);

    // NDC
    float     x        = (2.0f * static_cast<float>(mouseX)) / VIEWPORT_WIDTH - 1.0f;
    float     y        = 1.0f - (2.0f * static_cast<float>(mouseY)) / VIEWPORT_HEIGHT;
    glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

    // Inverse Projection / View
    Camera   *camera     = Game::get_camera();
    mat4      projection = glm::perspective(glm::radians(camera->get_zoom()),
                                       (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT,
                                       NEAR,
                                       FAR);
    glm::mat4 view       = camera->view_matrix();

    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye.z         = -1.0f;
    ray_eye.w         = 0.0f;

    glm::vec3 ray_dir_world = glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));
    glm::vec3 ray_origin    = Game::get_camera()->get_pos();

    // Raycast
    auto hit =
        Physics::raycast(Util::to_jolt_vec3(ray_origin), Util::to_jolt_vec3(ray_dir_world), 100.0f);

    if (hit.has_value()) {
      const RayHitInfo &info        = hit.value();
      ObjectType        object_type = info.user_data->object_type;
      if (object_type == ObjectType::GAME_OBJECT) {
        std::cout << "Clicked object" << '\n';
      }

      if (object_type == ObjectType::MAP) {
        std::cout << "Clicked map" << '\n';
      }
    }
  }
}

void open_editor() {
  Audio::play_audio("UI_Select.wav", 1.0f);
  Input::show_cursor();
  Input::center_mouse_cursor();

  _is_open = true;
}

void close_editor() {
  Audio::play_audio("UI_Select.wav", 1.0f);
  Input::disable_cursor();

  _is_open = false;
}

void toggle_editor_open_state() {
  _is_open = !_is_open;
  if (_is_open) {
    open_editor();
  } else {
    close_editor();
  }
}

bool is_open() {
  return _is_open;
}
} // namespace Editor
