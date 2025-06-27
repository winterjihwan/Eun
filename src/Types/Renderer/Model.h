#pragma once

#include "Defines.h"
#include "Material.h"
#include "Types.h"
#include "stb_image/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Model {
public:
  std::string name;
  std::string path;
  bool        gamma_correction;

  Model(const std::string &name, const std::string &path, bool gamma = false);

  Model(Model &&)                 = default;
  Model &operator=(Model &&)      = default;
  Model(const Model &)            = delete;
  Model &operator=(const Model &) = delete;

  virtual ~Model();

  void               load();
  void               render();
  void               setup_materials(unsigned int mesh_index, unsigned int material_index);
  const std::string &get_name();
  virtual std::vector<Vertex> &get_vertices();
  std::vector<uint>           &get_indices();

protected:
  enum BUFFER_TYPE {
    INDEX_BUFFER     = 0,
    VERTEX_BUFFER    = 1,
    WVP_MAT_BUFFER   = 2,
    WORLD_MAT_BUFFER = 3,
    NUM_BUFFERS      = 4
  };

  struct BasicMeshEntry {
    BasicMeshEntry() {
      NumIndices    = 0;
      BaseVertex    = 0;
      BaseIndex     = 0;
      MaterialIndex = INVALID_MATERIAL;
    }

    uint NumIndices;
    uint BaseVertex;
    uint BaseIndex;
    uint MaterialIndex;
  };

  const aiScene              *_scene = 0;
  glm::mat4                   _global_inverse_transform;
  std::vector<BasicMeshEntry> _meshes;
  std::vector<Vertex>         _vertices;
  std::vector<uint>           _indices;

  GLuint _VAO                  = 0;
  GLuint _buffers[NUM_BUFFERS] = {0};

protected:
  void         clear();
  virtual void reserve_space(uint num_vertices, uint num_indices);
  virtual void upload_to_gpu();
  virtual void init_single_mesh(uint mesh_index, const aiMesh *aiMesh);

private:
  std::unique_ptr<Assimp::Importer> _importer;

  std::vector<Material> _materials;

  void init_from_scene(const aiScene *scene, const std::string &path);
  void load_textures(const std::string &dir, const aiMaterial *material, int index);
  void load_diffuse_texture(const std::string &dir, const aiMaterial *material, int index);
  void load_diffuse_texture_embedded(const aiTexture *texture, int index);
  void load_diffuse_texture_from_file(const std::string &dir, const aiString &path, int index);
  void load_specular_texture(const std::string &dir, const aiMaterial *material, int index);
  void load_specular_texture_embedded(const aiTexture *paiTexture, int MaterialIndex);
  void load_specular_texture_from_file(const std::string &dir, const aiString &path, int index);
  void load_colors(const aiMaterial *material, int index);
  void count_vertices_and_indices(const aiScene *scene,
                                  unsigned int  &num_vertices,
                                  unsigned int  &num_indices);
};
