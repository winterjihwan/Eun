#pragma once

#include "Enums.h"
#include "Mesh.h"
#include "Texture.h"
#include "Types.h"
#include "Types/Animation/Animdata.h"
#include "Types/Animation/Assimp_utils.h"
#include "stb_image/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <future>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Model {
public:
  std::string          name;
  std::vector<Texture> textures_loaded;
  std::vector<Mesh>    meshes;
  std::string          directory;
  bool                 gamma_correction;

  Model(const std::string &name, bool gamma = false) {
    this->name             = name;
    this->gamma_correction = gamma;
  }

  std::future<void> load_async(const std::string &path) {
    return std::async(std::launch::async, [this, path]() { this->load(path); });
  }

  void load(std::string const &path) {
    Assimp::Importer importer;
    const aiScene   *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                 aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
    read_hierarchy_data(_root_node, scene->mRootNode);
    glm::mat4 identity = glm::mat4(1.0f);
    compute_global_bind_pose(_root_node, identity);
  }

  void draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].draw(shader);
  }

  void upload_to_gpu() {
    for (Mesh &mesh : meshes) {
      mesh.upload_to_gpu();
    }
  }

  auto &get_bone_info_map() {
    return _bone_info_map;
  }

  int &get_bone_count() {
    return _bone_counter;
  }

  AssimpNodeData &get_root_node() {
    return _root_node;
  }

private:
  std::map<std::string, glm::mat4> _global_bind_pose;
  std::map<std::string, BoneInfo>  _bone_info_map;
  int                              _bone_counter = 0;
  AssimpNodeData                   _root_node;

  void read_hierarchy_data(AssimpNodeData &dest, const aiNode *src) {
    dest.name           = src->mName.data;
    dest.transformation = AssimpUtils::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount  = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++) {
      AssimpNodeData child;
      read_hierarchy_data(child, src->mChildren[i]);
      dest.children.push_back(child);
    }
  }

  void compute_global_bind_pose(const AssimpNodeData &node, const glm::mat4 &parentTransform) {
    glm::mat4 globalTransform = parentTransform * node.transformation;
    if (_bone_info_map.find(node.name) != _bone_info_map.end()) {
      _global_bind_pose[node.name] = globalTransform;
    }

    for (const auto &child : node.children) {
      compute_global_bind_pose(child, globalTransform);
    }
  }

  void process_node(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(process_mesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      process_node(node->mChildren[i], scene);
    }
  }

  void set_vertex_bone_data_to_default(Vertex &vertex) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
      vertex.bone_ids[i] = -1;
      vertex.weights[i]  = 0.0f;
    }
  }

  Mesh process_mesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      set_vertex_bone_data_to_default(vertex);
      vertex.position = AssimpUtils::GetGLMVec(mesh->mVertices[i]);
      vertex.normal   = AssimpUtils::GetGLMVec(mesh->mNormals[i]);

      if (mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x            = mesh->mTextureCoords[0][i].x;
        vec.y            = mesh->mTextureCoords[0][i].y;
        vertex.texcoords = vec;
      } else
        vertex.texcoords = glm::vec2(0.0f, 0.0f);

      vertices.push_back(vertex);
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuse_maps =
        load_material_textures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    std::vector<Texture> specular_maps =
        load_material_textures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    std::vector<Texture> normal_maps =
        load_material_textures(material, aiTextureType_HEIGHT, TextureType::NORMAL);
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

    std::vector<Texture> height_maps =
        load_material_textures(material, aiTextureType_AMBIENT, TextureType::HEIGHT);
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    extract_bone_weight_for_vertices(vertices, mesh, scene);

    return Mesh(std::move(vertices), std::move(indices), std::move(textures), "");
  }

  void set_vertex_bone_data(Vertex &vertex, int boneID, float weight) {
    if (weight == 0.0f)
      return;

    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
      if (vertex.bone_ids[i] < 0) {
        vertex.weights[i]  = weight;
        vertex.bone_ids[i] = boneID;
        break;
      }
    }
  }

  void extract_bone_weight_for_vertices(std::vector<Vertex> &vertices,
                                        aiMesh              *mesh,
                                        const aiScene       *scene) {
    auto &boneInfoMap = _bone_info_map;
    int  &boneCount   = _bone_counter;

    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
      int         boneID   = -1;
      std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
      if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
        BoneInfo newBoneInfo;
        newBoneInfo.id = boneCount;
        newBoneInfo.offset =
            AssimpUtils::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
        boneInfoMap[boneName] = newBoneInfo;

        boneID = boneCount;
        boneCount++;
      } else {
        boneID = boneInfoMap[boneName].id;
      }

      assert(boneID != -1);

      auto weights    = mesh->mBones[boneIndex]->mWeights;
      int  numWeights = mesh->mBones[boneIndex]->mNumWeights;

      for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
        int   vertexId = weights[weightIndex].mVertexId;
        float weight   = weights[weightIndex].mWeight;
        assert(vertexId <= vertices.size());
        set_vertex_bone_data(vertices[vertexId], boneID, weight);
      }
    }
  }

  std::vector<Texture>
  load_material_textures(aiMaterial *mat, aiTextureType ai_type, TextureType type) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++) {
      aiString str;
      mat->GetTexture(ai_type, i, &str);

      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          skip = true;

          break;
        }
      }

      if (!skip) {
        std::string path = std::string(str.C_Str());
        path             = directory + '/' + path;

        Texture texture("", path, type);
        textures.push_back(texture);
        // TODO: Resource waste
        textures_loaded.push_back(texture);
      }
    }
    return textures;
  }
};
