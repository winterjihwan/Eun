#pragma once

#include "Types/Game/AnimEntity.h"
#include "Types/Game/BloodVolumetric.h"
#include "Types/Game/Decal.h"
#include <vector>

namespace RenderDataManager {
void begin_frame();
void update();

// TODO: Unify with 'submit_render_item'
void submit_anim_entity(AnimEntity *anim_entity);
void submit_decal(Decal *decal);
void submit_blood_volumetric(BloodVolumetric *blood_volumetric);
void submit_ui_render_item(UIRenderItem *ui_render_item);

const std::vector<AnimEntity *>      &get_anim_entities();
const std::vector<Decal *>           &get_decals();
const std::vector<BloodVolumetric *> &get_blood_volumetrics();
const std::vector<UIRenderItem *>    &get_ui_render_items();

} // namespace RenderDataManager
