#include "Particle.h"
#include "AssetManager/AssetManager.h"
#include "Renderer/RenderDataManager.h"
#include "Util/Util.h"

ParticleSystem::~ParticleSystem() {
  if (_vao) {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo_quad);
    glDeleteBuffers(1, &_vbo_instance);
  }
}

void ParticleSystem::init(Texture *texture, int max_particles) {
  _texture = texture;
  _particles.clear();
  _particles.reserve(max_particles);

  for (int i = 0; i < max_particles; ++i) {
    Particle p;
    reset_particle(p);
    _particles.push_back(p);
  }

  generate_quad();
}

void ParticleSystem::reset_particle(Particle &p) {
  if (_use_bounds) {
    glm::vec3 center = _bounds->get_center();
    glm::vec3 extent = _bounds->get_extent();

    float x  = Util::random_float(center.x - extent.x, center.x + extent.x);
    float y  = Util::random_float(center.y - extent.y, center.y + extent.y);
    float z  = Util::random_float(center.z - extent.z, center.z + extent.z);
    p.offset = glm::vec3(x, y, z);
  } else {
    p.offset = glm::vec3(Util::random_float(-1.0f, 1.0f), 0.0f, Util::random_float(-1.0f, 1.0f));
  }

  p.size  = Util::random_float(0.5f, 1.0f);
  p.alpha = 1.0f;
  p.frame = Util::random_float(0.0f, _frame_count * 0.5f);
}

void ParticleSystem::update(float delta_time) {
  for (Particle &particle : _particles) {
    particle.alpha -= delta_time * 0.5f;
    particle.size += delta_time * 0.1f;
    particle.frame += delta_time * 12.0f;

    if (particle.alpha <= 0.0f || particle.frame >= _frame_count) {
      reset_particle(particle);
    }
  }
}

void ParticleSystem::submit_render_item() {
  RenderDataManager::submit_particle_system(this);
}

void ParticleSystem::render(const glm::mat4 &view,
                            const glm::mat4 &projection,
                            const glm::vec3 &camera_right,
                            const glm::vec3 &camera_up) {
  if (_particles.empty()) {
    return;
  }

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo_instance);
  glBufferData(
      GL_ARRAY_BUFFER, _particles.size() * sizeof(Particle), _particles.data(), GL_DYNAMIC_DRAW);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _particles.size());
  glBindVertexArray(0);
}

void ParticleSystem::set_bounds(AABB *bounds) {
  _bounds     = bounds;
  _use_bounds = true;
}

void ParticleSystem::set_particle_count(int count) {
  if (count < 0) {
    return;
  }

  int current = _particles.size();
  if (count > current) {
    _particles.reserve(count);
    for (int i = current; i < count; ++i) {
      Particle p;
      reset_particle(p);
      _particles.push_back(p);
    }
  } else if (count < current) {
    _particles.resize(count);
  }
}

void ParticleSystem::generate_quad() {
  float quad[] = {
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      0.5f,
      0.5f,
  };

  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo_quad);
  glGenBuffers(1, &_vbo_instance);

  glBindVertexArray(_vao);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo_quad);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo_instance);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, offset));
  glVertexAttribDivisor(1, 1);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
      2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, size));
  glVertexAttribDivisor(2, 1);

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(
      3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, alpha));
  glVertexAttribDivisor(3, 1);

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(
      4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, frame));
  glVertexAttribDivisor(4, 1);
}

Texture *ParticleSystem::get_texture() const {
  return _texture;
}
