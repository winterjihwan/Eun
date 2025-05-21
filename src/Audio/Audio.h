#pragma once

#include <string>

namespace Audio {
void init();
void load_audio(const std::string &filename);
void play_audio(const std::string &filename, float volume, float frequency = 1.0f);
void loop_audio(const std::string &filename, float volume);
void stop_audio(const std::string &filename);

void load_audio();
}; // namespace Audio
