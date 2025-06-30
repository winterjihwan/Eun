#include "Types.h"
#include <Jolt/Jolt.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <glm/gtx/euler_angles.hpp>
#include <vector>

namespace Util {
// Types
JPH::Vec3 to_jolt_vec3(const glm::vec3 &v);
JPH::Quat to_jolt_quat(const glm::quat &q);
glm::vec3 to_vec3(const JPH::Vec3 &v);

// File
std::string get_dir_from_filename(const std::string &filename);

// To String
std::string to_string(const std::string &name, const glm::mat4 &mat);
std::string to_string(ObjectType object_type);

// Math
float     random_float(float min, float max);
glm::vec3 random_dir_in_hemisphere(const glm::vec3 &normal);
glm::mat4 lerp(const glm::mat4 &a, const glm::mat4 &b, float factor);
glm::mat4
transform(const glm::vec3 &position, const glm::vec3 &rotation_rad, const glm::vec3 &scale);
glm::vec3
to_capsule_position(const glm::vec3 &base_position, float capsule_height, float capsule_radius);

// Geometry
std::vector<Vertex>   generate_sphere_vertices(float radius, int segments);
std::vector<uint32_t> generate_sphere_indices(int segments);

std::vector<Vertex>
generate_capsule_vertices(float radius, float halfHeight, unsigned int segments);
std::vector<uint32_t> generate_capsule_indices(unsigned int segments);

std::vector<Vertex>   generate_quad_vertices(float width, float height);
std::vector<uint32_t> generate_quad_indices();

// JPH
JPH::ShapeRefC generate_capsule_shape(float capsule_height, float capsule_radius);

// Assimp
glm::mat4 to_mat4(const aiMatrix4x4 &from);
glm::vec3 to_vec3(const aiVector3D &vec);
glm::quat to_quat(const aiQuaternion &orientation);

} // namespace Util
