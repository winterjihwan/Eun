#pragma once

#include "API/OpenGL/Renderer/GL_frameBuffer.h"
#include "Types/Renderer/Shader.h"

namespace OpenGLRenderer {
void init();
void init_light();
void init_skybox();
void init_aabb();
void render_game();

void geometry_pass();
void skybox_pass();
void anim_pass();
void blood_volumetric_pass();
void decal_pass();
void ui_pass();
void light_pass();
void shadow_pass();
void aabb_pass();
void crystal_pass();
void particle_pass();

Shader            &get_shader(const std::string &name);
OpenGLFrameBuffer &get_frame_buffer(const std::string &name);
} // namespace OpenGLRenderer
