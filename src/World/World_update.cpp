#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types.h"
#include "Types/Game/BloodVolumetric.h"
#include "Types/Game/Building/Building.h"
#include "Types/Game/Decal.h"
#include "Types/Game/Unit/Unit.h"
#include "Util/Util.h"
#include "World.h"

namespace World {
extern std::vector<AnimEntity>      _anim_entities;
extern std::vector<Entity>          _entities;
extern std::vector<Bullet>          _bullets;
extern std::vector<Decal>           _decals;
extern std::vector<Building>        _buildings;
extern std::vector<Unit>            _units;
extern std::vector<BloodVolumetric> _blood_volumetrics;

void update(float delta_time) {
  process_bullets();

  // Buildings
  for (Building &building : _buildings) {
    building.update(delta_time);
  }

  // Units
  for (Unit &unit : _units) {
    unit.update(delta_time);
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
  }

  _bullets.clear();
}
} // namespace World
