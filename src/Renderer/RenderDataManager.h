#pragma once

#include "Types/Game/AnimEntity.h"
#include "Types/Game/Decal.h"
#include <vector>

namespace RenderDataManager {
void begin_frame();
void update();

// TODO: Unify with 'submit_render_item'
void submit_anim_entity(AnimEntity *anim_entity);
void submit_decal(Decal *decal);

const std::vector<AnimEntity *> &get_anim_entities();
const std::vector<Decal *>      &get_decals();

} // namespace RenderDataManager
