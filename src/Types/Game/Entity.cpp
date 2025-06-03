#include "Entity.h"
#include "Renderer/RenderDataManager.h"

void Entity::init(EntityCreateInfo &&entity_create_info) {
  _name      = entity_create_info.name;
  _model     = entity_create_info.model;
  _transform = entity_create_info.transform;
}

void Entity::update(float delta_time) {
  if (_on_update) {
    _on_update(*this, delta_time);
  }
}

void Entity::submit_render_item() {
  RenderDataManager::submit_entity(this);
}

void Entity::set_collider(JPH::BodyID *collider) {
  _collider = collider;
}

void Entity::set_on_update(std::function<void(Entity &, float)> on_update) {
  _on_update = on_update;
}

const std::string &Entity::get_name() {
  return _name;
}

Model *Entity::get_model() {
  return _model;
}

glm::mat4 &Entity::get_transform() {
  return _transform;
}
