#include "World.h"
#include "AssetManager/AssetManager.h"
#include "Defines.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include <vector>

namespace World {
std::vector<AnimEntity> _anim_entities;

void init() {
  //  Brian
  {
    AnimEntity *brian_anim_entity   = &_anim_entities.emplace_back();
    Model      *brian_model         = AssetManager::get_model_by_name("Brian");
    Animator   *brian_animator_walk = AssetManager::get_animator_by_name("Brian_Walk");

    glm::mat4 transform_brian = glm::mat4(1.0f);
    transform_brian = glm::translate(transform_brian, glm::vec3(13.0f, PLAYER_HEIGHT, -5.0f));
    transform_brian = glm::scale(transform_brian, glm::vec3(.5f, .5f, .5f));

    brian_anim_entity->init(brian_model, brian_animator_walk, transform_brian, "Brian");
  }
}

void update(float delta_time) {
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
