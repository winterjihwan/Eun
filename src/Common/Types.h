#pragma once
#include "Defines.h"
#include "Enums.h"
#include "Keycodes.h"
#include <glm/glm.hpp>
#include <string>

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

struct PhysicsUserData {
  // TODO: Object UUID
  PhysicsType physics_type = PhysicsType::NONE;
  ObjectType  object_type  = ObjectType::NONE;
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texcoords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  int       m_BoneIDs[MAX_BONE_INFLUENCE];
  float     m_Weights[MAX_BONE_INFLUENCE];
};

// TODO: Replace with Texture class
struct Texture_t {
  unsigned int id;
  TextureType  type;
  std::string  path;
};
