#include "AssetManager.h"

#include "Core/Game.h"
#include "Physics/Physics.h"
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
std::vector<Texture>      _textures; // TODO: Remove Texture from Mesh
std::vector<ExrTexture>   _exr_textures;
std::vector<DeferredTask> _deferred_tasks;
bool                      _loading_complete = false;

// TODO: Index Map
void init() {
  // Reserve
  _models.reserve(16);
  _animations.reserve(16);
  _textures.reserve(64);
  _exr_textures.reserve(36);

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
    brian.load("res/objects/Brian/Brian.gltf");

    Model                &brian_knife = _models.emplace_back("Brian_Knife");
    std::future<void>     future   = brian_knife.load_async("res/objects/Brian/Brian_Knife.gltf");
    std::function<void()> callback = [&brian_knife]() {
      _animations.emplace_back("Idle", "res/animations/Brian_Idle_Knife.gltf", &brian_knife);
      _animations.emplace_back("Idle_Knife", "res/animations/Brian_Idle_Knife.gltf", &brian_knife);
      _animations.emplace_back("Walk", "res/animations/Brian_Walk.gltf", &brian_knife);
      _animations.emplace_back("Stab", "res/animations/Brian_Stab.gltf", &brian_knife);
    };
    _deferred_tasks.push_back({std::move(future), callback});
  }

  // Knife
  {
    Model &knife = _models.emplace_back("Knife");

    std::future<void> future = knife.load_async("res/objects/Knife/scene.gltf");
    _deferred_tasks.push_back({std::move(future), 0});
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
      Model &model = _models.emplace_back(blood_model_names[i]);

      std::future<void> future = model.load_async(blood_model_paths[i]);
      _deferred_tasks.push_back({std::move(future), 0});
    }

    std::vector<std::string> exr_paths = {
        "res/textures/Blood/blood_pos1.exr",
        "res/textures/Blood/blood_pos2.exr",
        "res/textures/Blood/blood_pos3.exr",
        "res/textures/Blood/blood_pos4.exr",
        "res/textures/Blood/blood_pos5.exr",
        "res/textures/Blood/blood_pos6.exr",
        "res/textures/Blood/blood_norm1.exr",
        "res/textures/Blood/blood_norm2.exr",
        "res/textures/Blood/blood_norm3.exr",
        "res/textures/Blood/blood_norm4.exr",
        "res/textures/Blood/blood_norm5.exr",
        "res/textures/Blood/blood_norm6.exr",
    };

    std::vector<std::string> exr_names = {
        "blood_pos_1",
        "blood_pos_2",
        "blood_pos_3",
        "blood_pos_4",
        "blood_pos_5",
        "blood_pos_6",
        "blood_norm_1",
        "blood_norm_2",
        "blood_norm_3",
        "blood_norm_4",
        "blood_norm_5",
        "blood_norm_6",
    };

    for (size_t i = 0; i < exr_paths.size(); ++i) {
      std::string path = exr_paths[i];
      std::string name = exr_names[i];

      ExrTexture       &exr_texture = _exr_textures.emplace_back(name);
      std::future<void> future      = exr_texture.load_async(path);
      _deferred_tasks.push_back({std::move(future), 0});
    }
  }
}

void update_loading() {
  UIBackend::blit_text(
      "Eunnnnnn", "AncizarSerif", 0, VIEWPORT_HEIGHT - 32, UIAlignment::CENTERED, 1.0f);

  if (_loading_complete) {
    std::cout << "AssetManager::update_loading(), Why update loading again?" << std::endl;
    assert(0);
  }

  for (DeferredTask &task : _deferred_tasks) {
    if (task.future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      return;
    }
  }

  _loading_complete = true;

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

    for (ExrTexture &texture : _exr_textures) {
      texture.upload_to_gpu();
    }

    World::init();
    Game::init();
  }
}

void shutdown() {
}

Animation *get_animation_by_name(const std::string &name) {
  for (Animation &animation : _animations) {
    if (name == animation._name) {
      return &animation;
    }
  }

  std::cout << "AssetManager::get_animation_by_name() failed, no animation with name: " << name
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
    if (name == texture.get_name()) {
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
