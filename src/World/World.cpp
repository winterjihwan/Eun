#include "World.h"
#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Keycodes.h"
#include "Types/Game/BloodVolumetric.h"
#include <vector>

namespace World {
std::vector<AnimEntity>      _anim_entities;
std::vector<Bullet>          _bullets;
std::vector<Decal>           _decals;
std::vector<Npc>             _npcs;
std::vector<BloodVolumetric> _blood_volumetrics;

void init() {
  // Npc
  {
    NpcCreateInfo npc_create_info;
    npc_create_info.name           = "Brian";
    npc_create_info.model          = AssetManager::get_model_by_name("Brian");
    npc_create_info.animators.idle = AssetManager::get_animator_by_name("Brian_Idle");
    // npc_create_info.animators.walk  = AssetManager::get_animator_by_name("Brian_Walk");
    // npc_create_info.animators.death = AssetManager::get_animator_by_name("Brian_Death");
    npc_create_info.model_transform = glm::translate(glm::mat4(1.0f), glm::vec3(13.0f, 0, -5.0f));
    npc_create_info.capsule_radius  = 0.15f;
    npc_create_info.capsule_height  = 1.5f;
    npc_create_info.capsule_position =
        glm::vec3(13.0f,
                  (npc_create_info.capsule_height + 2.0f * npc_create_info.capsule_radius) / 2.0f,
                  -5.0f);

    Npc &npc = _npcs.emplace_back();
    npc.init(std::move(npc_create_info));
  }
}

void submit_render_items() {
  // Npcs
  for (Npc &npc : _npcs) {
    AnimEntity *anim_entity = npc.get_anim_entity();
    anim_entity->submit_render_item();
  }

  // Anim Entities
  for (AnimEntity &anim_entity : _anim_entities) {
    anim_entity.submit_render_item();
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

void add_anim_entity(AnimEntity &&anim_entity) {
  _anim_entities.push_back(std::move(anim_entity));
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

AnimEntity *get_anim_entity_by_name(const std::string &name) {
  for (AnimEntity &anim_entity : _anim_entities) {
    if (name == anim_entity.get_name()) {
      return &anim_entity;
    }
  }

  std::cout << "World::get_anim_entity_by_name() failed, no anim entity with name: " << name
            << std::endl;
  assert(0);
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
