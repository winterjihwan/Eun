#include "Mesh.h"
#include <vector>

namespace Util {
std::vector<Vertex>   generate_sphere_vertices(float radius, int segments);
std::vector<uint32_t> generate_sphere_indices(int segments);
} // namespace Util
