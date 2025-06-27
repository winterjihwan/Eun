#include "Core/Game.h"
#include "Player.h"
#include <glm/gtc/quaternion.hpp>

void Player::update_weapon_view(float delta_time) {
  if (_current_weapon_type != WeaponType::HAND) {
    _weapon_view.update(delta_time);

    Camera   *camera    = Game::get_camera();
    glm::vec3 cam_pos   = Game::get_player()->get_pos();
    glm::vec3 cam_front = camera->get_front();
    glm::vec3 cam_up    = camera->get_up();
    glm::vec3 cam_right = camera->get_right();

    glm::vec3 weapon_offset = cam_right * 0.15f + cam_front * 0.2f - cam_up * 0.75f;
    glm::vec3 weapon_pos    = cam_pos + weapon_offset;

    _weapon_view.set_position(weapon_pos);

    glm::quat rot = glm::quatLookAt(-cam_front, cam_up);
    _weapon_view.set_rotation(rot);

    _weapon_view.submit_render_item();
  }
}
