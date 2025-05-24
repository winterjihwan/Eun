#pragma once

#include "Mesh.h"
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

using namespace std;

class Model {
public:
  string          name;
  vector<Texture> textures_loaded;
  vector<Mesh>    meshes;
  string          directory;
  bool            gammaCorrection;

  Model(string const &path, const string &name, bool gamma = false)
      : name(name), gammaCorrection(gamma) {
    loadModel(path);
  }

  void Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(shader);
  }

  void DrawWithRagdollPose(Shader                                 &shader,
                           const std::map<std::string, glm::mat4> &bone_transforms) {
    for (unsigned int i = 0; i < meshes.size(); ++i) {
      std::vector<glm::mat4> final_matrices(100, glm::mat4(1.0f));

      for (const auto &[bone_name, info] : m_BoneInfoMap) {
        int         bone_id       = info.id;
        glm::mat4   offset        = info.offset;
        std::string stripped_name = bone_name;
        if (stripped_name.find("Armature_") == 0)
          stripped_name = stripped_name.substr(9);

        if (bone_transforms.count(stripped_name)) {
          final_matrices[bone_id] = bone_transforms.at(stripped_name);
        }
      }

      for (int j = 0; j < final_matrices.size(); ++j) {
        shader.setMat4("finalBonesMatrices[" + std::to_string(j) + "]", final_matrices[j]);
      }

      meshes[i].Draw(shader);
    }
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
  std::map<string, BoneInfo> m_BoneInfoMap;
  int                        m_BoneCounter = 0;
  AssimpNodeData             m_RootNode;

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

  void loadModel(string const &path) {
    Assimp::Importer importer;
    const aiScene   *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                 aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    ReadHierarchyData(m_RootNode, scene->mRootNode);
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
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

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

    vector<Texture> diffuseMaps =
        loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<Texture> specularMaps =
        loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    ExtractBoneWeightForVertices(vertices, mesh, scene);

    return Mesh(vertices, indices, textures);
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

  unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false) {
    string filename = string(path);
    filename        = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int            width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
        format = GL_RGB;
      else
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
    }

    return textureID;
  }

  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);

      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          skip = true;

          break;
        }
      }

      if (!skip) {
        Texture texture;
        texture.id   = TextureFromFile(str.C_Str(), this->directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        textures_loaded.push_back(texture);
      }
    }
    return textures;
  }
};
