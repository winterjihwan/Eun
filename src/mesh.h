#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Types.h"
#include "Util/Util.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Mesh {
private:
  unsigned int VBO, EBO;

public:
  std::string               name;
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture>      textures;
  unsigned int              VAO;

  Mesh(std::vector<Vertex>       vertices,
       std::vector<unsigned int> indices,
       std::vector<Texture>      textures,
       std::string               name) {
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;
    this->name     = name;
  }

  void draw(Shader &shader) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i);

      std::string number;
      TextureType type = textures[i].type;
      if (type == TextureType::DIFFUSE)
        number = std::to_string(diffuseNr++);
      else if (type == TextureType::SPECULAR)
        number = std::to_string(specularNr++);
      else if (type == TextureType::NORMAL)
        number = std::to_string(normalNr++);
      else if (type == TextureType::HEIGHT)
        number = std::to_string(heightNr++);

      glUniform1i(glGetUniformLocation(shader.ID, (Util::to_string(type) + number).c_str()), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].get_handle());
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
  }

  void update_mesh(const std::vector<Vertex>       &new_vertices,
                   const std::vector<unsigned int> &new_indices) {
    vertices = new_vertices;
    indices  = new_indices;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);
  }

  void upload_to_gpu() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    // Vertex Texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoords));

    // Vertex Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

    // Vertex Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

    // Ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

    // Weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(
        6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
  }
};
