#include "Defines.h"
#include "Physics/Physics.h"
#include "Player.h"
#include "Types.h"
#include "Types/Game/Entity.h"
#include "World/World.h"

void Player::update_feet_interaction(float delta_time) {
  std::optional<RayHitInfo> hit = Physics::raycast(_position, DOWN_DIRECTION, 3.0f);
  if (!hit || !hit->user_data) {
    return;
  }

  PhysicsUserData *data = hit->user_data;

  if (data->object_type == ObjectType::PLATFORM) {
    Entity *entity = World::get_entity_by_object_id(data->object_id);
    entity->on_stand(delta_time);
  }
}
