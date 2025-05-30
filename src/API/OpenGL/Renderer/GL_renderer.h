#pragma once

namespace OpenGLRenderer {
void init();
void render_game();

void geometry_pass();
void skybox_pass();
void anim_pass();
void blood_volumetric_pass();
void decal_pass();
void ui_pass();
} // namespace OpenGLRenderer
