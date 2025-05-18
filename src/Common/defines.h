#pragma once

#include <glm/glm.hpp>

// Player
// TODO: No up high in the sky, just use loading screen
const float     UP_HIGH_IN_THE_SKY = 200.0f;
const float     PLAYER_HEIGHT      = 1.65f;
const glm::vec3 PLAYER_SPAWN_POS   = glm::vec3(13.0f, UP_HIGH_IN_THE_SKY + PLAYER_HEIGHT, 0.0f);
const float     GRAVITY            = -15.75f;

// Camera
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;
