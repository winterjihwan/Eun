#include "AssetManager.h"

#include "Physics/Physics.h"
#include "Renderer/Renderer.h"
#include "Types/Animation/Animation.h"
#include "World/World.h"
#include <memory>
#include <vector>

unsigned int load_cubemap(vector<std::string> faces);

namespace AssetManager {
std::vector<Model>                      _models;
std::vector<Mesh>                       _meshes;
std::vector<std::shared_ptr<Animation>> _animations;
std::vector<Animator>                   _animators;
std::vector<Ref<Ragdoll>>               _ragdolls;

// TODO: Index Map
void init() {
  /* Models */
  // Map
  Model &map = _models.emplace_back("res/objects/Map_v1/Map_v1.obj", "Map");
  for (const Mesh &mesh : map.meshes) {
    Physics::register_static_mesh(mesh.vertices, mesh.indices, glm::mat4(1.0f));
  }

  // Brian
  {
    Model &brian = _models.emplace_back("res/objects/Brian/Idle.dae", "Brian");

    // for (const Mesh &mesh : brian.meshes) {
    //   Physics::register_convex_dynamic_body(mesh.vertices, mesh.indices, glm::mat4(1.0f));
    // }

    std::shared_ptr<Animation> &brian_idle_animation =
        _animations.emplace_back(std::make_shared<Animation>("res/objects/Brian/Idle.dae", &brian));
    _animators.emplace_back(&*brian_idle_animation, "Brian_Idle");

    std::shared_ptr<Animation> &brian_walk_animation =
        _animations.emplace_back(std::make_shared<Animation>("res/objects/Brian/Walk.dae", &brian));
    _animators.emplace_back(&*brian_walk_animation, "Brian_Walk");

    std::shared_ptr<Animation> &brian_death_animation = _animations.emplace_back(
        std::make_shared<Animation>("res/objects/Brian/Death.dae", &brian));
    _animators.emplace_back(&*brian_death_animation, "Brian_Death");
  }

  // Pistol
  {
    Model &pistol = _models.emplace_back("res/objects/Pistol/scene.gltf", "Pistol");
    std::shared_ptr<Animation> &pistol_animation = _animations.emplace_back(
        std::make_shared<Animation>("res/objects/Pistol/scene.gltf", &pistol));

    _animators.emplace_back(&*pistol_animation, "Pistol_Draw");
    Animator *pistol_draw_animator = get_animator_by_name("Pistol_Draw");
    pistol_draw_animator->SetClip(0.0f, 1.14f);
    pistol_draw_animator->SetIsLoop(false);

    _animators.emplace_back(&*pistol_animation, "Pistol_Idle");
    Animator *pistol_idle_animator = get_animator_by_name("Pistol_Idle");
    pistol_idle_animator->SetClip(10.9f, 11.2f);
    pistol_idle_animator->SetIsLoop(false);

    _animators.emplace_back(&*pistol_animation, "Pistol_Fire");
    Animator *pistol_fire_animator = get_animator_by_name("Pistol_Fire");
    pistol_fire_animator->SetClip(7.5f, 8.0f);
    pistol_fire_animator->SetIsLoop(false);

    _animators.emplace_back(&*pistol_animation, "Pistol_Reload");
    Animator *pistol_reload_animator = get_animator_by_name("Pistol_Reload");
    pistol_reload_animator->SetClip(8.2f, 10.8f);
    pistol_reload_animator->SetIsLoop(false);

    _animators.emplace_back(&*pistol_animation, "Pistol_Inspect");
    Animator *pistol_inspect_animator = get_animator_by_name("Pistol_Inspect");
    pistol_inspect_animator->SetClip(1.5f, 7.0f);
    pistol_inspect_animator->SetIsLoop(false);
  }

  // HK_416
  {
    Model &hk_416 = _models.emplace_back("res/objects/HK_416/scene.gltf", "HK_416");
    std::shared_ptr<Animation> &hk_416_animation = _animations.emplace_back(
        std::make_shared<Animation>("res/objects/HK_416/scene.gltf", &hk_416));

    _animators.emplace_back(&*hk_416_animation, "HK_416_Draw");
    Animator *hk_416_draw_animator = get_animator_by_name("HK_416_Draw");
    hk_416_draw_animator->SetClip(0.0f, 2.2f);
    hk_416_draw_animator->SetIsLoop(false);

    _animators.emplace_back(&*hk_416_animation, "HK_416_Idle");
    Animator *hk_416_idle_animator = get_animator_by_name("HK_416_Idle");
    hk_416_idle_animator->SetClip(8.0f, 8.2f);
    hk_416_idle_animator->SetIsLoop(false);

    _animators.emplace_back(&*hk_416_animation, "HK_416_Inspect");
    Animator *hk_416_inspect_animator = get_animator_by_name("HK_416_Inspect");
    hk_416_inspect_animator->SetClip(2.2f, 8.0f);
    hk_416_inspect_animator->SetIsLoop(false);

    _animators.emplace_back(&*hk_416_animation, "HK_416_Fire");
    Animator *hk_416_fire_animator = get_animator_by_name("HK_416_Fire");
    hk_416_fire_animator->SetClip(21.96f, 22.36f);
    hk_416_fire_animator->SetIsLoop(false);

    _animators.emplace_back(&*hk_416_animation, "HK_416_Reload");
    Animator *hk_416_reload_animator = get_animator_by_name("HK_416_Reload");
    hk_416_reload_animator->SetClip(8.25f, 14.5f);
    hk_416_reload_animator->SetIsLoop(false);
  }

  /* Objects */

  // Test Sphere
  float                 radius   = 0.5f;
  int                   segments = 64;
  std::vector<Vertex>   sphere_v = Util::generate_sphere_vertices(radius, segments);
  std::vector<uint32_t> sphere_i = Util::generate_sphere_indices(segments);
  std::vector<Texture>  sphere_t = std::vector<Texture>();

  _meshes.emplace_back(sphere_v, sphere_i, sphere_t, "Test_Sphere");

  World::init();
  Renderer::init();
}

void shutdown() {
  _ragdolls.clear();
}

std::vector<Animator> &get_animators() {
  return _animators;
}

Animator *get_animator_by_name(const std::string &name) {
  for (auto &animator : _animators) {
    if (name == animator.GetName()) {
      return &animator;
    }
  }

  std::cout << "AssetManager::get_animator_by_name() failed, no animator with name: " << name
            << std::endl;
  assert(0);
}

std::vector<Mesh> &get_meshes() {
  return _meshes;
}

Mesh *get_mesh_by_name(const std::string &name) {
  for (auto &mesh : _meshes) {
    if (name == mesh.name) {
      return &mesh;
    }
  }

  std::cout << "AssetManager::get_mesh_by_name() failed, no mesh with name: " << name << std::endl;
  assert(0);
}

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

std::vector<Ref<Ragdoll>> &get_ragdolls() {
  return _ragdolls;
}

} // namespace AssetManager
