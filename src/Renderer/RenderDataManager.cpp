#include "RenderDataManager.h"

namespace RenderDataManager {
std::vector<AnimEntity *> _anim_entities;

void begin_frame() {
  _anim_entities.clear();
}

void update() {
  // TODO: Draw Indirect Commands
}

void submit_anim_entity(AnimEntity *anim_entity) {
  _anim_entities.push_back(anim_entity);
}

const std::vector<AnimEntity *> &get_anim_entities() {
  return _anim_entities;
}
} // namespace RenderDataManager
