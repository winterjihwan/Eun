#include "Util.h"

namespace Util {
std::vector<Vertex> generate_sphere_vertices(float radius, int segments) {
  std::vector<Vertex> vertices;
  // Ensure segments are reasonable
  segments = std::max(segments, 4);
  // Angular step sizes
  float thetaStep = glm::two_pi<float>() / segments; // Longitude step
  float phiStep   = glm::pi<float>() / segments;     // Latitude step
  for (int i = 0; i <= segments; ++i) {              // Latitude loop
    float phi = i * phiStep;                         // Latitude angle
    for (int j = 0; j <= segments; ++j) {            // Longitude loop
      float theta = j * thetaStep;                   // Longitude angle
      // Calculate position on the sphere
      glm::vec3 position = glm::vec3(
          radius * sin(phi) * cos(theta), radius * cos(phi), radius * sin(phi) * sin(theta));
      // Calculate normal (direction from sphere center)
      glm::vec3 normal = glm::normalize(position);
      // Calculate tangent (partial derivative with respect to theta)
      glm::vec3 tangent = glm::normalize(
          glm::vec3(-radius * sin(phi) * sin(theta), 0.0f, radius * sin(phi) * cos(theta)));
      // Add vertex to the vector
      Vertex &vertex  = vertices.emplace_back();
      vertex.position = position;
      vertex.normal   = normal;
      vertex.tangent  = tangent;
    }
  }
  return vertices;
}

std::vector<uint32_t> generate_sphere_indices(int segments) {
  std::vector<uint32_t> indices;
  for (int i = 0; i < segments; ++i) {   // Latitude loop
    for (int j = 0; j < segments; ++j) { // Longitude loop
      int nextI = i + 1;
      int nextJ = (j + 1) % (segments + 1);
      // Indices of the quad
      uint32_t v0 = i * (segments + 1) + j;
      uint32_t v1 = nextI * (segments + 1) + j;
      uint32_t v2 = i * (segments + 1) + nextJ;
      uint32_t v3 = nextI * (segments + 1) + nextJ;
      // First triangle
      indices.push_back(v2);
      indices.push_back(v1);
      indices.push_back(v0);
      // Second triangle
      indices.push_back(v3);
      indices.push_back(v1);
      indices.push_back(v2);
    }
  }
  return indices;
}
} // namespace Util
