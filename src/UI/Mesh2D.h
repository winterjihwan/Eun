#pragma once
#include "Types.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Mesh2D {
private:
  GLuint  _vao                = 0;
  GLuint  _vbo                = 0;
  GLuint  _ebo                = 0;
  GLsizei _index_count        = 0;
  GLsizei _vertex_buffer_size = 0;
  GLsizei _index_buffer_size  = 0;

public:
  void update_vertex_buffer(std::vector<Vertex2D> &vertices, std::vector<unsigned int> &indices) {
    _index_count = (GLuint)indices.size();
    if (_index_count == 0) {
      return;
    }

    GLsizei vertex_buffer_size = (GLsizei)(vertices.size() * sizeof(Vertex2D));
    GLsizei index_buffer_size  = (GLsizei)(indices.size() * sizeof(unsigned int));

    if (vertex_buffer_size > _vertex_buffer_size || index_buffer_size > _index_buffer_size) {
      reset();
      _vertex_buffer_size = vertex_buffer_size;
      _index_buffer_size  = index_buffer_size;
      glBindVertexArray(_vao);

      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_size, vertices.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_buffer_size, indices.data());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, color));

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  int get_vao() {
    return _vao;
  }

  GLsizei get_index_count() {
    return _index_count;
  }

  void reset() {
    if (_vao != 0) {
      glDeleteVertexArrays(1, &_vao);
      glDeleteBuffers(1, &_vbo);
      glDeleteBuffers(1, &_ebo);
      _vao = 0;
      _vbo = 0;
      _ebo = 0;
    }

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
  }
};
