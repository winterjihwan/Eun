#pragma once

namespace OpenGLRenderer {
void init();
void render_game();

void geometry_pass();
void skybox_pass();
void anim_pass();
void decal_pass();
} // namespace OpenGLRenderer
