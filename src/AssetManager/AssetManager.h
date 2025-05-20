#pragma once

#include "Model.h"
#include "Types/Animation/Animator.h"
#include "Util/Util.h"
#include <vector>

namespace AssetManager {
void init();

std::vector<Animator> &get_animators();
Animator              *get_animator_by_name(const std::string &name);

std::vector<Mesh> &get_meshes();
// Model             *get_mesh_by_name(const std::string &name);

std::vector<Model> &get_models();
Model              *get_model_by_name(const std::string &name);
} // namespace AssetManager
