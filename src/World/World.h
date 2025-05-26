#pragma once

#include "Types/Game/AnimEntity.h"

namespace World {
void init();
void begin_frame();
void update(float delta_time);
void submit_render_items();

AnimEntity *get_anim_entity_by_name(const std::string &name);

} // namespace World
