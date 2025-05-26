#include "World.h"
#include "AssetManager/AssetManager.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"
#include <vector>

namespace World {
std::vector<AnimEntity> _anim_entities;
std::vector<Bullet>     _bullets;

void init() {
  //  Brian
  {
    AnimEntity *brian_anim_entity   = &_anim_entities.emplace_back();
    Model      *brian_model         = AssetManager::get_model_by_name("Brian");
    Animator   *brian_animator_walk = AssetManager::get_animator_by_name("Brian_Idle");

    glm::mat4 transform_brian = glm::mat4(1.0f);
    transform_brian           = glm::translate(transform_brian, glm::vec3(13.0f, 0, -5.0f));

    brian_anim_entity->init(brian_model, brian_animator_walk, transform_brian, "Brian");
  }
}

void update(float delta_time) {
  // HACK
  if (Input::key_pressed(EUN_KEY_5)) {
    AnimEntity *brian_anim_entity    = get_anim_entity_by_name("Brian");
    Animator   *brian_animator_death = AssetManager::get_animator_by_name("Brian_Death");
    brian_animator_death->PlayAnimation();
    brian_anim_entity->set_animator(brian_animator_death);
  }
  if (Input::key_pressed(EUN_KEY_6)) {
    AnimEntity *brian_anim_entity   = get_anim_entity_by_name("Brian");
    Animator   *brian_animator_walk = AssetManager::get_animator_by_name("Brian_Walk");
    brian_animator_walk->PlayAnimation();
    brian_anim_entity->set_animator(brian_animator_walk);
  }

  process_bullets();

  // Anim Entities
  for (AnimEntity &anim_entity : _anim_entities) {
    anim_entity.update(delta_time);
  }
}

void submit_render_items() {
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

      printf("Bullet hit body %u at distance %.2f\n", hitBody.GetIndexAndSequenceNumber(), dist);
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
} // namespace World
