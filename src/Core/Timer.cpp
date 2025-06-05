#include "Timer.h"
#include <algorithm>

namespace TimerManager {
std::vector<Timer> _timers;

void init() {
  _timers.clear();
}

void update(float delta_time) {
  for (Timer &timer : _timers) {
    if (timer.triggered) {
      continue;
    }

    timer.elapsed += delta_time;
    if (timer.elapsed >= timer.delay) {
      timer.callback();
      timer.triggered = true;
    }
  }

  _timers.erase(
      std::remove_if(_timers.begin(), _timers.end(), [](const Timer &t) { return t.triggered; }),
      _timers.end());
}

void add_timer(float delay, std::function<void()> callback) {
  _timers.emplace_back(delay, callback);
}

} // namespace TimerManager
