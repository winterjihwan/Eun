#pragma once
#include "Types.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Mesh2D {
private:
  GLuint  _vao                = 0;
  GLuint  _vbo                = 0;
  GLsizei _vertex_buffer_size = 0;

public:
  void update_vertex_buffer(std::vector<Vertex2D> &vertices) {
    GLsizei vertex_buffer_size = (GLsizei)(vertices.size() * sizeof(Vertex2D));

    if (vertex_buffer_size > _vertex_buffer_size) {
      reset();
      _vertex_buffer_size = vertex_buffer_size;
      glBindVertexArray(_vao);

      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_size, vertices.data());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  int get_vao() {
    return _vao;
  }

  void reset() {
    if (_vao != 0) {
      glDeleteVertexArrays(1, &_vao);
      glDeleteBuffers(1, &_vbo);
      _vao = 0;
      _vbo = 0;
    }

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
  }
};
