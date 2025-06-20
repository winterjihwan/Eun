#include "World.h"
#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Keycodes.h"
#include "Types/Game/BloodVolumetric.h"
#include <vector>

namespace World {
std::vector<AnimEntity>      _anim_entities;
std::vector<Entity>          _entities;
std::vector<Bullet>          _bullets;
std::vector<Decal>           _decals;
std::vector<Npc>             _npcs;
std::vector<BloodVolumetric> _blood_volumetrics;

void init() {
  _anim_entities.reserve(10);
  _entities.reserve(64);
  _bullets.reserve(32);
  _decals.reserve(32);
  _npcs.reserve(8);
  _blood_volumetrics.reserve(32);

  // Custom Map
  {
    Mesh *mesh = AssetManager::get_mesh_by_name("Plane");

    glm::mat4 pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
    pos           = glm::rotate(pos, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    EntityCreateInfo info;
    info.name       = mesh->name;
    info.renderable = AssetManager::get_mesh_by_name("Plane");
    info.transform  = pos;

    add_entity(Entity(std::move(info)));
  }

  // Npc
  // {
  //   NpcCreateInfo npc_create_info;
  //   npc_create_info.name            = "Npc1";
  //   npc_create_info.model           = AssetManager::get_skinned_model_by_name("Brian");
  //   npc_create_info.animations.idle = "Idle";
  //   // npc_create_info.animators.walk  = AssetManager::get_animator_by_name("Brian_Walk");
  //   // npc_create_info.animators.death = AssetManager::get_animator_by_name("Brian_Death");
  //   npc_create_info.model_transform =
  //       glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, -5.0f));
  //   npc_create_info.capsule_radius   = 0.15f;
  //   npc_create_info.capsule_height   = 1.5f;
  //   npc_create_info.capsule_position = glm::vec3(
  //       0.0f,
  //       (npc_create_info.capsule_height + 2.0f * npc_create_info.capsule_radius) / 2.0f - 3.0f,
  //       -5.0f);
  //
  //   Npc &npc = _npcs.emplace_back();
  //   npc.init(std::move(npc_create_info));
  // }
}

void submit_render_items() {
  // Npc
  for (Npc &npc : _npcs) {
    AnimEntity *anim_entity = npc.get_anim_entity();
    anim_entity->submit_render_item();
  }

  // Anim Entities
  for (AnimEntity &anim_entity : _anim_entities) {
    anim_entity.submit_render_item();
  }

  // Entities
  for (Entity &entity : _entities) {
    entity.submit_render_item();
  }

  // Blood Volumetrics
  for (BloodVolumetric &blood_volumetric : _blood_volumetrics) {
    blood_volumetric.submit_render_item();
  }

  // Decals
  for (Decal &decal : _decals) {
    decal.submit_render_item();
  }
}

AnimEntity *add_anim_entity(AnimEntity &&anim_entity) {
  _anim_entities.push_back(std::move(anim_entity));
  return &_anim_entities.back();
}

Entity *add_entity(Entity &&entity) {
  _entities.push_back(std::move(entity));
  return &_entities.back();
}

void add_bullet(Bullet &&bullet) {
  _bullets.push_back(std::move(bullet));
}

void add_decal(Decal &&decal) {
  _decals.push_back(std::move(decal));
}

void add_blood_volumetric(BloodVolumetric &&blood_volumetric) {
  _blood_volumetrics.push_back(std::move(blood_volumetric));
}

std::vector<Bullet> &get_bullets() {
  return _bullets;
}

std::vector<Npc> &get_npcs() {
  return _npcs;
}

Npc *get_npc_by_name(const std::string &name) {
  for (Npc &npc : _npcs) {
    if (name == npc.get_name()) {
      return &npc;
    }
  }

  std::cout << "World::get_npc_by_name() failed, no npc with name: " << name << std::endl;
  assert(0);
}
} // namespace World
