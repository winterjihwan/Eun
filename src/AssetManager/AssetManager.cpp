#include "AssetManager.h"

#include "Physics/Physics.h"
#include "Renderer/Renderer.h"
#include "Types/Animation/Animation.h"
#include <memory>
#include <vector>

unsigned int load_cubemap(vector<std::string> faces);

namespace AssetManager {
std::vector<Model>                      _models;
std::vector<Mesh>                       _meshes;
std::vector<std::unique_ptr<Animation>> _animations;
std::vector<Animator>                   _animators;

// TODO: Index Map
void init() {
  /* Models */
  // Map
  Model &map = _models.emplace_back("res/objects/Map_v1/Map_v1.obj", "Map");
  for (const Mesh &mesh : map.meshes) {
    Physics::register_static_mesh(mesh.vertices, mesh.indices, glm::mat4(1.0f));
  }

  // Vampire
  Model &vampire = _models.emplace_back("res/objects/Vampire/dancing_vampire.dae", "Vampire");
  std::unique_ptr<Animation> &vampire_animation = _animations.emplace_back(
      std::make_unique<Animation>("res/objects/Vampire/dancing_vampire.dae", &vampire));
  _animators.emplace_back(&*vampire_animation);

  // Knife
  Model &pistol = _models.emplace_back("res/objects/Pistol/scene.gltf", "Pistol");
  _animations.emplace_back(std::make_unique<Animation>("res/objects/Pistol/scene.gltf", &pistol));
  _animators.emplace_back(_animations.back().get());

  // HK_416
  Model &hk_416 = _models.emplace_back("res/objects/HK_416/scene.gltf", "HK_416");
  _animations.emplace_back(std::make_unique<Animation>("res/objects/HK_416/scene.gltf", &hk_416));
  _animators.emplace_back(_animations.back().get());

  /* Objects */

  // Test Sphere
  float                 radius   = 0.5f;
  int                   segments = 64;
  std::vector<Vertex>   sphere_v = Util::generate_sphere_vertices(radius, segments);
  std::vector<uint32_t> sphere_i = Util::generate_sphere_indices(segments);
  std::vector<Texture>  sphere_t = std::vector<Texture>();

  _meshes.emplace_back(sphere_v, sphere_i, sphere_t);

  Renderer::init();
}

std::vector<Animator> &get_animators() {
  return _animators;
}

Animator *get_animator_by_name(const std::string &name) {
  for (auto &animator : _animators) {
    if (name == animator._path) {
      return &animator;
    }
  }

  std::cout << "AssetManager::get_model_by_name() failed, no model with name: " << name
            << std::endl;
  assert(0);
}

std::vector<Mesh> &get_meshes() {
  return _meshes;
}

// Model *get_mesh_by_name(const std::string &name) {
// }

std::vector<Model> &get_models() {
  return _models;
}

Model *get_model_by_name(const std::string &name) {
  for (auto &model : _models) {
    if (name == model.name) {
      return &model;
    }
  }

  std::cout << "AssetManager::get_model_by_name() failed, no model with name: " << name
            << std::endl;
  assert(0);
}
} // namespace AssetManager
