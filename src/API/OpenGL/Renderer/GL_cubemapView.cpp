#include "GL_cubemapView.h"
#include <iostream>
#include <stb_image/stb_image.h>

void OpenGLCubemapView::init() {
  create_cubemap();
}

// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
void OpenGLCubemapView::create_cubemap() {
  glGenTextures(1, &_handle);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);

  std::vector<std::string> faces{
      "res/textures/Sky/skybox_r.jpg",
      "res/textures/Sky/skybox_l.jpg",
      "res/textures/Sky/skybox_u.jpg",
      "res/textures/Sky/skybox_d.jpg",
      "res/textures/Sky/skybox_f.jpg",
      "res/textures/Sky/skybox_b.jpg",
  };

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0,
                   GL_RGB,
                   width,
                   height,
                   0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE,
                   data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

GLuint OpenGLCubemapView::get_handle() const {
  return _handle;
}
