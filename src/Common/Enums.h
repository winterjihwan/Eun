#pragma once

enum class GameMode {
  TPS = 0,
  RTS,
  EDITOR,
};

enum class WeaponAction {
  IDLE = 0,
  DRAW,
  STAB,
  FIRE,
  RELOAD,
};

enum class DecalType {
  GLASS = 0,
  SCRATCH,
  PLASTER,
  BLOOD,
};

enum class NpcState {
  IDLE = 0,
  // DEATH,
};

enum class PhysicsType {
  NONE = 0,
  RIGID_DYNAMIC,
  RIGID_STATIC,
  UNDEFINED,
};

enum class ObjectType {
  NONE = 0,
  PLAYER,
  MAP,
  PLATFORM,
  ENHANCE,
  GAME_OBJECT,
  UNIT,
  BUILDING,
  DECAL,
};

enum class TextureType {
  DIFFUSE = 0,
  SPECULAR,
  NORMAL,
  HEIGHT,
};

enum class UIAlignment {
  CENTERED = 0,
  CENTERED_HORIZONTAL,
  CENTERED_VERTICAL,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT
};

enum class ViewMode {
  FIRST = 0,
  THIRD,
};

enum class PlayerState {
  IDLE = 0,
  WALKING_FORWARD,
  WALKING_BACKWARD,
  WALKING_LEFT,
  WALKING_RIGHT,
  JUMPING,
  STAB,
};

enum class EditorState {
  IDLE,
};
