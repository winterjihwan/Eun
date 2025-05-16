#pragma once

#include <glm/glm.hpp>

// Player
const float     UP_HIGH_IN_THE_SKY = 70.0f;
const float     PLAYER_HEIGHT      = 1.8f;
const glm::vec3 PLAYER_SPAWN_POS =
    glm::vec3(9.0f, UP_HIGH_IN_THE_SKY + PLAYER_HEIGHT, 0.0f); // TODO: Remove this hack shit
const float GRAVITY = -15.75f;

// Camera
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;
