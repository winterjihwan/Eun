#pragma once

#include "Types/Game/Building/BuildingCommon.h"
#include "Types/Game/Hint.h"
#include "Types/Game/Npc.h"
#include "Types/Game/Particle.h"

struct Building : public Npc {
  Building() = default;

  void init(const std::string &name);
  void update(float delta_time) override;
  void submit_render_item() override;
  bool take_damage(float damage);
  int  get_reward();

private:
  float                  _health         = 0.0f;
  float                  _current_health = 0.0f;
  int                    _reward         = 0.0f;
  BuildingAnimationNames _animation_names;
  ParticleSystem         _particle_system;
  Hint                   _hint;
};
