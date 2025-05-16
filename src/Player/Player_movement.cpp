#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Player.h"
#include <glm/glm.hpp>

void Player::update_movement(float delta_time, Camera camera) {
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

  walk = glm::length(walk) > 0.0f ? glm::normalize(walk) : glm::vec3(0.0f);

  glm::vec3 vel = walk * _speed;
  auto     &bi  = Physics::get_physics_system().GetBodyInterface();

  JPH::Vec3 cur_vel = bi.GetLinearVelocity(_body_id);
  bi.SetLinearVelocity(_body_id, JPH::Vec3(vel.x, cur_vel.GetY(), vel.z));
}
