#pragma once

#include "Enums.h"
#include "Player/Player.h"

namespace Game {
void     init();
void     update();
void     begin_frame();
void     set_game_mode(GameMode game_mode);
bool     is_game_mode(GameMode game_mode);
GameMode get_game_mode();
float    get_delta_time();
Player  *get_player();
Camera  *get_camera();
} // namespace Game
