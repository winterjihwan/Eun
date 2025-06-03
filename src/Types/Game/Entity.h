#pragma once

#include "CreateInfo.h"
#include "Model.h"
#include <string>

struct Entity {
  Entity()          = default;
  virtual ~Entity() = default;

  void         init(EntityCreateInfo &&entity_create_info);
  virtual void update(float delta_time);
  void         submit_render_item();
  void         set_collider(JPH::BodyID *collider);
  void         set_on_update(std::function<void(Entity &, float)> on_update);

  const std::string &get_name();
  Model             *get_model();
  glm::mat4         &get_transform();

private:
  std::string                          _name;
  Model                               *_model;
  JPH::BodyID                         *_collider  = 0;
  glm::mat4                            _transform = glm::mat4(1.0f);
  std::function<void(Entity &, float)> _on_update = 0;
};
