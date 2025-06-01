#include "AnimEntity.h"
#include "Physics/Physics.h"
#include "Renderer/RenderDataManager.h"

void AnimEntity::init(AnimEntityCreateInfo &&anim_entity_create_info) {
  _name             = anim_entity_create_info.name;
  _model            = anim_entity_create_info.model;
  _current_animator = anim_entity_create_info.animator;
  _model_transform  = anim_entity_create_info.transform;
}

void AnimEntity::update(float delta_time) {
  if (_current_animator != 0) {
    _current_animator->UpdateAnimation(delta_time);
  }

  if (_collider) {
    glm::mat4   model_transform = glm::mat4(1.0f);
    const Body *body = Physics::get_physics_system().GetBodyLockInterface().TryGetBody(*_collider);
    if (body) {
      RVec3 pos = body->GetCenterOfMassPosition();

      // HACK: Subtract capsule height by half
      _model_transform =
          glm::translate(model_transform, glm::vec3(pos.GetX(), pos.GetY() - 0.9f, pos.GetZ()));
    } else {
      std::cout << "AnimEntity::update(), No body found for body_id: " << _collider->GetIndex()
                << std::endl;
      assert(0);
    }
  }
}

void AnimEntity::submit_render_item() {
  // TODO: Actually submit render item
  RenderDataManager::submit_anim_entity(this);
}

void AnimEntity::set_model(Model *model) {
  _model = model;
}

void AnimEntity::set_animator(Animator *animator) {
  if (_current_animator == animator) {
    return;
  }

  _current_animator = animator;
  _current_animator->PlayAnimation();
}

void AnimEntity::set_model_transform(glm::mat4 model_transform) {
  _model_transform = model_transform;
}

void AnimEntity::set_collider(JPH::BodyID *collider) {
  _collider = collider;
}

const std::string &AnimEntity::get_name() {
  return _name;
}

Model *AnimEntity::get_model() {
  return _model;
}

Animator *AnimEntity::get_animator() {
  return _current_animator;
}

glm::mat4 &AnimEntity::get_model_transform() {
  return _model_transform;
}
