#include "AssetManager/AssetManager.h"
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
  for (auto it = _blood_volumetrics.begin(); it != _blood_volumetrics.end();) {
    if (it->_current_time > 0.9f) {
      it = _blood_volumetrics.erase(it);
    } else {
      it->update(delta_time);
      it++;
    }
  }
}

void process_bullets() {
  for (Bullet &bullet : _bullets) {
    JPH::Vec3 origin    = Util::to_jolt_vec3(bullet.get_origin());
    JPH::Vec3 direction = Util::to_jolt_vec3(bullet.get_direction());
    float     max_dist  = 100.0f;

    std::optional<RayHitInfo> hit = Physics::raycast(origin, direction, max_dist);
    if (!hit.has_value()) {
      continue;
    }

    PhysicsUserData *data = hit->user_data;
    if (!data) {
      continue;
    }

    // Bullet hole decal
    if (data->physics_type == PhysicsType::RIGID_STATIC) {
      DecalCreateInfo decal_create_info;
      decal_create_info.hit_position = Util::from_jolt_vec3(hit->hit_pos);
      decal_create_info.hit_normal   = Util::from_jolt_vec3(hit->hit_normal);
      decal_create_info.type         = DecalType::PLASTER;
      decal_create_info.mesh         = AssetManager::get_mesh_by_name("Bullet_Hole");

      if (data->object_type == ObjectType::MAP) {
        decal_create_info.type = DecalType::PLASTER;
      }

      add_decal(Decal(decal_create_info));
    }

    // Blood
    if (data->physics_type == PhysicsType::RIGID_DYNAMIC && data->object_type == ObjectType::NPC) {

      // Blood Volumetric
      BloodVolumetricCreateInfo blood_volumetric_create_info;
      blood_volumetric_create_info.position = Util::from_jolt_vec3(hit->hit_pos);
      blood_volumetric_create_info.rotation = glm::vec3(0.0f);
      blood_volumetric_create_info.front    = bullet.get_direction();
      blood_volumetric_create_info.model    = AssetManager::get_model_by_name("Blood_6");
      blood_volumetric_create_info.exr_texture_pos =
          AssetManager::get_exr_texture_by_name("blood_pos_6");
      blood_volumetric_create_info.exr_texture_norm =
          AssetManager::get_exr_texture_by_name("blood_norm_6");

      add_blood_volumetric(BloodVolumetric(blood_volumetric_create_info));

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
      blood_info.hit_position = Util::from_jolt_vec3(decal_hit->hit_pos) + offset;
      blood_info.hit_normal   = Util::from_jolt_vec3(decal_hit->hit_normal);
      blood_info.type         = DecalType::BLOOD;
      blood_info.mesh =
          AssetManager::get_mesh_by_name(std::format("Blood_Stain_{}", _blood_stain_index));

      add_decal(Decal(blood_info));
    }
  }

  _bullets.clear();
}
} // namespace World
