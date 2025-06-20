#pragma once

#include <glm/glm.hpp>

// General
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// MATH
constexpr float EUN_PI = 3.141592653589793f;

// Game
constexpr unsigned int VIEWPORT_WIDTH  = 1280;
constexpr unsigned int VIEWPORT_HEIGHT = 800;
constexpr float        NEAR            = 0.3f;
constexpr float        FAR             = 100.0f;

// Player
const float     PLAYER_HEIGHT    = 1.65f;
const glm::vec3 PLAYER_SPAWN_POS = glm::vec3(0.0f, PLAYER_HEIGHT, 0.0f);
const float     GRAVITY          = -15.75f;

// Camera
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

// Audio
#define AUDIO_BASE_PATH "res/audio/"

// Animation
#define MAX_BONE_INFLUENCE 8

// Material
#define INVALID_MATERIAL 0xFFFFFFFF

//  Model
#define POSITION_LOCATION  0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION    2

#define COLOR_TEXTURE_UNIT                          GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX                    0
#define SHADOW_TEXTURE_UNIT                         GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX                   1
#define NORMAL_TEXTURE_UNIT                         GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX                   2
#define RANDOM_TEXTURE_UNIT                         GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX                   3
#define DISPLACEMENT_TEXTURE_UNIT                   GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX             4
#define ALBEDO_TEXTURE_UNIT                         GL_TEXTURE5
#define ALBEDO_TEXTURE_UNIT_INDEX                   5
#define ROUGHNESS_TEXTURE_UNIT                      GL_TEXTURE6
#define ROUGHNESS_TEXTURE_UNIT_INDEX                6
#define MOTION_TEXTURE_UNIT                         GL_TEXTURE7
#define MOTION_TEXTURE_UNIT_INDEX                   7
#define SPECULAR_EXPONENT_UNIT                      GL_TEXTURE8
#define SPECULAR_EXPONENT_UNIT_INDEX                8
#define CASCACDE_SHADOW_TEXTURE_UNIT0               SHADOW_TEXTURE_UNIT
#define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX         SHADOW_TEXTURE_UNIT_INDEX
#define CASCACDE_SHADOW_TEXTURE_UNIT1               GL_TEXTURE9
#define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX         9
#define CASCACDE_SHADOW_TEXTURE_UNIT2               GL_TEXTURE10
#define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX         10
#define SHADOW_CUBE_MAP_TEXTURE_UNIT                GL_TEXTURE11
#define SHADOW_CUBE_MAP_TEXTURE_UNIT_INDEX          11
#define SHADOW_MAP_RANDOM_OFFSET_TEXTURE_UNIT       GL_TEXTURE12
#define SHADOW_MAP_RANDOM_OFFSET_TEXTURE_UNIT_INDEX 12
#define METALLIC_TEXTURE_UNIT                       GL_TEXTURE14
#define METALLIC_TEXTURE_UNIT_INDEX                 14
#define HEIGHT_TEXTURE_UNIT                         GL_TEXTURE15
#define HEIGHT_TEXTURE_UNIT_INDEX                   15

// Animation
#define MAX_NUM_BONES_PER_VERTEX 4
#define BONE_ID_LOCATION         3
#define BONE_WEIGHT_LOCATION     4
#define MAX_BONES                (200)

// Assimp
#define ASSIMP_LOAD_FLAGS_GEOMETRY                                                                 \
  (aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals |          \
   aiProcess_LimitBoneWeights | aiProcess_SplitLargeMeshes | aiProcess_ImproveCacheLocality |      \
   aiProcess_RemoveRedundantMaterials | aiProcess_FindDegenerates | aiProcess_FindInvalidData |    \
   aiProcess_GenUVCoords | aiProcess_CalcTangentSpace | aiProcess_FlipUVs)
#define ASSIMP_LOAD_FLAGS_ANIMATION aiProcess_ValidateDataStructure
