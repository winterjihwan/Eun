#include "AssetManager.h"

#include "Core/Game.h"
#include "Physics/Physics.h"
#include "Renderer/Renderer.h"
#include "Types/Animation/Animation.h"
#include "Types/Texture/ExrTexture.h"
#include "UI/UIBackend.h"
#include "World/World.h"
#include <future>
#include <vector>

unsigned int load_cubemap(std::vector<std::string> faces);

struct DeferredTask {
  std::future<void>     future;
  std::function<void()> callback;
};

namespace AssetManager {
std::vector<Model>        _models;
std::vector<Mesh>         _meshes;
std::vector<Animation>    _animations;
std::vector<Animator>     _animators;
std::vector<Texture>      _textures; // TODO: Remove Texture from Mesh
std::vector<ExrTexture>   _exr_textures;
std::vector<DeferredTask> _deferred_tasks;
bool                      _loading_complete = false;

// TODO: Index Map
void init() {
  // Reserve
  _models.reserve(16);
  _animations.reserve(16);
  _animators.reserve(32);
  _textures.reserve(64);

  // Map
  {
    Model                &map      = _models.emplace_back("Map");
    std::future<void>     future   = map.load_async("res/objects/Map_v1/Map_v1.obj");
    std::function<void()> callback = [&map]() {
      for (const Mesh &mesh : map.meshes) {
        Physics::register_static_mesh(mesh.vertices, mesh.indices, glm::mat4(1.0f));
      }
    };
    _deferred_tasks.push_back({std::move(future), callback});
  }

  // Brian
  {
    Model &brian = _models.emplace_back("Brian");

    std::future<void>     future   = brian.load_async("res/objects/Brian/brian.dae");
    std::function<void()> callback = [&brian]() {
      // Animations
      Animation &brian_idle_animation =
          _animations.emplace_back(Animation("res/animations/Breath_Idle.dae", &brian));
      _animators.emplace_back(&brian_idle_animation, "Brian_Idle");
    };
    _deferred_tasks.push_back({std::move(future), callback});
  }

  // Glock
  {
    Model &glock = _models.emplace_back("Glock");

    std::future<void> future = glock.load_async("res/objects/Glock/Glock.obj");
    _deferred_tasks.push_back({std::move(future), 0});
  }

  // Pete
  {
    Model &pete = _models.emplace_back("Pete");

    std::future<void>     future   = pete.load_async("res/objects/Pete/Pete.dae");
    std::function<void()> callback = [&pete]() {
      // Animations
      Animation &brian_idle_animation =
          _animations.emplace_back(Animation("res/animations/Gun_Idle.dae", &pete));
      _animators.emplace_back(&brian_idle_animation, "Pete_Idle");
    };
    _deferred_tasks.push_back({std::move(future), callback});
  }

  // Decal
  {
    // Bullet Hole
    {
      std::vector<Vertex>   quad_v = Util::generate_quad_vertices(0.3f, 0.3f);
      std::vector<uint32_t> quad_i = Util::generate_quad_indices();
      std::vector<Texture>  quad_t;
      quad_t.emplace_back("Bullet_Hole", "res/textures/Bullet_Hole.png", TextureType::DIFFUSE);

      _meshes.emplace_back(quad_v, quad_i, quad_t, "Bullet_Hole");
    }

    // Blood decal
    {
      std::vector<Vertex>   quad_v = Util::generate_quad_vertices(1.0f, 1.0f);
      std::vector<uint32_t> quad_i = Util::generate_quad_indices();
      std::vector<Texture>  quad_t_1;
      quad_t_1.emplace_back(
          "Blood_Stain_1", "res/textures/Blood/blood_decal_1.png", TextureType::DIFFUSE);
      _meshes.emplace_back(quad_v, quad_i, quad_t_1, "Blood_Stain_1");

      std::vector<Texture> quad_t_2;
      quad_t_2.emplace_back(
          "Blood_Stain_2", "res/textures/Blood/blood_decal_2.png", TextureType::DIFFUSE);
      _meshes.emplace_back(quad_v, quad_i, quad_t_2, "Blood_Stain_2");

      std::vector<Texture> quad_t_3;
      quad_t_3.emplace_back(
          "Blood_Stain_3", "res/textures/Blood/blood_decal_3.png", TextureType::DIFFUSE);
      _meshes.emplace_back(quad_v, quad_i, quad_t_3, "Blood_Stain_3");

      std::vector<Texture> quad_t_4;
      quad_t_4.emplace_back(
          "Blood_Stain_4", "res/textures/Blood/blood_decal_4.png", TextureType::DIFFUSE);
      _meshes.emplace_back(quad_v, quad_i, quad_t_4, "Blood_Stain_4");
    }
  }

  // Volumetric blood
  {
    const std::vector<std::string> blood_model_names = {
        "Blood_1",
        "Blood_2",
        "Blood_3",
        "Blood_4",
        "Blood_5",
        "Blood_6",
    };
    const std::vector<std::string> blood_model_paths = {
        "res/objects/Blood/blood_mesh1.obj",
        "res/objects/Blood/blood_mesh2.obj",
        "res/objects/Blood/blood_mesh3.obj",
        "res/objects/Blood/blood_mesh4.obj",
        "res/objects/Blood/blood_mesh5.obj",
        "res/objects/Blood/blood_mesh6.obj",
    };

    for (size_t i = 0; i < blood_model_names.size(); ++i) {
      Model            &model  = _models.emplace_back(blood_model_names[i]);
      std::future<void> future = model.load_async(blood_model_paths[i]);
      _deferred_tasks.push_back({std::move(future), 0});
    }

    _exr_textures.emplace_back("res/textures/Blood/blood_pos1.exr", "blood_pos_1");
    _exr_textures.emplace_back("res/textures/Blood/blood_pos2.exr", "blood_pos_2");
    _exr_textures.emplace_back("res/textures/Blood/blood_pos3.exr", "blood_pos_3");
    _exr_textures.emplace_back("res/textures/Blood/blood_pos4.exr", "blood_pos_4");
    _exr_textures.emplace_back("res/textures/Blood/blood_pos5.exr", "blood_pos_5");
    _exr_textures.emplace_back("res/textures/Blood/blood_pos6.exr", "blood_pos_6");

    _exr_textures.emplace_back("res/textures/Blood/blood_norm1.exr", "blood_norm_1");
    _exr_textures.emplace_back("res/textures/Blood/blood_norm2.exr", "blood_norm_2");
    _exr_textures.emplace_back("res/textures/Blood/blood_norm3.exr", "blood_norm_3");
    _exr_textures.emplace_back("res/textures/Blood/blood_norm4.exr", "blood_norm_4");
    _exr_textures.emplace_back("res/textures/Blood/blood_norm5.exr", "blood_norm_5");
    _exr_textures.emplace_back("res/textures/Blood/blood_norm6.exr", "blood_norm_6");
  }

  // Pistol
  {
    Model &pistol = _models.emplace_back("Pistol");

    std::future<void>     future   = pistol.load_async("res/objects/Pistol/scene.gltf");
    std::function<void()> callback = [&pistol]() {
      Animation &pistol_animation =
          _animations.emplace_back(Animation("res/objects/Pistol/scene.gltf", &pistol));

      Animator *draw_animator = &_animators.emplace_back(&pistol_animation, "Pistol_Draw");
      draw_animator->SetClip(0.0f, 1.14f);
      draw_animator->SetIsLoop(false);

      Animator *idle_animator = &_animators.emplace_back(&pistol_animation, "Pistol_Idle");
      idle_animator->SetClip(10.9f, 11.2f);
      idle_animator->SetIsLoop(false);

      Animator *fire_animator = &_animators.emplace_back(&pistol_animation, "Pistol_Fire");
      fire_animator->SetClip(7.5f, 8.0f);
      fire_animator->SetIsLoop(false);

      Animator *reload_animator = &_animators.emplace_back(&pistol_animation, "Pistol_Reload");
      reload_animator->SetClip(8.2f, 10.8f);
      reload_animator->SetIsLoop(false);

      Animator *inspect_animator = &_animators.emplace_back(&pistol_animation, "Pistol_Inspect");
      inspect_animator->SetClip(1.5f, 7.0f);
      inspect_animator->SetIsLoop(false);
    };
    _deferred_tasks.push_back({std::move(future), callback});
  }

  // HK_416
  {
    Model &hk_416 = _models.emplace_back("HK_416");

    std::future<void>     future   = hk_416.load_async("res/objects/HK_416/scene.gltf");
    std::function<void()> callback = [&hk_416]() {
      Animation &hk_416_animation =
          _animations.emplace_back(Animation("res/objects/HK_416/scene.gltf", &hk_416));

      Animator *draw_animator = &_animators.emplace_back(&hk_416_animation, "HK_416_Draw");
      draw_animator->SetClip(0.0f, 2.2f);
      draw_animator->SetIsLoop(false);

      Animator *idle_animator = &_animators.emplace_back(&hk_416_animation, "HK_416_Idle");
      idle_animator->SetClip(8.0f, 8.2f);
      idle_animator->SetIsLoop(false);

      Animator *inspect_animator = &_animators.emplace_back(&hk_416_animation, "HK_416_Inspect");
      inspect_animator->SetClip(2.2f, 8.0f);
      inspect_animator->SetIsLoop(false);

      Animator *fire_animator = &_animators.emplace_back(&hk_416_animation, "HK_416_Fire");
      fire_animator->SetClip(21.96f, 22.36f);
      fire_animator->SetIsLoop(false);

      Animator *reload_animator = &_animators.emplace_back(&hk_416_animation, "HK_416_Reload");
      reload_animator->SetClip(8.25f, 14.5f);
      reload_animator->SetIsLoop(false);
    };
    _deferred_tasks.push_back({std::move(future), callback});
  }
}

void update_loading() {
  UIBackend::blit_text("E U N",
                       "NoScary",
                       VIEWPORT_WIDTH / 2 - 8,
                       VIEWPORT_HEIGHT / 2 - 16,
                       UIAlignment::CENTERED,
                       1.0f);

  if (_loading_complete) {
    std::cout << "AssetManager::update_loading(), Why update loading again?" << std::endl;
    assert(0);
  }

  _loading_complete = true;
  for (DeferredTask &task : _deferred_tasks) {
    if (task.future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      _loading_complete = false;
      break;
    }
  }

  if (_loading_complete) {
    for (DeferredTask &task : _deferred_tasks) {
      task.future.get();
      if (task.callback) {
        task.callback();
      }
    }

    // GPU Upload
    for (Model &model : _models) {
      model.upload_to_gpu();
    }

    for (Mesh &mesh : _meshes) {
      mesh.upload_to_gpu();
    }

    for (Texture &texture : _textures) {
      texture.upload_to_gpu();
    }

    World::init();
    Renderer::init();
    Game::init();
  }
}

void shutdown() {
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

Texture *get_texture_by_name(const std::string &name) {
  for (auto &texture : _textures) {
    if (name == texture.name) {
      return &texture;
    }
  }

  std::cout << "AssetManager::get_texture_by_name() failed, no texture with name: " << name
            << std::endl;
  assert(0);
}

ExrTexture *get_exr_texture_by_name(const std::string &name) {
  for (auto &texture : _exr_textures) {
    if (name == texture.name) {
      return &texture;
    }
  }

  std::cout << "AssetManager::get_exr_texture_by_name() failed, no exr texture with name: " << name
            << std::endl;
  assert(0);
}

bool loading_complete() {
  return _loading_complete;
}

} // namespace AssetManager
