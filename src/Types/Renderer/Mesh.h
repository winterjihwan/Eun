#pragma once

#include "Defines.h"
#include "Shader.h"
#include "Texture.h"
#include "Types.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Mesh {
private:
  unsigned int _VBO, _EBO;

public:
  std::string               name;
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  Texture                  *texture_diffuse;
  unsigned int              VAO;

  Mesh(std::vector<Vertex>       vertices,
       std::vector<unsigned int> indices,
       Texture                  *texture_diffuse,
       const std::string        &name) {
    this->vertices        = vertices;
    this->indices         = indices;
    this->texture_diffuse = texture_diffuse;
    this->name            = name;
  }

  void draw(Shader &shader) {
    if (texture_diffuse) {
      glActiveTexture(GL_TEXTURE0);
      glUniform1i(glGetUniformLocation(shader.ID, "sampler"), 0);
      glBindTexture(GL_TEXTURE_2D, texture_diffuse->get_texture());
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (texture_diffuse) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }

  void upload_to_gpu() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void *)offsetof(Vertex, position));

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
};
