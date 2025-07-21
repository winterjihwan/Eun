#include "RTS.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Backend/GLFW.h"
#include "Camera/OrthoCamera.h"
#include "Defines.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Renderer/RenderDataManager.h"
#include "World/World.h"
#include <optional>

namespace RTS {
bool                     _is_open             = false;
OrthoCamera              _camera              = OrthoCamera(-10.0f, 10.0f, -10.0f, 10.0f);
ObjectType               _hovered_object_type = ObjectType::NONE;
uint64_t                 _hovered_object_id   = 0;
glm::vec2                _hovered_xz          = glm::vec2(0.0f);
std::optional<glm::vec2> _move_target;
ObjectType               _selected_object_type = ObjectType::NONE;
uint64_t                 _selected_object_id   = 0;
std::optional<Unit *>    _unit;

void update(float delta_time) {
  if (!is_open()) {
    return;
  }

  float     move_speed = 10.0f * delta_time;
  glm::vec3 direction(0.0f);

  if (Input::key_down(EUN_KEY_W))
    direction.z -= 1.0f;
  if (Input::key_down(EUN_KEY_S))
    direction.z += 1.0f;
  if (Input::key_down(EUN_KEY_A))
    direction.x -= 1.0f;
  if (Input::key_down(EUN_KEY_D))
    direction.x += 1.0f;

  if (glm::length(direction) > 0.0f)
    direction = glm::normalize(direction);

  _camera.move(direction * move_speed);

  update_hover();
  update_selection();
  update_move();
}

void update_hover() {
  // Cursor
  double mouseX, mouseY;
  glfwGetCursorPos(static_cast<GLFWwindow *>(GLFW::get_window_pointer()), &mouseX, &mouseY);

  // NDC
  float     x   = (2.0f * static_cast<float>(mouseX)) / VIEWPORT_WIDTH - 1.0f;
  float     y   = 1.0f - (2.0f * static_cast<float>(mouseY)) / VIEWPORT_HEIGHT;
  glm::vec4 ndc = glm::vec4(x, y, -1.0f, 1.0f);

  glm::mat4 inv_proj  = glm::inverse(_camera.projection());
  glm::mat4 inv_view  = glm::inverse(_camera.view());
  glm::vec4 world_pos = inv_view * inv_proj * ndc;
  world_pos /= world_pos.w;

  glm::vec3 ray_origin = glm::vec3(world_pos);
  glm::vec3 ray_dir    = glm::vec3(0.0f, -1.0f, 0.0f);

  // Raycast
  std::optional<RayHitInfo> hit = Physics::raycast(ray_origin, ray_dir, 200.0f);

  if (hit.has_value()) {
    const RayHitInfo &info = hit.value();
    _hovered_object_type   = info.user_data->object_type;
    _hovered_object_id     = info.user_data->object_id;

    if (_hovered_object_type == ObjectType::MAP) {
      glm::vec3 p = Util::to_vec3(hit->hit_pos);
      _hovered_xz = glm::vec2(p.x, p.z);
    }
  } else {
    _hovered_object_type = ObjectType::NONE;
    _hovered_object_id   = 0;
  }
}

void update_selection() {
  // Unit Click
  if (Input::left_mouse_pressed()) {
    Audio::play_audio("UI_Select.wav", 1.0f);
    _selected_object_id   = _hovered_object_id;
    _selected_object_type = _hovered_object_type;

    std::cout << "Selected: " << Util::to_string(_selected_object_type) << " "
              << _selected_object_id << std::endl;

    if (_selected_object_type == ObjectType::UNIT && _selected_object_id != 0)
      _unit = World::get_unit_by_object_id(_selected_object_id);
    else
      _unit = std::nullopt;
  }

  // Map Click
  if (_unit && Input::right_mouse_pressed() && _hovered_object_type == ObjectType::MAP) {
    _move_target = _hovered_xz;
  }
}

void update_move() {
  if (!_unit || !_move_target) {
    return;
  }

  glm::vec3 unit_pos  = _unit.value()->get_anim_entity()->get_position();
  glm::vec2 unit_pos2 = glm::vec2(unit_pos.x, unit_pos.z);
  glm::vec2 dir2      = _move_target.value() - unit_pos2;

  if (glm::length(dir2) < 0.1f) {
    _unit.value()->set_velocity(glm::vec3(0.0f));
    _move_target = std::nullopt;
    return;
  }

  dir2          = glm::normalize(dir2);
  glm::vec3 dir = glm::vec3(dir2.x, 0.0f, dir2.y);
  _unit.value()->set_velocity(dir * 5.0f);
}

void submit_render_items() {
  // On Select, Highlight

  if (_selected_object_type == ObjectType::UNIT) {
    Unit *unit = World::get_unit_by_object_id(_selected_object_id);
    RenderDataManager::submit_aabb(unit->get_aabb());
  }

  if (_selected_object_type == ObjectType::BUILDING) {
    Building *building = World::get_building_by_object_id(_selected_object_id);
    RenderDataManager::submit_aabb(building->get_aabb());
  }
}

glm::mat4 view() {
  return _camera.view();
}

glm::mat4 projection() {
  return _camera.projection();
}

glm::vec3 get_camera_position() {
  return _camera.get_position();
}

void open_rts() {
  Input::show_cursor();
  Input::center_mouse_cursor();

  _is_open = true;
}

void close_rts() {
  Input::disable_cursor();
  _is_open = false;

  // Unit Halt
  if (_unit) {
    _unit.value()->set_velocity(glm::vec3(0.0f));
    _move_target = std::nullopt;
  }
}

bool is_open() {
  return _is_open;
}
} // namespace RTS
