#pragma once

#include "Types/Animation/Animation.h"
#include <vector>
namespace World {
void init();
void begin_frame();
void update();
void submit_render_items();

std::vector<Animation> &get_animation_transforms();

} // namespace World
