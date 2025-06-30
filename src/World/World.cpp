#include "World.h"
#include "AssetManager/AssetManager.h"
#include "CreateInfo.h"
#include "Defines.h"
#include "Enums.h"
#include "Keycodes.h"
#include "Types/Game/Ally/Ally.h"
#include "Types/Game/BloodVolumetric.h"
#include <vector>

namespace World {
std::vector<AnimEntity>      _anim_entities;
std::vector<Entity>          _entities;
std::vector<Bullet>          _bullets;
std::vector<Decal>           _decals;
std::vector<Bot>             _bots;
std::vector<Ally>            _allies;
std::vector<BloodVolumetric> _blood_volumetrics;

void init() {
  _anim_entities.reserve(10);
  _entities.reserve(64);
  _bullets.reserve(32);
  _decals.reserve(32);
  _bots.reserve(8);
  _allies.reserve(64);
  _blood_volumetrics.reserve(32);

  // Plane
  {
    Mesh *mesh = AssetManager::get_mesh_by_name("Plane");

    EntityCreateInfo info;
    info.name        = mesh->name;
    info.renderable  = AssetManager::get_mesh_by_name("Plane");
    info.position    = glm::vec3(0.0f, GROUND_HEIGHT, 0.0f);
    info.rotation    = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    info.scale       = glm::vec3(1.0f);
    info.object_type = ObjectType::MAP;

    add_entity(Entity(std::move(info)));
  }

  // Shop
  {
    Model *model = AssetManager::get_model_by_name("Shop");

    EntityCreateInfo info;
    info.name        = model->name;
    info.renderable  = model;
    info.position    = glm::vec3(5.0f, -2.0f, -3.0f);
    glm::quat rot_x  = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::quat rot_y  = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0, 1, 0));
    info.rotation    = rot_y * rot_x;
    info.object_type = ObjectType::GAME_OBJECT;

    add_entity(Entity(std::move(info)));
  }

  // Blue Beacon
  {

    Mesh *mesh = AssetManager::get_mesh_by_name("Blue");

    EntityCreateInfo info;
    info.name        = mesh->name;
    info.renderable  = AssetManager::get_mesh_by_name("Blue");
    info.position    = glm::vec3(3.8f, PLATFORM_HEIGHT, 4.5f);
    info.rotation    = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    info.object_type = ObjectType::PLATFORM;
    info.on_stand    = [acc = 0.0f](Entity &entity, float delta_time) mutable {
      acc += delta_time;
      if (acc >= 0.2f) {
        std::cout << "Entity " << entity.get_name() << '\n';
        acc = 0.0f;
      }
    };

    add_entity(Entity(std::move(info)));
  }

  // Mannequin
  {
    Bot &bot = _bots.emplace_back();
    bot.init("Mannequin");
  }

  // Greece_Soldier
  {
    glm::vec3 offset_position = glm::vec3(1.0f, -3.0f, -5.0f);
    spawn_ally("Greece_Soldier", offset_position);
  }
}

void submit_render_items() {
  // Bots
  for (Bot &bot : _bots) {
    AnimEntity *anim_entity = bot.get_anim_entity();
    anim_entity->submit_render_item();
  }

  // Allies
  for (Ally &ally : _allies) {
    AnimEntity *anim_entity = ally.get_anim_entity();
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

void add_ally(Ally &&ally) {
  _allies.push_back(std::move(ally));
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

Entity *get_entity_by_object_id(uint64_t object_id) {
  for (Entity &entity : _entities) {
    if (entity.get_id() == object_id) {
      return &entity;
    }
  }

  std::cout << "World::get_entity_by_object_id() failed, no entity with id: " << object_id
            << std::endl;
  assert(0);
}

Bot *get_bot_by_name(const std::string &name) {
  for (Bot &bot : _bots) {
    if (name == bot.get_name()) {
      return &bot;
    }
  }

  std::cout << "World::get_bot_by_name() failed, no npc with name: " << name << std::endl;
  assert(0);
}

Bot *get_bot_by_object_id(uint64_t object_id) {
  for (Bot &bot : _bots) {
    if (bot.get_id() == object_id) {
      return &bot;
    }
  }

  std::cout << "World::get_bot_by_object_id() failed, no npc with id: " << object_id << std::endl;
  assert(0);
}
} // namespace World
