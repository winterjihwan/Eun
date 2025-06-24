#pragma once

namespace OpenGLRenderer {
void init();
void init_light();
void init_skybox();
void render_game();

void geometry_pass();
void skybox_pass();
void anim_pass();
void blood_volumetric_pass();
void decal_pass();
void ui_pass();
void light_pass();
void shadow_pass();
} // namespace OpenGLRenderer
