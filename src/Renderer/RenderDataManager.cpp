#include "RenderDataManager.h"

namespace RenderDataManager {
std::vector<AnimEntity *>      _anim_entities;
std::vector<Entity *>          _entities;
std::vector<Decal *>           _decals;
std::vector<BloodVolumetric *> _blood_volumetrics;
std::vector<AABB *>            _aabbs;

void begin_frame() {
  // DID YOU FORGET ME
  _anim_entities.clear();
  _entities.clear();
  _decals.clear();
  _blood_volumetrics.clear();
  _aabbs.clear();
}

void update() {
  // TODO: Draw Indirect Commands
}

void submit_anim_entity(AnimEntity *anim_entity) {
  _anim_entities.push_back(anim_entity);
}

void submit_entity(Entity *entity) {
  _entities.push_back(entity);
}

void submit_decal(Decal *decal) {
  _decals.push_back(decal);
}

void submit_blood_volumetric(BloodVolumetric *blood_volumetric) {
  _blood_volumetrics.push_back(blood_volumetric);
}

void submit_aabb(AABB *aabb) {
  _aabbs.push_back(aabb);
}

const std::vector<AnimEntity *> &get_anim_entities() {
  return _anim_entities;
}

const std::vector<Entity *> &get_entities() {
  return _entities;
}

const std::vector<Decal *> &get_decals() {
  return _decals;
}

const std::vector<BloodVolumetric *> &get_blood_volumetrics() {
  return _blood_volumetrics;
}

const std::vector<AABB *> &get_aabbs() {
  return _aabbs;
}
} // namespace RenderDataManager
