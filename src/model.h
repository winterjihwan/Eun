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
  bool                 gammaCorrection;

  Model(const std::string &path, const std::string &name, bool gamma = false)
      : name(name), gammaCorrection(gamma) {
    loadModel(path);
  }

  void Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].draw(shader);
  }

  std::string StripArmaturePrefix(const std::string &name) {
    const std::string prefix = "Armature_";
    if (name.rfind(prefix, 0) == 0)
      return name.substr(prefix.size());
    return name;
  }

  auto &GetBoneInfoMap() {
    return m_BoneInfoMap;
  }
  int &GetBoneCount() {
    return m_BoneCounter;
  }
  AssimpNodeData &GetRootNode() {
    return m_RootNode;
  }

private:
  std::map<std::string, glm::mat4> m_GlobalBindPose;
  std::map<std::string, BoneInfo>  m_BoneInfoMap;
  int                              m_BoneCounter = 0;
  AssimpNodeData                   m_RootNode;

  void ReadHierarchyData(AssimpNodeData &dest, const aiNode *src) {
    dest.name           = src->mName.data;
    dest.transformation = AssimpUtils::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount  = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++) {
      AssimpNodeData child;
      ReadHierarchyData(child, src->mChildren[i]);
      dest.children.push_back(child);
    }
  }

  void loadModel(std::string const &path) {
    Assimp::Importer importer;
    const aiScene   *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                 aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    ReadHierarchyData(m_RootNode, scene->mRootNode);
    glm::mat4 identity = glm::mat4(1.0f);
    ComputeGlobalBindPose(m_RootNode, identity);
  }

  void ComputeGlobalBindPose(const AssimpNodeData &node, const glm::mat4 &parentTransform) {
    glm::mat4 globalTransform = parentTransform * node.transformation;
    if (m_BoneInfoMap.find(node.name) != m_BoneInfoMap.end()) {
      m_GlobalBindPose[node.name] = globalTransform;
    }

    for (const auto &child : node.children) {
      ComputeGlobalBindPose(child, globalTransform);
    }
  }

  void processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  void SetVertexBoneDataToDefault(Vertex &vertex) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
      vertex.m_BoneIDs[i] = -1;
      vertex.m_Weights[i] = 0.0f;
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      SetVertexBoneDataToDefault(vertex);
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

    std::vector<Texture> diffuseMaps =
        loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps =
        loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::HEIGHT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    ExtractBoneWeightForVertices(vertices, mesh, scene);

    return Mesh(std::move(vertices), std::move(indices), std::move(textures), "");
  }

  void SetVertexBoneData(Vertex &vertex, int boneID, float weight) {
    if (weight == 0.0f)
      return;

    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
      if (vertex.m_BoneIDs[i] < 0) {
        vertex.m_Weights[i] = weight;
        vertex.m_BoneIDs[i] = boneID;
        break;
      }
    }
  }

  void
  ExtractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene) {
    auto &boneInfoMap = m_BoneInfoMap;
    int  &boneCount   = m_BoneCounter;

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
        SetVertexBoneData(vertices[vertexId], boneID, weight);
      }
    }
  }

  std::vector<Texture>
  loadMaterialTextures(aiMaterial *mat, aiTextureType ai_type, TextureType type) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++) {
      aiString str;
      mat->GetTexture(ai_type, i, &str);

      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
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
        textures_loaded.push_back(texture);
      }
    }
    return textures;
  }
};
