#pragma once

#include <functional>

namespace TimerManager {

struct Timer {
  float                 delay;
  float                 elapsed;
  bool                  triggered;
  std::function<void()> callback;

  Timer(float delay, std::function<void()> callback) {
    this->delay     = delay;
    this->elapsed   = 0.0f;
    this->triggered = false;
    this->callback  = callback;
  }
};

void init();
void update(float delta_time);
void add_timer(float delay, std::function<void()> callback);

} // namespace TimerManager
