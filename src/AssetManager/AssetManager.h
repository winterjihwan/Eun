#pragma once

#include "Types/Animation/Animation.h"
#include "Types/Renderer/Mesh.h"
#include "Types/Renderer/SkinnedModel.h"
#include "Types/Texture/ExrTexture.h"
#include "Util/Util.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <string>

namespace AssetManager {
void init();
void shutdown();

Animation         *get_animation_by_name(const std::string &name);
Animation         *get_animation_by_index(int index);
int                get_animation_index_by_name(const std::string &name);
std::vector<Mesh> &get_meshes();
Mesh              *get_mesh_by_name(const std::string &name);
Model             *get_model_by_name(const std::string &name);
Model             *get_model_by_index(int index);
SkinnedModel      *get_skinned_model_by_name(const std::string &name);
SkinnedModel      *get_skinned_model_by_index(int index);
int                get_skinned_model_index_by_name(const std::string &name);
Texture           *get_texture_by_name(const std::string &name);
ExrTexture        *get_exr_texture_by_name(const std::string &name);

bool loading_complete();
void update_loading();
} // namespace AssetManager
