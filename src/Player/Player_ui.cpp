#include "Player.h"
#include "UI/UIBackend.h"
#include <iomanip>

void Player::update_ui() {
  // Minerals
  std::stringstream ss;
  ss << "Minerals " << std::setw(8) << std::setfill(' ') << _minerals;
  UIBackend::blit_text(ss.str(),
                       "AncizarSerif",
                       VIEWPORT_WIDTH - 196,
                       VIEWPORT_HEIGHT - 16,
                       UIAlignment::CENTERED,
                       0.5f);
}
