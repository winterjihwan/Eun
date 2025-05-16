#include "Player.h"
#include "Defines.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include <glm/glm.hpp>

void Player::init(glm::vec3 position) {
  _position = position;
  _body_id  = create_player_physics(
      glm::vec3(_position.x, _position.y, _position.z), PLAYER_HEIGHT, 0.25f, Layers::MOVING);
  // TODO: Only if its ground tho, deffered to asset system
  Physics::register_on_contact(_body_id,
                               [this](const BodyID &, const BodyID &) { _on_ground = true; });
}

void Player::update(float delta_time, Camera camera) {
  update_movement(delta_time, camera);
  update_flashlight();
}

glm::vec3 Player::get_pos() {
  auto      &body_interface = Physics::get_physics_system().GetBodyInterface();
  JPH::RVec3 pos            = body_interface.GetCenterOfMassPosition(_body_id);

  return glm::vec3(pos.GetX(), pos.GetY() + PLAYER_HEIGHT, pos.GetZ());
}
