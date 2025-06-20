#include "SkinnedModel.h"
#include "Util/Util.h"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

SkinnedModel::~SkinnedModel() {
  // TODO: Fix CLEAR
  // clear();
}

void SkinnedModel::reserve_space(unsigned int num_vertices, unsigned int num_indices) {
  Model::reserve_space(num_vertices, num_indices);
  initialize_required_node_map(_scene->mRootNode);
}

void SkinnedModel::upload_to_gpu() {
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _buffers[VERTEX_BUFFER]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[INDEX_BUFFER]);

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(_skinned_vertices[0]) * _skinned_vertices.size(),
               &_skinned_vertices[0],
               GL_STATIC_DRAW);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

  // Position
  glEnableVertexAttribArray(POSITION_LOCATION);
  glVertexAttribPointer(POSITION_LOCATION,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, position));

  // Tex Coords
  glEnableVertexAttribArray(TEX_COORD_LOCATION);
  glVertexAttribPointer(TEX_COORD_LOCATION,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, tex_coords));

  // Normal
  glEnableVertexAttribArray(NORMAL_LOCATION);
  glVertexAttribPointer(NORMAL_LOCATION,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(SkinnedVertex),
                        (void *)offsetof(SkinnedVertex, normal));

  // Bone IDs
  size_t num_floats = 3 + 2 + 3;
  glEnableVertexAttribArray(BONE_ID_LOCATION);
  glVertexAttribIPointer(BONE_ID_LOCATION,
                         MAX_NUM_BONES_PER_VERTEX,
                         GL_INT,
                         sizeof(SkinnedVertex),
                         (const void *)(num_floats * sizeof(float)));

  // Bone Weights
  num_floats += MAX_NUM_BONES_PER_VERTEX;
  glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
  glVertexAttribPointer(BONE_WEIGHT_LOCATION,
                        MAX_NUM_BONES_PER_VERTEX,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(SkinnedVertex),
                        (const void *)(num_floats * sizeof(float)));
}

void SkinnedModel::init_single_mesh(uint index, const aiMesh *mesh) {
  const aiVector3D zero_3d(0.0f);

  // Vertices
  SkinnedVertex v = {};
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    const aiVector3D &pos = mesh->mVertices[i];
    v.position            = glm::vec3(pos.x, pos.y, pos.z);

    if (mesh->mNormals) {
      const aiVector3D &pNormal = mesh->mNormals[i];
      v.normal                  = glm::vec3(pNormal.x, pNormal.y, pNormal.z);
    } else {
      aiVector3D normal(0.0f);
      v.normal = glm::vec3(normal.x, normal.y, normal.z);
    }

    const aiVector3D &tex_coords = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero_3d;
    v.tex_coords                 = glm::vec2(tex_coords.x, tex_coords.y);

    _skinned_vertices.push_back(v);
  }

  // Indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace &Face = mesh->mFaces[i];
    _indices.push_back(Face.mIndices[0]);
    _indices.push_back(Face.mIndices[1]);
    _indices.push_back(Face.mIndices[2]);
  }

  // Bones
  load_mesh_bones(index, mesh, _skinned_vertices, _meshes[index].BaseVertex);
}

void SkinnedModel::load_mesh_bones(uint                        index,
                                   const aiMesh               *mesh,
                                   std::vector<SkinnedVertex> &skinned_vertices,
                                   int                         base_vertex) {
  if (mesh->mNumBones > MAX_BONES) {
    std::cout << "INCREASE MAX_BONES (SHADER TOO)" << std::endl;
    assert(0);
  }

  for (uint i = 0; i < mesh->mNumBones; i++) {
    load_single_bone(index, mesh->mBones[i], skinned_vertices, base_vertex);
  }
}

void SkinnedModel::load_single_bone(uint                        index,
                                    const aiBone               *bone,
                                    std::vector<SkinnedVertex> &skinned_vertices,
                                    int                         base_vertex) {
  int bone_id = get_bone_id(bone);

  // New Bone
  if (bone_id == _bone_infos.size()) {
    BoneInfo bone_info(Util::to_mat4(bone->mOffsetMatrix));
    _bone_infos.push_back(bone_info);
  }

  for (uint i = 0; i < bone->mNumWeights; i++) {
    const aiVertexWeight &vertex_weight    = bone->mWeights[i];
    uint                  global_vertex_id = base_vertex + bone->mWeights[i].mVertexId;

    skinned_vertices[global_vertex_id].bones.add_bone_data(bone_id, vertex_weight.mWeight);
  }

  mark_required_nodes_for_bone(bone);
}

void SkinnedModel::mark_required_nodes_for_bone(const aiBone *bone) {
  const aiNode *node = _required_node_map[bone->mName.C_Str()].node;

  while (node) {
    auto it = _required_node_map.find(node->mName.C_Str());
    if (it == _required_node_map.end()) {
      std::cout << "SkinnedModel::mark_required_nodes_for_bone() error, no bone with name: "
                << node->mName.C_Str() << std::endl;
      assert(0);
    }

    it->second.required = true;
    node                = node->mParent;
  }
}

int SkinnedModel::get_bone_id(const aiBone *bone) {
  int         bone_index = 0;
  std::string bone_name(bone->mName.C_Str());

  if (_bone_name_to_index_map.find(bone_name) == _bone_name_to_index_map.end()) {
    bone_index                         = (int)_bone_name_to_index_map.size();
    _bone_name_to_index_map[bone_name] = bone_index;
  } else {
    bone_index = _bone_name_to_index_map[bone_name];
  }

  return bone_index;
}

void SkinnedModel::initialize_required_node_map(const aiNode *node) {
  std::string node_name(node->mName.C_Str());

  NodeInfo info(node);

  _required_node_map[node_name] = info;

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    initialize_required_node_map(node->mChildren[i]);
  }
}

void SkinnedModel::get_bone_transforms(Animation              *animation,
                                       float                   time_ticks,
                                       std::vector<glm::mat4> &out_transforms) {
  glm::mat4 identity = glm::mat4(1.0f);
  read_node_hierarchy(time_ticks, _scene->mRootNode, identity, animation);
  out_transforms.resize(_bone_infos.size());

  for (uint i = 0; i < _bone_infos.size(); i++) {
    out_transforms[i] = _bone_infos[i].final_transformation;
  }
}

void SkinnedModel::read_node_hierarchy(float            animation_time_ticks,
                                       const aiNode    *node,
                                       const glm::mat4 &parent_transform,
                                       Animation       *animation) {
  std::string node_name(node->mName.data);
  glm::mat4   node_transformation(Util::to_mat4(node->mTransformation));

  const aiNodeAnim *node_anim = animation->find_node_anim(node_name);

  if (node_anim) {
    LocalTransform transform;
    calc_local_transform(transform, animation_time_ticks, node_anim);

    glm::mat4 scaling_matrix = glm::scale(
        glm::mat4(1.0f), glm::vec3(transform.scaling.x, transform.scaling.y, transform.scaling.z));

    glm::quat rotation_quat(
        transform.rotation.w, transform.rotation.x, transform.rotation.y, transform.rotation.z);
    glm::mat4 rotation_matrix = glm::toMat4(rotation_quat);

    glm::mat4 translation_matrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(transform.translation.x, transform.translation.y, transform.translation.z));

    node_transformation = translation_matrix * rotation_matrix * scaling_matrix;
  }

  glm::mat4 global_transformation = parent_transform * node_transformation;

  if (_bone_name_to_index_map.find(node_name) != _bone_name_to_index_map.end()) {
    uint bone_index = _bone_name_to_index_map[node_name];
    _bone_infos[bone_index].final_transformation =
        _global_inverse_transform * global_transformation * _bone_infos[bone_index].offset_matrix;
  }

  // Recursively do children
  for (uint i = 0; i < node->mNumChildren; i++) {
    std::string child_name(node->mChildren[i]->mName.data);

    auto it = _required_node_map.find(child_name);
    if (it == _required_node_map.end()) {
      std::cout << "SkinnedModel::read_node_hierarchy() error, no child with name: " << child_name
                << std::endl;
      assert(0);
    }

    if (it->second.required) {
      read_node_hierarchy(
          animation_time_ticks, node->mChildren[i], global_transformation, animation);
    }
  }
}

void SkinnedModel::calc_local_transform(LocalTransform   &transform,
                                        float             animation_time_ticks,
                                        const aiNodeAnim *node_anim) {
  calc_interpolated_position(transform.translation, animation_time_ticks, node_anim);
  calc_interpolated_rotation(transform.rotation, animation_time_ticks, node_anim);
  calc_interpolated_scaling(transform.scaling, animation_time_ticks, node_anim);
}

uint SkinnedModel::find_position(float animation_time_ticks, const aiNodeAnim *node_anim) {
  for (uint i = 0; i < node_anim->mNumPositionKeys - 1; i++) {
    float t = (float)node_anim->mPositionKeys[i + 1].mTime;
    if (animation_time_ticks < t) {
      return i;
    }
  }

  return 0;
}

void SkinnedModel::calc_interpolated_position(aiVector3D       &out,
                                              float             animation_time_ticks,
                                              const aiNodeAnim *node_anim) {
  if (node_anim->mNumPositionKeys == 1) {
    out = node_anim->mPositionKeys[0].mValue;
    return;
  }

  uint position_index      = find_position(animation_time_ticks, node_anim);
  uint next_position_index = position_index + 1;
  assert(next_position_index < node_anim->mNumPositionKeys);
  float t1 = (float)node_anim->mPositionKeys[position_index].mTime;

  if (t1 > animation_time_ticks) {
    out = node_anim->mPositionKeys[position_index].mValue;
  } else {
    float t2         = (float)node_anim->mPositionKeys[next_position_index].mTime;
    float delta_time = t2 - t1;
    float factor     = (animation_time_ticks - t1) / delta_time;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D &start = node_anim->mPositionKeys[position_index].mValue;
    const aiVector3D &end   = node_anim->mPositionKeys[next_position_index].mValue;
    aiVector3D        delta = end - start;
    out                     = start + factor * delta;
  }
}

uint SkinnedModel::find_rotation(float animation_time_ticks, const aiNodeAnim *node_anim) {
  assert(node_anim->mNumRotationKeys > 0);

  for (uint i = 0; i < node_anim->mNumRotationKeys - 1; i++) {
    float t = (float)node_anim->mRotationKeys[i + 1].mTime;
    if (animation_time_ticks < t) {
      return i;
    }
  }

  return 0;
}

void SkinnedModel::calc_interpolated_rotation(aiQuaternion     &out,
                                              float             animation_time_ticks,
                                              const aiNodeAnim *node_anim) {
  if (node_anim->mNumRotationKeys == 1) {
    out = node_anim->mRotationKeys[0].mValue;
    return;
  }

  uint rotation_index      = find_rotation(animation_time_ticks, node_anim);
  uint next_rotation_index = rotation_index + 1;
  assert(next_rotation_index < node_anim->mNumRotationKeys);
  float t1 = (float)node_anim->mRotationKeys[rotation_index].mTime;

  if (t1 > animation_time_ticks) {
    out = node_anim->mRotationKeys[rotation_index].mValue;
  } else {
    float t2         = (float)node_anim->mRotationKeys[next_rotation_index].mTime;
    float delta_time = t2 - t1;
    float factor     = (animation_time_ticks - t1) / delta_time;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion &start = node_anim->mRotationKeys[rotation_index].mValue;
    const aiQuaternion &end   = node_anim->mRotationKeys[next_rotation_index].mValue;
    aiQuaternion::Interpolate(out, start, end, factor);
  }

  out.Normalize();
}

void SkinnedModel::calc_interpolated_scaling(aiVector3D       &out,
                                             float             animation_time_ticks,
                                             const aiNodeAnim *node_anim) {
  // Need 2 for scaling
  if (node_anim->mNumScalingKeys == 1) {
    out = node_anim->mScalingKeys[0].mValue;
    return;
  }

  uint scaling_index    = find_scaling(animation_time_ticks, node_anim);
  uint NextScalingIndex = scaling_index + 1;
  assert(NextScalingIndex < node_anim->mNumScalingKeys);
  float t1 = (float)node_anim->mScalingKeys[scaling_index].mTime;
  if (t1 > animation_time_ticks) {
    out = node_anim->mScalingKeys[scaling_index].mValue;
  } else {
    float t2         = (float)node_anim->mScalingKeys[NextScalingIndex].mTime;
    float delta_time = t2 - t1;
    float factor     = (animation_time_ticks - (float)t1) / delta_time;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D &start = node_anim->mScalingKeys[scaling_index].mValue;
    const aiVector3D &end   = node_anim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D        delta = end - start;
    out                     = start + factor * delta;
  }
}

uint SkinnedModel::find_scaling(float animation_time_ticks, const aiNodeAnim *node_anim) {
  assert(node_anim->mNumScalingKeys > 0);

  for (uint i = 0; i < node_anim->mNumScalingKeys - 1; i++) {
    float t = (float)node_anim->mScalingKeys[i + 1].mTime;
    if (animation_time_ticks < t) {
      return i;
    }
  }

  return 0;
}
