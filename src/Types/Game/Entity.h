#pragma once

#include "CreateInfo.h"
#include <string>

struct Entity {
  Entity(EntityCreateInfo &&info);

  void update(float delta_time);
  void submit_render_item();
  void set_collider(JPH::BodyID *collider);
  void set_on_update(std::function<void(Entity &, float)> on_update);

  const std::string            &get_name();
  glm::mat4                    &get_transform();
  std::variant<Model *, Mesh *> get_renderable();

private:
  std::string                          _name;
  JPH::BodyID                          _mesh;
  JPH::AABox                           _aabb;
  std::variant<Model *, Mesh *>        _renderable;
  JPH::BodyID                         *_collider  = 0;
  glm::mat4                            _transform = glm::mat4(1.0f);
  std::function<void(Entity &, float)> _on_update = 0;
};
