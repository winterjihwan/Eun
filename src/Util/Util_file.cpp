#include "Util.h"

namespace Util {
std::string get_dir_from_filename(const std::string &filename) {
  std::string::size_type slash_index;

  slash_index = filename.find_last_of("/");

  std::string Dir;

  if (slash_index == std::string::npos) {
    Dir = ".";
  } else if (slash_index == 0) {
    Dir = "/";
  } else {
    Dir = filename.substr(0, slash_index);
  }

  return Dir;
}
} // namespace Util
