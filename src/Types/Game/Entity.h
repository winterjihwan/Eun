#pragma once

#include "CreateInfo.h"
#include "Enums.h"
#include <string>

struct Entity {
  Entity(EntityCreateInfo &&info);

  void update(float delta_time);
  void submit_render_item();

  void set_collider(JPH::BodyID *collider);
  void set_on_update(std::function<void(Entity &, float)> on_update);
  void set_position(glm::vec3 &position);
  void set_rotation(glm::quat &rotation);
  void set_scale(glm::vec3 &scale);

  void add_translation(const glm::vec3 &delta);
  void add_rotation(const glm::quat &delta);
  void add_scale(const glm::vec3 &delta);

  const std::string            &get_name();
  uint64_t                      get_id();
  glm::mat4                    &get_transform();
  std::variant<Model *, Mesh *> get_renderable();

  void log_transform();

private:
  std::string                          _name;
  JPH::BodyID                          _body;
  JPH::BodyID                         *_collider = 0;
  JPH::AABox                           _aabb;
  std::variant<Model *, Mesh *>        _renderable;
  uint64_t                             _object_id;
  ObjectType                           _object_type;
  glm::vec3                            _position  = glm::vec3(0.0f);
  glm::quat                            _rotation  = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3                            _scale     = glm::vec3(1.0f);
  std::function<void(Entity &, float)> _on_update = 0;
  mutable glm::mat4                    _transform_cache;
  mutable bool                         _dirty = true;
};
