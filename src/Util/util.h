#include "Types.h"
#include <Jolt/Jolt.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace Util {
// Types
JPH::Vec3 to_jolt_vec3(const glm::vec3 &v);
glm::vec3 from_jolt_vec3(const JPH::Vec3 &v);

// To String
std::string to_string(TextureType type);

// Math
float     random_float(float min, float max);
glm::vec3 random_dir_in_hemisphere(const glm::vec3 &normal);

// Geometry
std::vector<Vertex>   generate_sphere_vertices(float radius, int segments);
std::vector<uint32_t> generate_sphere_indices(int segments);

std::vector<Vertex>
generate_capsule_vertices(float radius, float halfHeight, unsigned int segments);
std::vector<uint32_t> generate_capsule_indices(unsigned int segments);

std::vector<Vertex>   generate_quad_vertices(float width, float height);
std::vector<uint32_t> generate_quad_indices();

} // namespace Util
