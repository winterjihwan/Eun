#pragma once

#include "Enums.h"
#include "Keycodes.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Viewport {
  int *width;
  int *height;
};

struct Transform {
  glm::vec3 position = glm::vec3(0);
  glm::vec3 rotation = glm::vec3(0);
  glm::vec3 scale    = glm::vec3(1);
  glm::mat4 to_mat4() {
    glm::mat4 m = glm::translate(glm::mat4(1), position);
    m *= glm::mat4_cast(glm::quat(rotation));
    m = glm::scale(m, scale);
    return m;
  };
};

struct TransformConfig {
  glm::vec3 translate  = glm::vec3(0.0f);
  glm::vec3 rotate_deg = glm::vec3(0.0f);
  glm::vec3 scale      = glm::vec3(1.0f);
};

// Player
// TODO: One place for control config
struct PlayerControls {
  unsigned int WALK_FORWARD  = EUN_KEY_W;
  unsigned int WALK_BACKWARD = EUN_KEY_S;
  unsigned int WALK_LEFT     = EUN_KEY_A;
  unsigned int WALK_RIGHT    = EUN_KEY_D;
  unsigned int FIRE          = EUN_MOUSE_LEFT;
  unsigned int JUMP          = EUN_KEY_SPACE;
  unsigned int NEXT_WEAPON   = EUN_KEY_Q;
  unsigned int FLASHLIGHT    = EUN_KEY_F;
};

struct PhysicsUserData {
  // TODO: Object UUID
  PhysicsType physics_type = PhysicsType::NONE;
  ObjectType  object_type  = ObjectType::NONE;
  uint64_t    object_id    = 0;
};

struct Vertex {
  glm::vec3 position;
  glm::vec2 tex_coords;
  glm::vec3 normal;
};

struct RayHitInfo {
  const JPH::Body      *body;
  PhysicsUserData      *user_data;
  JPH::TransformedShape ts;
  JPH::Vec3             hit_pos;
  JPH::Vec3             hit_normal;
  JPH::RayCastResult    result;
};

struct Vertex2D {
  glm::vec4 position;
};

struct Mesh2DData {
  std::vector<Vertex2D> vertices;
  unsigned int          texture_id;
};

struct UIRenderItem {
  int          base_vertex  = 0;
  unsigned int vertex_count = 0;
  unsigned int texture_id;
};

struct Character {
  unsigned int texture_id;
  glm::ivec2   size;
  glm::ivec2   bearing;
  unsigned int advance;
};
