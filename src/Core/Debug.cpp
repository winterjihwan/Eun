#include "Debug.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Physics/Physics.h"

namespace Debug {
void update() {

  static bool on = false;

  // HACK
  if (Input::key_pressed(EUN_KEY_9)) {
    on = !on;
  }

  if (on) {
    Physics::submit_debug_lines();
  }
}
} // namespace Debug
