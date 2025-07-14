#pragma once

#include "Types/Game/AnimEntity.h"
#include "Types/Game/BloodVolumetric.h"
#include "Types/Game/Building/Building.h"
#include "Types/Game/Bullet.h"
#include "Types/Game/Decal.h"
#include "Types/Game/Entity.h"
#include "Types/Game/Unit/Unit.h"

namespace World {
void init();
void begin_frame();
void update(float delta_time);
void submit_render_items();

void process_bullets();

AnimEntity *add_anim_entity(AnimEntity &&anim_entity);
Entity     *add_entity(Entity &&entity);
void        add_unit(Unit &&unit);
void        add_bullet(Bullet &&bullet);
void        add_decal(Decal &&decal);
void        add_blood_volumetric(BloodVolumetric &&blood_volumetric);

std::vector<Bullet> &get_bullets();
Entity              *get_entity_by_object_id(uint64_t object_id);
Building            *get_building_by_name(const std::string &name);
Building            *get_building_by_object_id(uint64_t object_id);

// Npcs
void spawn_unit(const std::string &name, const glm::vec3 &position = glm::vec3(0.0f));

} // namespace World
