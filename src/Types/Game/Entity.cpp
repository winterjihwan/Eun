#include "Entity.h"
#include "Physics/Physics.h"
#include "Renderer/RenderDataManager.h"
#include <variant>

Entity::Entity(EntityCreateInfo &&info) {
  _name       = info.name;
  _renderable = info.renderable;
  _transform  = info.transform;

  if (std::holds_alternative<Mesh *>(_renderable)) {
    Mesh *mesh = std::get<Mesh *>(_renderable);
    _mesh      = Physics::register_static_mesh(
        mesh->vertices, mesh->indices, info.transform, info.object_type);
    _aabb = Physics::get_aabb(_mesh);
  } else {
    // TODO: Physics creation for <Model *>
  }
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

glm::mat4 &Entity::get_transform() {
  return _transform;
}

std::variant<Model *, Mesh *> Entity::get_renderable() {
  return _renderable;
}
