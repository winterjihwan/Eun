#pragma once

namespace Editor {
void init();
void update(float delta_time);
void update_hover();
void update_selection();
void update_translation();

void open_editor();
void close_editor();
void toggle_editor_open_state();

bool is_open();
} // namespace Editor
