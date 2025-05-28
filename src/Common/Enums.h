#pragma once

enum class WeaponAction {
  IDLE = 0,
  DRAW,
  FIRE,
  RELOAD,
  INSPECT,
};

enum class DecalType {
  GLASS,
  PLASTER,
  BLOOD,
};

enum class NpcAnimationState {
  IDLE = 0,
  WALK,
  DEATH,
};

enum class PhysicsType {
  NONE = 0,
  RIGID_DYNAMIC,
  RIGID_STATIC,
  UNDEFINED,
};

enum class ObjectType {
  NONE = 0,
  MAP,
  GAME_OBJECT,
  NPC,
  DECAL,
  UNDEFINED,
};

enum class TextureType {
  DIFFUSE = 0,
  SPECULAR,
  NORMAL,
  HEIGHT,
};
