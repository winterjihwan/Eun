#include "Editor.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Backend/GLFW.h"
#include "Core/Game.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "World/World.h"

namespace Editor {
bool       _is_open              = false;
ObjectType _hovered_object_type  = ObjectType::NONE;
ObjectType _selected_object_type = ObjectType::NONE;
uint64_t   _hovered_object_id    = 0;
uint64_t   _selected_object_id   = 0;

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

  update_hover();
  update_selection();
  update_translation();
}

void update_hover() {
  // Cursor
  double mouseX, mouseY;
  glfwGetCursorPos(static_cast<GLFWwindow *>(GLFW::get_window_pointer()), &mouseX, &mouseY);

  // NDC
  float     x        = (2.0f * static_cast<float>(mouseX)) / VIEWPORT_WIDTH - 1.0f;
  float     y        = 1.0f - (2.0f * static_cast<float>(mouseY)) / VIEWPORT_HEIGHT;
  glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

  // Inverse Projection / View
  Camera   *camera     = Game::get_camera();
  glm::mat4 projection = camera->projection();
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
    const RayHitInfo &info = hit.value();
    _hovered_object_type   = info.user_data->object_type;
    _hovered_object_id     = info.user_data->object_id;
  }
}

void update_selection() {
  if (Input::left_mouse_pressed()) {
    Audio::play_audio("UI_Select.wav", 1.0f);
    _selected_object_id   = _hovered_object_id;
    _selected_object_type = _hovered_object_type;

    std::cout << "Selected: " << Util::to_string(_selected_object_type) << " "
              << _selected_object_id << std::endl;
  }
}

void update_translation() {
  if (_selected_object_id == 0) {
    return;
  }

  Entity *entity = World::get_entity_by_object_id(_selected_object_id);

  if (Input::key_pressed(EUN_KEY_UP)) {
    entity->add_translation(glm::vec3(0.0f, 0.05f, 0.0f));
  } else if (Input::key_pressed(EUN_KEY_DOWN)) {
    entity->add_translation(glm::vec3(0.0f, -0.05f, 0.0f));
  } else if (Input::key_pressed(EUN_KEY_LEFT)) {
    entity->add_translation(glm::vec3(-0.05f, 0.0f, 0.0f));
  } else if (Input::key_pressed(EUN_KEY_RIGHT)) {
    entity->add_translation(glm::vec3(0.05f, 0.0f, 0.0f));
  } else if (Input::key_pressed(EUN_KEY_LEFT_BRACKET)) {
    entity->add_translation(glm::vec3(0.0f, 0.0f, 0.05f));
  } else if (Input::key_pressed(EUN_KEY_RIGHT_BRACKET)) {
    entity->add_translation(glm::vec3(0.0f, 0.0f, -0.05f));
  }

  entity->log_transform();
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

  _hovered_object_type  = ObjectType::NONE;
  _selected_object_type = ObjectType::NONE;
  _hovered_object_id    = 0;
  _selected_object_id   = 0;

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
