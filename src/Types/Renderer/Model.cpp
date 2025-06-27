#include "Model.h"
#include "Defines.h"
#include "Util/Util.h"

#include <iostream>

Model::Model(const std::string &name, const std::string &path, bool gamma) {
  this->name             = name;
  this->path             = path;
  this->gamma_correction = gamma;
  this->_importer        = std::make_unique<Assimp::Importer>();
}

void Model::clear() {
  if (_buffers[0] != 0) {
    glDeleteBuffers(ARRAY_SIZE(_buffers), _buffers);
    _buffers[0] = _buffers[1] = 0;
  }

  if (_VAO != 0) {
    glDeleteVertexArrays(1, &_VAO);
    _VAO = 0;
  }
}

Model::~Model() {
  // TODO: Fix CLEAR
  // clear();
}

void Model::load() {
  clear();

  // VAO
  glGenVertexArrays(1, &_VAO);

  glBindVertexArray(_VAO);
  glGenBuffers(ARRAY_SIZE(_buffers), _buffers);

  _scene = _importer->ReadFile(path, ASSIMP_LOAD_FLAGS_GEOMETRY);

  if (_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
    std::cout << "Model::load(), AI_SCENE_FLAGS_INCOMPLETE set: " << _importer->GetErrorString()
              << std::endl;
    return;
  };

  if (!_scene) {
    std::cout << "Model::load(), No scene: " << _importer->GetErrorString() << std::endl;
    return;
  }

  if (!_scene->mRootNode) {
    std::cout << "Model::load(), No root node " << _importer->GetErrorString() << std::endl;
    return;
  }

  _global_inverse_transform = Util::to_mat4(_scene->mRootNode->mTransformation);
  _global_inverse_transform = glm::inverse(_global_inverse_transform);

  init_from_scene(_scene, path);

  glBindVertexArray(0);
}

void Model::render() {
  glBindVertexArray(_VAO);

  for (unsigned int i = 0; i < _meshes.size(); i++) {
    unsigned int material_index = _meshes[i].MaterialIndex;
    assert(material_index < _materials.size());

    setup_materials(i, material_index);

    glDrawElementsBaseVertex(GL_TRIANGLES,
                             _meshes[i].NumIndices,
                             GL_UNSIGNED_INT,
                             (void *)(sizeof(unsigned int) * _meshes[i].BaseIndex),
                             _meshes[i].BaseVertex);
  }

  glBindVertexArray(0);
}

void Model::setup_materials(unsigned int mesh_index, unsigned int material_index) {
  if (_materials[material_index].diffuse) {
    _materials[material_index].diffuse->bind(COLOR_TEXTURE_UNIT);
  }

  if (_materials[material_index].specular_exponent) {
    _materials[material_index].specular_exponent->bind(SPECULAR_EXPONENT_UNIT);
  }
}

void Model::reserve_space(unsigned int num_vertices, unsigned int num_indices) {
  _vertices.reserve(num_vertices);
  _indices.reserve(num_indices);
}

void Model::init_from_scene(const aiScene *scene, const std::string &path) {
  _meshes.resize(scene->mNumMeshes);
  _materials.resize(scene->mNumMaterials);

  // Vertices and indices
  unsigned int num_vertices = 0;
  unsigned int num_indices  = 0;

  count_vertices_and_indices(scene, num_vertices, num_indices);

  reserve_space(num_vertices, num_indices);

  for (unsigned int i = 0; i < _meshes.size(); i++) {
    const aiMesh *aiMesh = scene->mMeshes[i];
    init_single_mesh(i, aiMesh);
  }

  // Materials
  std::string dir = Util::get_dir_from_filename(path);

  for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
    const aiMaterial *material = scene->mMaterials[i];

    load_textures(dir, material, i);
    load_colors(material, i);
  }

  upload_to_gpu();
}

void Model::upload_to_gpu() {
  glBindBuffer(GL_ARRAY_BUFFER, _buffers[VERTEX_BUFFER]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[INDEX_BUFFER]);

  glBufferData(
      GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

  // Position
  glEnableVertexAttribArray(POSITION_LOCATION);
  glVertexAttribPointer(
      POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

  // Tex Coords
  glEnableVertexAttribArray(TEX_COORD_LOCATION);
  glVertexAttribPointer(TEX_COORD_LOCATION,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex, tex_coords));

  //  Normal
  glEnableVertexAttribArray(NORMAL_LOCATION);
  glVertexAttribPointer(
      NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
}

void Model::load_textures(const std::string &dir, const aiMaterial *material, int index) {
  load_diffuse_texture(dir, material, index);
  load_specular_texture(dir, material, index);
}

void Model::load_diffuse_texture(const std::string &dir, const aiMaterial *material, int index) {
  _materials[index].diffuse = 0;

  if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiString Path;

    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) ==
        AI_SUCCESS) {
      const aiTexture *embedded_texture = _scene->GetEmbeddedTexture(Path.C_Str());

      if (embedded_texture) {
        load_diffuse_texture_embedded(embedded_texture, index);
      } else {
        load_diffuse_texture_from_file(dir, Path, index);
      }
    }
  }
}

void Model::load_diffuse_texture_embedded(const aiTexture *texture, int index) {
  _materials[index].diffuse = new Texture(GL_TEXTURE_2D, "", "");
  int buffer_size           = texture->mWidth;
  _materials[index].diffuse->load(buffer_size, texture->pcData);
}

void Model::load_diffuse_texture_from_file(const std::string &dir,
                                           const aiString    &path,
                                           int                index) {
  std::string fullpath = dir + "/" + path.data;

  _materials[index].diffuse = new Texture(GL_TEXTURE_2D, fullpath.c_str(), "");

  if (!_materials[index].diffuse->load()) {
    std::cout << "Model::load_dissue_texure_from_file() error, path: " << fullpath << std::endl;
    // exit(0);
  }
}

void Model::load_specular_texture(const std::string &dir, const aiMaterial *material, int index) {
  _materials[index].specular_exponent = NULL;

  if (material->GetTextureCount(aiTextureType_SHININESS) > 0) {
    aiString path;

    if (material->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) ==
        AI_SUCCESS) {
      const aiTexture *embedded_texture = _scene->GetEmbeddedTexture(path.C_Str());

      if (embedded_texture) {
        load_specular_texture_embedded(embedded_texture, index);
      } else {
        load_specular_texture_from_file(dir, path, index);
      }
    }
  }
}

void Model::load_specular_texture_embedded(const aiTexture *paiTexture, int index) {
  _materials[index].specular_exponent = new Texture(GL_TEXTURE_2D, "", "");
  int buffer_size                     = paiTexture->mWidth;
  _materials[index].specular_exponent->load(buffer_size, paiTexture->pcData);
}

void Model::load_specular_texture_from_file(const std::string &dir,
                                            const aiString    &path,
                                            int                index) {
  std::string fullpath = dir + "/" + path.data;

  _materials[index].specular_exponent = new Texture(GL_TEXTURE_2D, fullpath.c_str(), "");

  if (!_materials[index].specular_exponent->load()) {
    std::cout << "Model::load_specular_texure_from_file() error, path: " << fullpath << std::endl;
    exit(0);
  }
}

void Model::load_colors(const aiMaterial *material, int index) {
  aiColor4D ambient_color(0.0f, 0.0f, 0.0f, 0.0f);
  glm::vec4 all_ones(1.0f);

  int shading_model = 0;
  if (material->Get(AI_MATKEY_SHADING_MODEL, shading_model) == AI_SUCCESS) {
    // TODO: Process Shading Model
  }

  if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color) == AI_SUCCESS) {
    _materials[index].ambient_color.r = ambient_color.r;
    _materials[index].ambient_color.g = ambient_color.g;
    _materials[index].ambient_color.b = ambient_color.b;
  } else {
    _materials[index].ambient_color = all_ones;
  }

  aiColor3D diffuse_color(0.0f, 0.0f, 0.0f);
  if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS) {
    _materials[index].diffuse_color.r = diffuse_color.r;
    _materials[index].diffuse_color.g = diffuse_color.g;
    _materials[index].diffuse_color.b = diffuse_color.b;
  }

  aiColor3D specular_color(0.0f, 0.0f, 0.0f);
  if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color) == AI_SUCCESS) {
    _materials[index].specular_color.r = specular_color.r;
    _materials[index].specular_color.g = specular_color.g;
    _materials[index].specular_color.b = specular_color.b;
  }
}

void Model::init_single_mesh(uint mesh_index, const aiMesh *aiMesh) {
  const aiVector3D zero_3d(0.0f, 0.0f, 0.0f);

  Vertex v;

  for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
    const aiVector3D &pos = aiMesh->mVertices[i];
    v.position            = glm::vec3(pos.x, pos.y, pos.z);

    if (aiMesh->mNormals) {
      const aiVector3D &normal = aiMesh->mNormals[i];
      v.normal                 = glm::vec3(normal.x, normal.y, normal.z);
    } else {
      aiVector3D normal(0.0f, 1.0f, 0.0f);
      v.normal = glm::vec3(normal.x, normal.y, normal.z);
    }

    const aiVector3D &tex_coord =
        aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0][i] : zero_3d;
    v.tex_coords = glm::vec2(tex_coord.x, tex_coord.y);

    _vertices.push_back(v);
  }

  for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
    const aiFace &face = aiMesh->mFaces[i];

    _indices.push_back(face.mIndices[0]);
    _indices.push_back(face.mIndices[1]);
    _indices.push_back(face.mIndices[2]);
  }
}

void Model::count_vertices_and_indices(const aiScene *scene,
                                       unsigned int  &num_vertices,
                                       unsigned int  &num_indices) {
  for (unsigned int i = 0; i < _meshes.size(); i++) {
    _meshes[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
    _meshes[i].NumIndices    = scene->mMeshes[i]->mNumFaces * 3;
    _meshes[i].BaseVertex    = num_vertices;
    _meshes[i].BaseIndex     = num_indices;

    num_vertices += scene->mMeshes[i]->mNumVertices;
    num_indices += _meshes[i].NumIndices;
  }
};

const std::string &Model::get_name() {
  return name;
}

std::vector<Vertex> &Model::get_vertices() {
  return _vertices;
}

std::vector<uint> &Model::get_indices() {
  return _indices;
};
