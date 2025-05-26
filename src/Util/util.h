#include "Mesh.h"

#include <Jolt/Jolt.h>
#include <vector>

namespace Util {
std::vector<Vertex>   generate_sphere_vertices(float radius, int segments);
std::vector<uint32_t> generate_sphere_indices(int segments);
std::vector<Vertex>
generate_capsule_vertices(float radius, float halfHeight, unsigned int segments);
std::vector<uint32_t> generate_capsule_indices(unsigned int segments);

JPH::Vec3 to_jolt_vec3(const glm::vec3 &v);

} // namespace Util
