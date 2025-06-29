#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types.h"
#include "Types/Game/BloodVolumetric.h"
#include "Types/Game/Decal.h"
#include "Types/Game/NpcAlly.h"
#include "Util/Util.h"
#include "World.h"

namespace World {
extern std::vector<AnimEntity>      _anim_entities;
extern std::vector<Entity>          _entities;
extern std::vector<Bullet>          _bullets;
extern std::vector<Decal>           _decals;
extern std::vector<NpcEnemy>        _npc_enemies;
extern std::vector<NpcAlly>         _npc_allies;
extern std::vector<BloodVolumetric> _blood_volumetrics;

void update(float delta_time) {
  process_bullets();

  // Npcs
  for (NpcEnemy &npc : _npc_enemies) {
    npc.update(delta_time);
  }
  for (NpcAlly &npc : _npc_allies) {
    npc.update(delta_time);
  }

  // Anim Entities
  for (AnimEntity &anim_entity : _anim_entities) {
    anim_entity.update(delta_time);
  }

  // Entities
  for (Entity &entity : _entities) {
    entity.update(delta_time);
  }

  // Blood Volumetrics
  for (auto it = _blood_volumetrics.begin(); it != _blood_volumetrics.end();) {
    if (it->_current_time > 2.0f) {
      it = _blood_volumetrics.erase(it);
    } else {
      it->update(delta_time);
      it++;
    }
  }
}

void process_bullets() {
  for (Bullet &bullet : _bullets) {
    std::optional<RayHitInfo> hit =
        Physics::raycast(bullet.get_origin(), bullet.get_direction(), 100.0f);
    if (!hit.has_value()) {
      continue;
    }

    PhysicsUserData *data = hit->user_data;
    if (!data) {
      continue;
    }

    // Bullet hole decal
    if (data->physics_type == PhysicsType::RIGID_STATIC) {
      DecalCreateInfo info;
      info.hit_position = Util::to_vec3(hit->hit_pos);
      info.hit_normal   = Util::to_vec3(hit->hit_normal);
      info.type         = DecalType::PLASTER;
      info.mesh         = AssetManager::get_mesh_by_name("Bullet_Hole");

      if (data->object_type == ObjectType::MAP) {
        info.type = DecalType::PLASTER;
      }

      add_decal(Decal(info));
    }

    // Blood
    if (data->physics_type == PhysicsType::RIGID_DYNAMIC &&
        data->object_type == ObjectType::NPC_ENEMY) {

      static unsigned int blood_volumetric_index = 1;
      if (++blood_volumetric_index > 6)
        blood_volumetric_index = 1;

      // Blood Volumetric
      BloodVolumetricCreateInfo info;
      info.position          = Util::to_vec3(hit->hit_pos);
      info.rotation          = glm::vec3(0.0f);
      info.front             = bullet.get_direction();
      info.exr_texture_index = blood_volumetric_index;
      info.model = AssetManager::get_model_by_name(std::format("Blood_{}", blood_volumetric_index));

      add_blood_volumetric(BloodVolumetric(info));

      // Blood Decal Raycast
      JPH::Vec3 blood_origin      = hit->hit_pos + 0.02f * hit->hit_normal;
      JPH::Vec3 blood_dir         = JPH::Vec3(0.0f, -1.0f, 0.0f);
      float     blood_splash_dist = 10.0f;

      auto decal_hit = Physics::raycast(blood_origin, blood_dir, blood_splash_dist);
      if (!decal_hit || !decal_hit->user_data) {
        return;
      }

      if (decal_hit->user_data->object_type != ObjectType::MAP) {
        return;
      }

      static unsigned int _blood_stain_index = 1;
      if (++_blood_stain_index > 4)
        _blood_stain_index = 1;

      glm::vec3 offset =
          glm::vec3(Util::random_float(-0.3f, 0.3f), 0.0f, Util::random_float(-0.3f, 0.3f));

      DecalCreateInfo blood_info;
      blood_info.hit_position = Util::to_vec3(decal_hit->hit_pos) + offset;
      blood_info.hit_normal   = Util::to_vec3(decal_hit->hit_normal);
      blood_info.type         = DecalType::BLOOD;
      blood_info.mesh =
          AssetManager::get_mesh_by_name(std::format("Blood_Stain_{}", _blood_stain_index));

      add_decal(Decal(blood_info));
    }
  }

  _bullets.clear();
}
} // namespace World
