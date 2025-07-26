#pragma once

#include "Types/AABB/AABB.h"
#include "Types/Game/AnimEntity.h"
#include "Types/Game/BloodVolumetric.h"
#include "Types/Game/Decal.h"
#include "Types/Game/Entity.h"
#include "Types/Game/Particle.h"
#include <vector>

namespace RenderDataManager {
void begin_frame();
void update();

// TODO: Unify with 'submit_render_item'
void submit_anim_entity(AnimEntity *anim_entity);
void submit_entity(Entity *entity);
void submit_decal(Decal *decal);
void submit_blood_volumetric(BloodVolumetric *blood_volumetric);
void submit_aabb(AABB *aabb);
void submit_crystal_entity(Entity *glass_entity);
void submit_particle_system(ParticleSystem *particle_system);

const std::vector<AnimEntity *>      &get_anim_entities();
const std::vector<Entity *>          &get_entities();
const std::vector<Decal *>           &get_decals();
const std::vector<BloodVolumetric *> &get_blood_volumetrics();
const std::vector<AABB *>            &get_aabbs();
const std::vector<Entity *>          &get_crystal_entities();
const std::vector<ParticleSystem *>  &get_particle_systems();

} // namespace RenderDataManager
