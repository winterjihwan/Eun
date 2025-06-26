#include "UID.h"
#include <mutex>

namespace UID {
uint64_t   _uid = 1;
std::mutex _mutex;

uint64_t get_next() {
  std::lock_guard<std::mutex> lock(_mutex);
  return _uid++;
}
} // namespace UID
