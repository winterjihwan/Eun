#pragma once
#include "Keycodes.h"

// Player
// TODO: One place for control config
struct PlayerControls {
  unsigned int WALK_FORWARD  = EUN_KEY_W;
  unsigned int WALK_BACKWARD = EUN_KEY_S;
  unsigned int WALK_LEFT     = EUN_KEY_A;
  unsigned int WALK_RIGHT    = EUN_KEY_D;
  unsigned int RELOAD        = EUN_KEY_R;
  unsigned int FIRE          = EUN_MOUSE_LEFT;
  unsigned int JUMP          = EUN_KEY_SPACE;
  unsigned int NEXT_WEAPON   = EUN_KEY_Q;
  unsigned int FLASHLIGHT    = EUN_KEY_F;
};
