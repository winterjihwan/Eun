#pragma once

#include <cstring>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct ColorAttachment {
  const char *name            = "undefined";
  GLuint      handle          = 0;
  GLenum      internal_format = 0;

  GLenum format = 0;
  GLenum type   = 0;
};

struct DepthAttachment {
  GLuint handle          = 0;
  GLenum internal_format = 0;
};

class OpenGLFrameBuffer {
private:
  const char                  *_name   = "UNSET";
  GLuint                       _handle = 0;
  GLuint                       _width  = 0;
  GLuint                       _height = 0;
  std::vector<ColorAttachment> _color_attachments;
  DepthAttachment              _depth_attachment;

public:
  OpenGLFrameBuffer() = default;
  OpenGLFrameBuffer(const char *name, int width, int height);
  void   clean_up();
  void   create_attachment(const char *name,
                           GLenum      internal_format,
                           GLenum      min_filter = GL_NEAREST,
                           GLenum      mag_filter = GL_NEAREST);
  void   create_depth_attachment();
  void   bind();
  void   draw_buffers(std::vector<const char *> names);
  GLuint get_handle() const;
  GLuint get_width() const;
  GLuint get_height() const;
  GLuint get_color_attachment_handle_by_name(const char *name) const;
  GLuint get_depth_attachment_handle_by_name() const;
  GLuint get_color_attachment_slot_by_name(const char *name) const;
};
