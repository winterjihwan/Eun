#pragma once

enum class WeaponAction {
  IDLE = 0,
  DRAW,
  FIRE,
  RELOAD,
  INSPECT,
};

enum class NpcAnimationState {
  IDLE = 0,
  WALK,
  DEATH,
};
