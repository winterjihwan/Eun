#include "AssetManager.h"

#include "Core/Game.h"
#include "Types/Animation/Animation.h"
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
std::vector<SkinnedModel> _skinned_models;
std::vector<Mesh>         _meshes;
std::vector<Animation>    _animations;
std::vector<Texture>      _textures;
std::vector<ExrTexture>   _exr_textures;
std::vector<DeferredTask> _deferred_tasks;
bool                      _loading_complete = false;

// TODO: Index Map
void init() {
  // Reserve
  _models.reserve(16);
  _skinned_models.reserve(16);
  _meshes.reserve(16);
  _animations.reserve(16);
  _textures.reserve(64);
  _exr_textures.reserve(36);
  _deferred_tasks.reserve(16);

  // G_Buffer
  {
    std::vector<Vertex>   quad_v = Util::generate_quad_vertices(2.0f, 2.0f);
    std::vector<uint32_t> quad_i = Util::generate_quad_indices();

    _meshes.emplace_back(quad_v, quad_i, nullptr, "G_Buffer");
  }

  // Editor
  {
    std::vector<Vertex>   quad_v = Util::generate_quad_vertices(1.0f, 1.0f);
    std::vector<uint32_t> quad_i = Util::generate_quad_indices();

    _meshes.emplace_back(quad_v, quad_i, nullptr, "Mesh");
  }

  // Plane
  {
    std::vector<Vertex>   quad_v = Util::generate_quad_vertices(30.0f, 30.0f);
    std::vector<uint32_t> quad_i = Util::generate_quad_indices();
    Texture              *laminate_floor =
        &_textures.emplace_back(GL_TEXTURE_2D, "res/textures/Laminate_Floor.jpg", "Laminate_Floor");

    _meshes.emplace_back(quad_v, quad_i, laminate_floor, "Plane");
  }

  // Door HACK
  {
    std::vector<Vertex>   quad_v = Util::generate_quad_vertices(1.0f, 1.0f);
    std::vector<uint32_t> quad_i = Util::generate_quad_indices();
    Texture *marble = &_textures.emplace_back(GL_TEXTURE_2D, "res/textures/marble.jpg", "Marble");

    _meshes.emplace_back(quad_v, quad_i, marble, "Door");
  }

  // Weapon View
  {
    _skinned_models.emplace_back("Knife_View", "res/objects/Knife/Knife.fbx");
    _skinned_models.emplace_back("Glock_View", "res/objects/Glock/Glock.fbx");
  }

  // Mannequin
  {
    _skinned_models.emplace_back("Mannequin", "res/objects/Mannequin/Mannequin_Idle.fbx");
    Animation &mannequin_idle = _animations.emplace_back();
    mannequin_idle.init("Mannequin_Idle", "res/objects/Mannequin/Mannequin_Idle.fbx");
  }

  // Knife Animations
  {
    Animation &knife_swing_0 = _animations.emplace_back();
    knife_swing_0.init("Knife_Swing0", "res/animations/Knife/Knife_Swing0.fbx");

    Animation &knife_swing_1 = _animations.emplace_back();
    knife_swing_1.init("Knife_Swing1", "res/animations/Knife/Knife_Swing1.fbx");

    Animation &knife_swing_2 = _animations.emplace_back();
    knife_swing_2.init("Knife_Swing2", "res/animations/Knife//Knife_Swing2.fbx");

    Animation &knife_idle = _animations.emplace_back();
    knife_idle.init("Knife_Idle", "res/animations/Knife/Knife_Idle.fbx");

    Animation &knife_draw = _animations.emplace_back();
    knife_draw.init("Knife_Draw", "res/animations/Knife/Knife_Draw.fbx");
  }

  // Glock Animations
  {
    Animation &glock_draw = _animations.emplace_back();
    glock_draw.init("Glock_Draw", "res/animations/Glock/Glock_Draw.fbx");

    Animation &glock_idle = _animations.emplace_back();
    glock_idle.init("Glock_Idle", "res/animations/Glock/Glock_Idle.fbx");

    Animation &glock_fire_0 = _animations.emplace_back();
    glock_fire_0.init("Glock_Fire0", "res/animations/Glock/Glock_Fire0.fbx");

    Animation &glock_fire_1 = _animations.emplace_back();
    glock_fire_1.init("Glock_Fire1", "res/animations/Glock/Glock_Fire1.fbx");

    Animation &glock_fire_2 = _animations.emplace_back();
    glock_fire_2.init("Glock_Fire2", "res/animations/Glock/Glock_Fire2.fbx");

    Animation &glock_reload = _animations.emplace_back();
    glock_reload.init("Glock_Reload", "res/animations/Glock/Glock_Reload.fbx");
  }

  // Decal
  {
    // Bullet Hole
    {
      std::vector<Vertex>   quad_v = Util::generate_quad_vertices(0.3f, 0.3f);
      std::vector<uint32_t> quad_i = Util::generate_quad_indices();
      Texture              *bullet_hole =
          &_textures.emplace_back(GL_TEXTURE_2D, "res/textures/Bullet_Hole.png", "Bullet_Hole");
      _meshes.emplace_back(quad_v, quad_i, bullet_hole, "Bullet_Hole");
    }

    // Bullet Hole
    {
      std::vector<Vertex>   quad_v = Util::generate_quad_vertices(0.2f, 0.2f);
      std::vector<uint32_t> quad_i = Util::generate_quad_indices();
      Texture              *knife_scratch =
          &_textures.emplace_back(GL_TEXTURE_2D, "res/textures/Knife_Scratch.png", "Knife_Scratch");
      _meshes.emplace_back(quad_v, quad_i, knife_scratch, "Knife_Scratch");
    }

    // Blood decal
    {
      std::vector<Vertex>   quad_v        = Util::generate_quad_vertices(1.0f, 1.0f);
      std::vector<uint32_t> quad_i        = Util::generate_quad_indices();
      Texture              *blood_stain_1 = &_textures.emplace_back(
          GL_TEXTURE_2D, "res/textures/Blood/blood_decal_1.png", "Blood_Stain_1");
      _meshes.emplace_back(quad_v, quad_i, blood_stain_1, "Blood_Stain_1");

      Texture *blood_stain_2 = &_textures.emplace_back(
          GL_TEXTURE_2D, "res/textures/Blood/blood_decal_2.png", "Blood_Stain_2");
      _meshes.emplace_back(quad_v, quad_i, blood_stain_2, "Blood_Stain_2");

      Texture *blood_stain_3 = &_textures.emplace_back(
          GL_TEXTURE_2D, "res/textures/Blood/blood_decal_3.png", "Blood_Stain_3");
      _meshes.emplace_back(quad_v, quad_i, blood_stain_3, "Blood_Stain_3");

      Texture *blood_stain_4 = &_textures.emplace_back(
          GL_TEXTURE_2D, "res/textures/Blood/blood_decal_4.png", "Blood_Stain_4");
      _meshes.emplace_back(quad_v, quad_i, blood_stain_4, "Blood_Stain_4");
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
      _models.emplace_back(blood_model_names[i], blood_model_paths[i]);
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
    for (Texture &texture : _textures) {
      texture.load();
    }

    for (Mesh &mesh : _meshes) {
      mesh.upload_to_gpu();
    }

    for (Model &model : _models) {
      model.load();
    }

    for (SkinnedModel &skinned_model : _skinned_models) {
      skinned_model.load();
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

Model *get_model_by_name(const std::string &name) {
  for (Model &model : _models) {
    if (name == model.get_name()) {
      return &model;
    }
  }

  std::cout << "AssetManager::get_model_by_name() failed, no model with name: " << name
            << std::endl;
  assert(0);
}

Model *get_model_by_index(int index) {
  if (index >= 0 && index < _models.size()) {
    return &_models[index];
  } else {
    std::cout << "AssetManager::get_model_by_index() failed, no model with name: " << index
              << std::endl;
    assert(0);
  }
}

SkinnedModel *get_skinned_model_by_name(const std::string &name) {
  for (SkinnedModel &skinned_model : _skinned_models) {
    if (name == skinned_model.get_name()) {
      return &skinned_model;
    }
  }

  std::cout << "AssetManager::get_skinned_model_by_name() failed, no skinned model with name: "
            << name << std::endl;
  assert(0);
}

SkinnedModel *get_skinned_model_by_index(int index) {
  if (index >= 0 && index < _skinned_models.size()) {
    return &_skinned_models[index];
  } else {
    std::cout << "AssetManager::get_skinned_model_by_index() failed, no skinned model with name: "
              << index << std::endl;
    assert(0);
  }
}

int get_skinned_model_index_by_name(const std::string &name) {
  for (int i = 0; i < _skinned_models.size(); i++) {
    if (_skinned_models[i].get_name() == name) {
      return i;
    }
  }

  std::cout
      << "AssetManager::get_skinned_model_index_by_name() failed, no skinned model with name: "
      << name << std::endl;
  assert(0);
}

Animation *get_animation_by_name(const std::string &name) {
  for (Animation &animation : _animations) {
    if (animation.get_name() == name) {
      return &animation;
    }
  }

  std::cout << "AssetManager::get_animation_by_name() failed, no animation with name: " << name
            << std::endl;
  assert(0);
}

Animation *get_animation_by_index(int index) {
  if (index >= 0 && index < _animations.size()) {
    return &_animations[index];
  } else {
    std::cout << "AssetManager::get_animation_model_by_index() failed, no animation with name: "
              << index << std::endl;
    assert(0);
  }
}

int get_animation_index_by_name(const std::string &name) {
  for (int i = 0; i < _animations.size(); i++) {
    if (_animations[i].get_name() == name) {
      return i;
    }
  }

  std::cout << "AssetManager::get_animation_index_by_name() failed, no animation with name: "
            << name << std::endl;
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

Texture *get_texture_by_name(const std::string &name) {
  for (Texture &texture : _textures) {
    if (name == texture.get_name()) {
      return &texture;
    }
  }

  std::cout << "AssetManager::get_texture_by_name() failed, no texture with name: " << name
            << std::endl;
  assert(0);
}

ExrTexture *get_exr_texture_by_name(const std::string &name) {
  for (ExrTexture &texture : _exr_textures) {
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
