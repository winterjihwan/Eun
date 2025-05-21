#include "Audio.h"
#include "Defines.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.hpp"
#include <iostream>

// TODO: Audio handle

namespace Audio {
ma_engine                                   _engine;
std::unordered_map<std::string, ma_sound *> _loaded_sounds;

void init() {
  ma_result result = ma_engine_init(NULL, &_engine);
  if (result != MA_SUCCESS) {
    std::cout << "Audio::init() fail with " << ma_result_description(result) << std::endl;
  }
}

void load_audio(const std::string &filename) {
  ma_sound   *sound     = new ma_sound{};
  std::string full_path = AUDIO_BASE_PATH + filename;
  ma_result   result =
      ma_sound_init_from_file(&_engine, full_path.c_str(), MA_SOUND_FLAG_DECODE, 0, 0, sound);

  if (result != MA_SUCCESS) {
    std::cout << "Audio::load_audio() failed with " << ma_result_description(result) << std::endl;
    delete sound;
    return;
  };

  _loaded_sounds[filename] = sound;
}

void play_audio(const std::string &filename, float volume, float frequency) {
  if (_loaded_sounds.find(filename) == _loaded_sounds.end()) {
    load_audio(filename);
  }

  ma_sound *sound = _loaded_sounds[filename];
  ma_sound_seek_to_pcm_frame(sound, 0);
  ma_sound_set_volume(sound, volume);
  ma_sound_set_pitch(sound, frequency);
  ma_sound_start(sound);
}

void loop_audio(const std::string &filename, float volume) {
  if (_loaded_sounds.find(filename) == _loaded_sounds.end()) {
    load_audio(filename);
  }

  ma_sound *sound = _loaded_sounds[filename];
  ma_sound_set_volume(sound, volume);
  ma_sound_set_looping(sound, true);
  ma_sound_start(sound);
}

void stop_audio(const std::string &filename) {
  auto it = _loaded_sounds.find(filename);
  if (it != _loaded_sounds.end()) {
    ma_sound_stop(it->second);
  }
}
} // namespace Audio
