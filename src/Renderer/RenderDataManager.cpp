#include "RenderDataManager.h"
#include "Types.h"

namespace RenderDataManager {
std::vector<AnimEntity *>      _anim_entities;
std::vector<Decal *>           _decals;
std::vector<BloodVolumetric *> _blood_volumetrics;
std::vector<UIRenderItem *>    _ui_render_items;

void begin_frame() {
  // DID YOU FORGET ME
  _anim_entities.clear();
  _decals.clear();
  _blood_volumetrics.clear();
  _ui_render_items.clear();
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

void submit_blood_volumetric(BloodVolumetric *blood_volumetric) {
  _blood_volumetrics.push_back(blood_volumetric);
}

void submit_ui_render_item(UIRenderItem *ui_render_item) {
  _ui_render_items.push_back(ui_render_item);
}

const std::vector<AnimEntity *> &get_anim_entities() {
  return _anim_entities;
}

const std::vector<Decal *> &get_decals() {
  return _decals;
}

const std::vector<BloodVolumetric *> &get_blood_volumetrics() {
  return _blood_volumetrics;
}

const std::vector<UIRenderItem *> &get_ui_render_items() {
  return _ui_render_items;
}
} // namespace RenderDataManager
