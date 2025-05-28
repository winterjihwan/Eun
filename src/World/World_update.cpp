#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types.h"
#include "Types/Game/BloodVolumetric.h"
#include "Types/Game/Decal.h"
#include "Util/Util.h"
#include "World.h"

namespace World {
extern std::vector<AnimEntity>      _anim_entities;
extern std::vector<Bullet>          _bullets;
extern std::vector<Decal>           _decals;
extern std::vector<Npc>             _npcs;
extern std::vector<BloodVolumetric> _blood_volumetrics;

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

  // Blood Volumetrics
  for (BloodVolumetric &blood_volumetric : _blood_volumetrics) {
    blood_volumetric.update(delta_time);
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

    if (!query.CastRay(ray, result)) {
      continue;
    }

    const BodyLockInterface &bi   = Physics::get_physics_system().GetBodyLockInterface();
    const Body              *body = bi.TryGetBody(result.mBodyID);
    if (!body) {
      std::cout << "World::process_bullets(), during decal TryGetBody() failed" << std::endl;
      assert(0);
    }

    PhysicsUserData *data = reinterpret_cast<PhysicsUserData *>(body->GetUserData());

    // Decal
    float            dist  = result.mFraction * max_dist;
    JPH::RVec3       pos   = body->GetPosition();
    JPH::Quat        rot   = body->GetRotation();
    const Shape     *shape = body->GetShape();
    BodyID           id    = body->GetID();
    TransformedShape ts(pos, rot, shape, id);

    JPH::Vec3 hit_pos    = origin + direction * dist;
    JPH::Vec3 hit_normal = ts.GetWorldSpaceSurfaceNormal(result.mSubShapeID2, hit_pos);

    if (!data) {
      // TODO: Add physics user data for all physics objects
      continue;
    }

    if (data->physics_type == PhysicsType::RIGID_STATIC) {
      DecalCreateInfo decal_create_info;
      decal_create_info.hit_position = Util::from_jolt_vec3(hit_pos);
      decal_create_info.hit_normal   = Util::from_jolt_vec3(hit_normal);

      if (data->object_type == ObjectType::MAP) {
        decal_create_info.type = DecalType::PLASTER;
      }

      add_decal(Decal(decal_create_info));
    }

    // Blood
    if (data->physics_type == PhysicsType::RIGID_DYNAMIC) {
      if (data->object_type == ObjectType::NPC) {

        JPH::Vec3 blood_origin = hit_pos + 0.02f * hit_normal;
        JPH::Vec3 blood_dir =
            Util::to_jolt_vec3(Util::random_dir_in_hemisphere(Util::from_jolt_vec3(hit_normal)))
                .Normalized();
        float blood_splash_dist = 10.0f;

        RRayCast      blood_ray(blood_origin, blood_dir * blood_splash_dist);
        RayCastResult blood_result;

        if (!query.CastRay(blood_ray, blood_result)) {
          continue;
        }

        const BodyLockInterface &bi     = Physics::get_physics_system().GetBodyLockInterface();
        const Body              *target = bi.TryGetBody(blood_result.mBodyID);
        if (!target) {
          std::cout << "World::process_bullets(), during blood TryGetBody() failed" << std::endl;
          continue;
        }

        float            dist  = blood_result.mFraction * blood_splash_dist;
        JPH::RVec3       pos   = target->GetPosition();
        JPH::Quat        rot   = target->GetRotation();
        const Shape     *shape = target->GetShape();
        BodyID           id    = target->GetID();
        TransformedShape ts(pos, rot, shape, id);

        JPH::Vec3 hit_pos    = blood_origin + blood_dir * dist;
        JPH::Vec3 hit_normal = ts.GetWorldSpaceSurfaceNormal(result.mSubShapeID2, hit_pos);

        PhysicsUserData *data = reinterpret_cast<PhysicsUserData *>(target->GetUserData());
        if (!data) {
          continue;
        }

        if (data->object_type != ObjectType::MAP) {
          continue;
        }

        DecalCreateInfo blood_info;
        blood_info.hit_position = Util::from_jolt_vec3(hit_pos);
        blood_info.hit_normal   = Util::from_jolt_vec3(hit_normal);
        blood_info.type         = DecalType::BLOOD;

        add_decal(Decal(blood_info));
      }
    }
  }

  _bullets.clear();
}
} // namespace World
