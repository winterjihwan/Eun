#pragma once

#include "CreateInfo.h"

struct Bullet {
  Bullet() = default;

  Bullet(BulletCreateInfo &bullet_create_info) {
    _origin       = bullet_create_info.origin;
    _direction    = bullet_create_info.direction;
    _weapon_index = bullet_create_info.weapon_index;
    _damage       = bullet_create_info.damage;
  }

  glm::vec3 get_origin() {
    return _origin;
  }
  glm::vec3 get_direction() {
    return _direction;
  }
  int32_t get_weapon_index() {
    return _weapon_index;
  }
  uint32_t get_damage() {
    return _damage;
  }

private:
  glm::vec3 _origin       = glm::vec3(0);
  glm::vec3 _direction    = glm::vec3(0);
  int32_t   _weapon_index = 0;
  uint32_t  _damage       = 0;
};
