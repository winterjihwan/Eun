#include "RenderDataManager.h"

namespace RenderDataManager {
std::vector<AnimEntity *> _anim_entities;
std::vector<Decal *>      _decals;

void begin_frame() {
  _anim_entities.clear();
  _decals.clear();
}

void update() {
  // TODO: Draw Indirect Commands
}

void submit_anim_entity(AnimEntity *anim_entity) {
  _anim_entities.push_back(anim_entity);
}

void submit_decal(Decal *decal) {
  _decals.push_back(decal);
}

const std::vector<AnimEntity *> &get_anim_entities() {
  return _anim_entities;
}

const std::vector<Decal *> &get_decals() {
  return _decals;
}
} // namespace RenderDataManager
