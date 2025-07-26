#pragma once

#include "Types/AABB/AABB.h"
#include "Types/Renderer/Texture.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Particle {
  glm::vec3 offset;
  float     size;
  float     alpha;
  float     frame;
};

struct ParticleSystem {
  ~ParticleSystem();

  void init(Texture *texture, int max_particles = 10);
  void update(float delta_time);
  void submit_render_item();
  void render(const glm::mat4 &view,
              const glm::mat4 &projection,
              const glm::vec3 &camera_right,
              const glm::vec3 &camera_up);
  void set_bounds(AABB *aabb);
  void set_particle_count(int count);

  Texture *get_texture() const;

private:
  std::vector<Particle> _particles;
  Texture              *_texture    = 0;
  bool                  _use_bounds = false;
  AABB                 *_bounds     = 0;
  GLuint                _vao = 0, _vbo_quad = 0, _vbo_instance = 0;

  const float _frame_count = 16.0f; // e.g. 4x4 flipbook

  void generate_quad();
  void reset_particle(Particle &p);
};
