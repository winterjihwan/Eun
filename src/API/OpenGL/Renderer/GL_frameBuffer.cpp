#include "GL_frameBuffer.h"
#include "GL_utils.h"

OpenGLFrameBuffer::OpenGLFrameBuffer(const char *name, int width, int height) {
  glGenFramebuffers(1, &_handle);
  _name   = name;
  _width  = width;
  _height = height;
}

void OpenGLFrameBuffer::clean_up() {
  _color_attachments.clear();
  glDeleteFramebuffers(1, &_handle);
  _handle = 0;
}

void OpenGLFrameBuffer::create_attachment(const char *name,
                                          GLenum      internal_format,
                                          GLenum      min_filter,
                                          GLenum      mag_filter) {
  ColorAttachment &color_attachment = _color_attachments.emplace_back();
  color_attachment.name             = name;
  color_attachment.internal_format  = internal_format;
  color_attachment.format           = OpenGLUtil::gl_internal_format_to_gl_format(internal_format);
  color_attachment.type             = OpenGLUtil::gl_internal_format_to_gl_type(internal_format);

  glGenTextures(1, &color_attachment.handle);
  glBindTexture(GL_TEXTURE_2D, color_attachment.handle);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               internal_format,
               _width,
               _height,
               0,
               color_attachment.format,
               color_attachment.type,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0 + _color_attachments.size() - 1,
                         GL_TEXTURE_2D,
                         color_attachment.handle,
                         0);
}

void OpenGLFrameBuffer::create_depth_attachment() {
  glGenRenderbuffers(1, &_depth_attachment.handle);
  glBindRenderbuffer(GL_RENDERBUFFER, _depth_attachment.handle);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
  glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_attachment.handle);
}

void OpenGLFrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _handle);
}

void OpenGLFrameBuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::clear_bind() {
  bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLFrameBuffer::draw_buffers(std::vector<const char *> names) {
  std::vector<GLuint> attachments;
  for (const char *name : names) {
    attachments.push_back(get_color_attachment_slot_by_name(name));
  }
  glDrawBuffers(attachments.size(), attachments.data());
}

void OpenGLFrameBuffer::blit_and_bind_to_default_frame_buffer() {
  bind();
  blit_to_default_frame_buffer();
  unbind();
}

void OpenGLFrameBuffer::blit_to_default_frame_buffer() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, _handle);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void OpenGLFrameBuffer::sanitize_check() {
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "OpenGLFrameBuffer::sanitize_check() error: 0x" << std::hex << status << '\n';
    assert(0);
  }
}

GLuint OpenGLFrameBuffer::get_handle() const {
  return _handle;
}

GLuint OpenGLFrameBuffer::get_width() const {
  return _width;
}

GLuint OpenGLFrameBuffer::get_height() const {
  return _height;
}

GLuint OpenGLFrameBuffer::get_color_attachment_handle_by_name(const char *name) const {
  for (int i = 0; i < _color_attachments.size(); i++) {
    if (name == _color_attachments[i].name) {
      return _color_attachments[i].handle;
    }
  }
  std::cerr << "OpenGLFrameBuffer::get_color_attachment_handle_by_name() Fail, no attachment "
            << name << " in framebuffer " << _name << '\n';
  return GL_NONE;
}

GLuint OpenGLFrameBuffer::get_depth_attachment_handle_by_name() const {
  return _depth_attachment.handle;
}

GLuint OpenGLFrameBuffer::get_color_attachment_slot_by_name(const char *name) const {
  for (int i = 0; i < _color_attachments.size(); i++) {
    if (name == _color_attachments[i].name) {
      return GL_COLOR_ATTACHMENT0 + i;
    }
  }

  std::cerr << "OpenGLFrameBuffer::get_color_attachment_slot_by_name() Fail, no attachment " << name
            << " in framebuffer " << _name << '\n';
  assert(0);
}
