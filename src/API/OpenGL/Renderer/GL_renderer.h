#pragma once

#include "Physics/Physics.h"

namespace OpenGLRenderer {
void init();
void render_game();

inline glm::mat4 ConvertToGLM(const JPH::RMat44 &mat) {
  glm::mat4 result;
  JPH::Vec3 cols[3] = {mat.GetAxisX(), mat.GetAxisY(), mat.GetAxisZ()};
  for (int i = 0; i < 3; ++i) {
    result[0][i] = cols[i].GetX();
    result[1][i] = cols[i].GetY();
    result[2][i] = cols[i].GetZ();
  }
  result[3] = glm::vec4(
      mat.GetTranslation().GetX(), mat.GetTranslation().GetY(), mat.GetTranslation().GetZ(), 1.0f);
  result[0][3] = result[1][3] = result[2][3] = 0.0f;
  return result;
}
} // namespace OpenGLRenderer
