#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Player.h"
#include "Types.h"
#include <glm/glm.hpp>

void Player::update_movement(float delta_time, Camera camera) {
  using namespace JPH;

  glm::vec3 front = camera.get_front();
  front.y         = 0.0f;
  glm::vec3 right = camera.get_right();

  glm::vec3 walk_dir(0.0f);
  if (Input::key_down(EUN_KEY_W))
    walk_dir += front;
  if (Input::key_down(EUN_KEY_S))
    walk_dir -= front;
  if (Input::key_down(EUN_KEY_A))
    walk_dir -= right;
  if (Input::key_down(EUN_KEY_D))
    walk_dir += right;
  if (glm::length(walk_dir) > 0.0f)
    walk_dir = glm::normalize(walk_dir);

  Vec3 move = Vec3(walk_dir.x, 0.0f, walk_dir.z) * _speed;
  Vec3 vel  = move;

  // Jump
  if (_character->GetGroundState() == CharacterBase::EGroundState::OnGround &&
      Input::key_pressed(EUN_KEY_SPACE)) {
    vel.SetY(4.9f);
  } else {
    vel += Physics::get_physics_system().GetGravity() * delta_time;
  }

  _character->SetLinearVelocity(vel);

  CharacterVirtual::ExtendedUpdateSettings settings;
  _character->ExtendedUpdate(
      delta_time,
      Physics::get_physics_system().GetGravity(),
      settings,
      Physics::get_physics_system().GetDefaultBroadPhaseLayerFilter(Layers::MOVING),
      Physics::get_physics_system().GetDefaultLayerFilter(Layers::MOVING),
      {},
      {},
      *Physics::get_temp_allocator());
}
