#include "API/OpenGL/Renderer/GL_renderer.h"
#include "Core/Game.h"
#include "Renderer/RenderDataManager.h"
#include "Types/Game/Particle.h"

namespace OpenGLRenderer {
extern glm::mat4 _view;
extern glm::mat4 _projection;

void particle_pass() {
  Shader &shader_fire = get_shader("Particle");
  shader_fire.use();

  Camera *camera = Game::get_camera();
  shader_fire.setMat4("u_view", _view);
  shader_fire.setMat4("u_projection", _projection);
  shader_fire.setVec3("u_camera_right", camera->get_right());
  shader_fire.setVec3("u_camera_up", camera->get_up());
  shader_fire.setFloat("u_frame_count", 16.0f);

  for (ParticleSystem *particle_system : RenderDataManager::get_particle_systems()) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particle_system->get_texture()->get_texture());
    shader_fire.setInt("u_particle_texture", 0);

    particle_system->render(_view, _projection, camera->get_right(), camera->get_up());
  }
}
} // namespace OpenGLRenderer
