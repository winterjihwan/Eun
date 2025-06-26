#pragma once

#include "Enums.h"
namespace Editor {
void init();
void update(float delta_time);
void update_object_hover();
void open_editor();
void close_editor();
void set_editor_state(EditorState editor_state);
void toggle_editor_open_state();
bool is_open();

} // namespace Editor
