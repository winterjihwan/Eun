#pragma once

#include "Player/Player.h"

namespace Game {
void    init();
void    update();
void    begin_frame();
float   get_delta_time();
Player *get_player();
Camera *get_camera();
} // namespace Game
