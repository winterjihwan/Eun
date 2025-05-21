#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Player.h"
#include "Types.h"
#include <glm/glm.hpp>

void Player::update_movement(float delta_time, Camera camera) {
  // Walk
  glm::vec3 front = camera.get_front();
  front.y         = 0.0f;
  glm::vec3 right = camera.get_right();

  glm::vec3 walk = glm::vec3(0);
  if (Input::key_down(EUN_KEY_W)) {
    walk += front;
  }
  if (Input::key_down(EUN_KEY_S)) {
    walk -= front;
  }
  if (Input::key_down(EUN_KEY_A)) {
    walk -= right;
  }
  if (Input::key_down(EUN_KEY_D)) {
    walk += right;
  }

  walk          = glm::length(walk) > 0.0f ? glm::normalize(walk) : glm::vec3(0.0f);
  glm::vec3 vel = walk * _speed;

  // Jump
  auto &bi        = Physics::get_physics_system().GetBodyInterface();
  float cur_vel_y = bi.GetLinearVelocity(_body_id).GetY();

  if (Input::key_pressed(EUN_KEY_SPACE) && _on_ground) {
    cur_vel_y += 4.9f;
    _on_ground = false;
  }

  bi.SetLinearVelocity(_body_id, JPH::Vec3(vel.x, cur_vel_y, vel.z));
}
