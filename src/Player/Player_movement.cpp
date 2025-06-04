#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Player.h"
#include "Types.h"
#include <glm/glm.hpp>

void Player::update_movement(float delta_time, Camera camera) {
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

  // Update Player State
  if (_character->GetGroundState() != CharacterBase::EGroundState::OnGround) {
    _player_state = PlayerState::JUMPING;

  } else if (glm::length(walk_dir) > 0.0f) {
    walk_dir = glm::normalize(walk_dir);
    front    = glm::normalize(front);
    right    = glm::normalize(right);

    float forward_dot = glm::dot(walk_dir, front);
    float right_dot   = glm::dot(walk_dir, right);

    if (forward_dot > 0.5f)
      _player_state = PlayerState::WALKING_FORWARD;
    else if (forward_dot < -0.5f)
      _player_state = PlayerState::WALKING_BACKWARD;
    else if (right_dot > 0.5f)
      _player_state = PlayerState::WALKING_RIGHT;
    else if (right_dot < -0.5f)
      _player_state = PlayerState::WALKING_LEFT;
  } else {
    _player_state = PlayerState::IDLE;
  }

  Vec3 move    = Vec3(walk_dir.x, 0.0f, walk_dir.z) * _speed;
  Vec3 cur_vel = _character->GetLinearVelocity();
  Vec3 new_vel = move + Vec3(0, cur_vel.GetY(), 0);

  if (_character->GetGroundState() == CharacterBase::EGroundState::OnGround) {
    // TODO: Enable Jump
    if (Input::key_pressed(EUN_KEY_SPACE)) {
      new_vel.SetY(4.9f);
    }
  } else {
    new_vel += Physics::get_physics_system().GetGravity() * delta_time;
  }

  _character->SetLinearVelocity(new_vel);

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

  _position = get_pos();
}
