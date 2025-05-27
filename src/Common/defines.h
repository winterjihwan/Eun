#pragma once

#include <glm/glm.hpp>

// Game
const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 800;

// Player
const float     PLAYER_HEIGHT    = 1.65f;
const glm::vec3 PLAYER_SPAWN_POS = glm::vec3(13.0f, PLAYER_HEIGHT, 0.0f);
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
