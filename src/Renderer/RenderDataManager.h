#pragma once

#include "Types/Game/AnimEntity.h"
#include <vector>

namespace RenderDataManager {
void begin_frame();
void update();

void submit_anim_entity(AnimEntity *anim_entity);

const std::vector<AnimEntity *> &get_anim_entities();

} // namespace RenderDataManager
