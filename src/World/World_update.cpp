#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types.h"
#include "Types/Game/Decal.h"
#include "Util/Util.h"
#include "World.h"

namespace World {
extern std::vector<AnimEntity> _anim_entities;
extern std::vector<Bullet>     _bullets;
extern std::vector<Decal>      _decals;
extern std::vector<Npc>        _npcs;

void update(float delta_time) {
  // HACK
  if (Input::key_pressed(EUN_KEY_5)) {
    Npc *brian = get_npc_by_name("Brian");
    brian->set_animation_state(NpcAnimationState::DEATH);
  }
  if (Input::key_pressed(EUN_KEY_6)) {
    Npc *brian = get_npc_by_name("Brian");
    brian->set_animation_state(NpcAnimationState::WALK);
  }

  process_bullets();

  // Npcs
  for (Npc &npc : _npcs) {
    AnimEntity *anim_entity = npc.get_anim_entity();
    anim_entity->update(delta_time);
  }

  // Anim Entities
  for (AnimEntity &anim_entity : _anim_entities) {
    anim_entity.update(delta_time);
  }
}

void process_bullets() {
  PhysicsSystem          &system = Physics::get_physics_system();
  const NarrowPhaseQuery &query  = system.GetNarrowPhaseQuery();

  for (Bullet &bullet : _bullets) {
    JPH::Vec3 origin    = Util::to_jolt_vec3(bullet.get_origin());
    JPH::Vec3 direction = Util::to_jolt_vec3(bullet.get_direction());
    float     max_dist  = 100.0f;

    RRayCast      ray(origin, direction * max_dist);
    RayCastResult result;

    const BodyLockInterface &bi = Physics::get_physics_system().GetBodyLockInterface();

    if (!query.CastRay(ray, result)) {
      continue;
    }

    BodyID      hit_body = result.mBodyID;
    float       dist     = result.mFraction * max_dist;
    const Body *body     = bi.TryGetBody(hit_body);
    if (!body) {
      std::cout << "World::process_bullets(), TryGetBody() failed" << std::endl;
      assert(0);
    }

    JPH::RVec3       pos   = body->GetPosition();
    JPH::Quat        rot   = body->GetRotation();
    const Shape     *shape = body->GetShape();
    BodyID           id    = body->GetID();
    TransformedShape ts(pos, rot, shape, id);

    JPH::Vec3 hit_pos    = origin + direction * dist;
    JPH::Vec3 hit_normal = ts.GetWorldSpaceSurfaceNormal(result.mSubShapeID2, hit_pos);

    PhysicsUserData *data = reinterpret_cast<PhysicsUserData *>(body->GetUserData());
    if (!data) {
      // TODO: Add physics user data for all physics objects
      continue;
    }

    // printf("Bullet hit body %u at distance %.2f, userData = %p\n", hit_body.GetIndex(), dist,
    // data);
    if (data->physics_type == PhysicsType::RIGID_STATIC) {

      DecalCreateInfo decal_create_info;
      decal_create_info.hit_position = Util::from_jolt_vec3(hit_pos);
      decal_create_info.hit_normal   = Util::from_jolt_vec3(hit_normal);

      if (data->object_type == ObjectType::MAP) {
        decal_create_info.type = DecalType::PLASTER;
      }

      add_decal(Decal(decal_create_info));
    }
  }

  _bullets.clear();
}
} // namespace World
