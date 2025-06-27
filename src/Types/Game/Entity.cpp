#include "Entity.h"
#include "Physics/Physics.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Renderer/Model.h"
#include "Types/UID/UID.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <variant>

Entity::Entity(EntityCreateInfo &&info) {
  _name        = info.name;
  _renderable  = info.renderable;
  _position    = info.position;
  _rotation    = info.rotation;
  _scale       = info.scale;
  _object_type = info.object_type;
  _object_id   = UID::get_next();

  if (std::holds_alternative<Mesh *>(_renderable)) {
    Mesh *mesh = std::get<Mesh *>(_renderable);
    _body      = Physics::register_static_mesh(
        mesh->vertices, mesh->indices, get_transform(), _object_type, _object_id);
    _aabb = Physics::get_aabb(_body);

  } else {
    Model *model = std::get<Model *>(_renderable);
    _body        = Physics::register_static_mesh(
        model->get_vertices(), model->get_indices(), get_transform(), _object_type, _object_id);
    _aabb = Physics::get_aabb(_body);
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

void Entity::set_on_update(std::function<void(Entity &, float)> on_update) {
  _on_update = on_update;
}

void Entity::set_position(glm::vec3 &position) {
  _position = position;
  _dirty    = true;
}

void Entity::set_rotation(glm::quat &rotation) {
  _rotation = rotation;
  _dirty    = true;
}

void Entity::set_scale(glm::vec3 &scale) {
  _scale = scale;
  _dirty = true;
}

void Entity::add_translation(const glm::vec3 &delta) {
  _position += delta;
  _dirty = true;
}

void Entity::add_rotation(const glm::quat &delta) {
  _rotation = delta * _rotation;
  _dirty    = true;
}

void Entity::add_scale(const glm::vec3 &delta) {
  _scale *= delta;
  _dirty = true;
}

const std::string &Entity::get_name() {
  return _name;
}

uint64_t Entity::get_id() {
  return _object_id;
}

glm::mat4 &Entity::get_transform() {
  if (_dirty) {
    glm::mat4 T      = glm::translate(glm::mat4(1.0f), _position);
    glm::mat4 R      = glm::toMat4(_rotation);
    glm::mat4 S      = glm::scale(glm::mat4(1.0f), _scale);
    _transform_cache = T * R * S;
    _dirty           = false;
  }

  return _transform_cache;
}

std::variant<Model *, Mesh *> Entity::get_renderable() {
  return _renderable;
}

void Entity::log_transform() {
  std::cout << _name << '\n';
  std::cout << "  Position: (" << std::fixed << std::setprecision(2) << _position.x << ", "
            << _position.y << ", " << _position.z << ")\n";
  std::cout << "  Rotation: (" << _rotation.x << ", " << _rotation.y << ", " << _rotation.z << ", "
            << _rotation.w << ")\n";
  std::cout << "  Scale   : (" << _scale.x << ", " << _scale.y << ", " << _scale.z << ")\n";
}
