#pragma once

#include "Types/Animation/Animation.h"
#include "Types/Renderer/Model.h"
#include <map>
#include <string>

#define MAX_NUM_BONES_PER_VERTEX 4

#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

#define MAX_BONES (200)

class SkinnedModel : public Model {
public:
  SkinnedModel(const std::string &name, const std::string &path, bool gamma = false)
      : Model(name, path, gamma) {};
  SkinnedModel(SkinnedModel &&)            = default;
  SkinnedModel &operator=(SkinnedModel &&) = default;
  ~SkinnedModel();

  void                         get_bone_transforms(Animation              *animation,
                                                   float                   time_ticks,
                                                   std::vector<glm::mat4> &out_transforms);
  virtual std::vector<Vertex> &get_vertices();

private:
  virtual void reserve_space(unsigned int num_vertices, unsigned int num_indices);
  virtual void init_single_mesh(uint index, const aiMesh *mesh);

  struct VertexBoneData {
    uint  bone_ids[MAX_NUM_BONES_PER_VERTEX] = {0};
    float weights[MAX_NUM_BONES_PER_VERTEX]  = {0.0f};
    int   index                              = 0;

    VertexBoneData() {};

    void add_bone_data(uint bone_id, float weight) {
      for (int i = 0; i < index; i++) {
        if (bone_ids[i] == bone_id) {
          return;
        }
      }

      // Zero Weight Problem
      if (weight == 0.0f) {
        return;
      }

      if (index == MAX_NUM_BONES_PER_VERTEX) {
        assert(0);
      }

      bone_ids[index] = bone_id;
      weights[index]  = weight;

      index++;
    }
  };

  struct SkinnedVertex {
    glm::vec3      position;
    glm::vec2      tex_coords;
    glm::vec3      normal;
    VertexBoneData bones;
  };

  struct BoneInfo {
    glm::mat4 offset_matrix;
    glm::mat4 final_transformation;

    BoneInfo(const glm::mat4 &offset) {
      offset_matrix        = offset;
      final_transformation = glm::mat4(0.0f);
    }
  };

  struct NodeInfo {
    NodeInfo() {
    }

    NodeInfo(const aiNode *n) {
      node = n;
    }

    const aiNode *node     = NULL;
    bool          required = false;
  };

  struct LocalTransform {
    aiVector3D   scaling;
    aiQuaternion rotation;
    aiVector3D   translation;
  };

  std::vector<Vertex>             _plain_vertices;
  std::vector<SkinnedVertex>      _skinned_vertices;
  std::map<std::string, uint>     _bone_name_to_index_map;
  std::vector<BoneInfo>           _bone_infos;
  std::map<std::string, NodeInfo> _required_node_map;

  virtual void upload_to_gpu();
  void         initialize_required_node_map(const aiNode *node);
  void         load_mesh_bones(uint                        index,
                               const aiMesh               *mesh,
                               std::vector<SkinnedVertex> &skinned_vertices,
                               int                         base_vertex);
  void         load_single_bone(uint                        index,
                                const aiBone               *bone,
                                std::vector<SkinnedVertex> &skinned_vertices,
                                int                         base_vertex);
  int          get_bone_id(const aiBone *bone);
  void         mark_required_nodes_for_bone(const aiBone *bone);
  void         read_node_hierarchy(float            animation_time_ticks,
                                   const aiNode    *node,
                                   const glm::mat4 &parent_transform,
                                   Animation       *animation);
  void         calc_local_transform(LocalTransform   &transform,
                                    float             animation_time_ticks,
                                    const aiNodeAnim *node_anim);
  uint         find_rotation(float animation_time_ticks, const aiNodeAnim *node_anim);
  void         calc_interpolated_rotation(aiQuaternion     &out,
                                          float             animation_time_ticks,
                                          const aiNodeAnim *node_anim);
  uint         find_position(float animation_time_ticks, const aiNodeAnim *node_anim);
  void         calc_interpolated_position(aiVector3D       &out,
                                          float             animation_time_ticks,
                                          const aiNodeAnim *node_anim);
  uint         find_scaling(float animation_time_ticks, const aiNodeAnim *node_anim);
  void         calc_interpolated_scaling(aiVector3D       &out,
                                         float             animation_time_ticks,
                                         const aiNodeAnim *node_anim);
};
