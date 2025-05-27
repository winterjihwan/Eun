#pragma once

#include "Model.h"
#include "Texture.h"
#include "Types/Animation/Animator.h"
#include "Util/Util.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <vector>

namespace AssetManager {
void init();
void shutdown();

std::vector<Animator> &get_animators();
Animator              *get_animator_by_name(const std::string &name);

std::vector<Mesh> &get_meshes();
Mesh              *get_mesh_by_name(const std::string &name);

std::vector<Model> &get_models();
Model              *get_model_by_name(const std::string &name);

Texture *get_texture_by_name(const std::string &name);
} // namespace AssetManager
