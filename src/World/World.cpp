#include "World.h"
#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Enums.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include "Types/Game/AnimEntity.h"
#include <vector>

namespace World {
std::vector<AnimEntity> _anim_entities;
std::vector<Bullet>     _bullets;
std::vector<Npc>        _npcs;

void init() {
  // Brian
  {
    NpcCreateInfo npc_create_info;
    npc_create_info.name            = "Brian";
    npc_create_info.model           = AssetManager::get_model_by_name("Brian");
    npc_create_info.animators.idle  = AssetManager::get_animator_by_name("Brian_Idle");
    npc_create_info.animators.walk  = AssetManager::get_animator_by_name("Brian_Walk");
    npc_create_info.animators.death = AssetManager::get_animator_by_name("Brian_Death");
    npc_create_info.model_transform = glm::translate(glm::mat4(1.0f), glm::vec3(13.0f, 0, -5.0f));
    npc_create_info.capsule_radius  = 0.3f;
    npc_create_info.capsule_height  = 1.2f;
    npc_create_info.capsule_position =
        glm::vec3(13.0f,
                  (npc_create_info.capsule_height + 2.0f * npc_create_info.capsule_radius) / 2.0f,
                  -5.0f);

    Npc &npc = _npcs.emplace_back();
    npc.init(std::move(npc_create_info));
  }
}

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
}

void process_bullets() {
  std::vector<Bullet>    &bullets = get_bullets();
  PhysicsSystem          &system  = Physics::get_physics_system();
  const NarrowPhaseQuery &query   = system.GetNarrowPhaseQuery();

  for (Bullet &bullet : bullets) {
    JPH::Vec3 origin    = Util::to_jolt_vec3(bullet.get_origin());
    JPH::Vec3 direction = Util::to_jolt_vec3(bullet.get_direction());
    float     maxDist   = 100.0f;

    RRayCast      ray(origin, direction * maxDist);
    RayCastResult result;

    if (query.CastRay(ray, result)) {
      BodyID hitBody = result.mBodyID;
      float  dist    = result.mFraction * maxDist;

      printf("Bullet hit body %u at distance %.2f\n", hitBody.GetIndex(), dist);
    }
  }

  bullets.clear();
}

void add_anim_entity(AnimEntity &&anim_entity) {
  _anim_entities.push_back(std::move(anim_entity));
}

void add_bullet(Bullet &&bullet) {
  _bullets.push_back(std::move(bullet));
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
