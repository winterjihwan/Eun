#include "Common/Defines.h"
#include "Input/Input.h"
#include "Keycodes.h"
#include "Model.h"
#include "Physics/Physics.h"
#include "Player/Player.h"
#include "Shader.h"
#include "Util/Util.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <GLFW/glfw3.h>

bool init_window();
void init_game();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void shutdown_game();

// Game
const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 800;

struct game_state {
  // Window
  GLFWwindow *window;

  // Player
  Player player;

  // Time
  float delta_time = 0.0f;
  float last_frame = 0.0f;

  // Camera
  Camera camera;
  float  lastX       = WIDTH / 2.0f;
  float  lastY       = HEIGHT / 2.0f;
  bool   first_mouse = true;
};

static game_state *state;

int main(void) {
  state = new game_state{};

  if (!init_window()) {
    return -1;
  }

  init_game();

  /* Shaders */
  Shader shader_model("shaders/model.vert", "shaders/model.frag");

  /* Models */
  Model scene("res/objects/Map_v1/Map_v1.obj");
  for (const Mesh &mesh : scene.meshes) {
    Physics::register_static_mesh(mesh.vertices, mesh.indices, glm::mat4(1.0f));
  }

  /* Objects */

  // Test Sphere
  float                 radius   = 0.5f;
  int                   segments = 64;
  std::vector<Vertex>   sphere_v = Util::generate_sphere_vertices(radius, segments);
  std::vector<uint32_t> sphere_i = Util::generate_sphere_indices(segments);
  std::vector<Texture>  sphere_t = std::vector<Texture>();

  Mesh test_sphere = Mesh(sphere_v, sphere_i, sphere_t);

  while (!glfwWindowShouldClose(state->window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    state->delta_time  = currentFrame - state->last_frame;
    state->last_frame  = currentFrame;

    /* Update Game */

    Input::update();
    state->player.update(state->delta_time, state->camera);
    Physics::update(state->delta_time);

    /* Render Game */

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Shaders
    shader_model.use();

    // Light Uniforms
    // Directional
    shader_model.setVec3("viewPos", state->player.get_pos());
    shader_model.setFloat("shininess", 32.0f);
    shader_model.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader_model.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader_model.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader_model.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // FlashLight
    shader_model.setBool("flashLight.is_on", state->player.get_flashlight_on());
    shader_model.setVec3("flashLight.position", state->player.get_pos());
    shader_model.setVec3("flashLight.direction", state->camera.get_front());
    shader_model.setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
    shader_model.setVec3("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader_model.setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
    shader_model.setFloat("flashLight.constant", 1.0f);
    shader_model.setFloat("flashLight.linear", 0.09f);
    shader_model.setFloat("flashLight.quadratic", 0.032f);
    shader_model.setFloat("flashLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader_model.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    // Per Frame Transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(state->camera.get_zoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(state->player.get_pos(),
                                 state->player.get_pos() + state->camera.get_front(),
                                 state->camera.get_up());
    shader_model.setMat4("projection", projection);
    shader_model.setMat4("view", view);

    // Scene
    glm::mat4 model_scene = glm::mat4(1.0f);
    shader_model.setMat4("model", model_scene);
    scene.Draw(shader_model);

    // Test Sphere
    glm::mat4 model_test_sphere = glm::mat4(1.0f);
    model_test_sphere           = glm::translate(model_test_sphere, glm::vec3(PLAYER_SPAWN_POS));
    model_test_sphere           = glm::translate(model_test_sphere, glm::vec3(0.0f, 0.0f, -3.0f));
    shader_model.setMat4("model", model_test_sphere);
    test_sphere.Draw(shader_model);

    /* -- */
    glfwSwapBuffers(state->window);
    glfwPollEvents();

    // Shutdown
    if (Input::key_pressed(SFPS_KEY_ESCAPE)) {
      break;
    }
  }

  glfwTerminate();

  shutdown_game();

  delete state;

  return 0;
}

bool init_window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  state->window = glfwCreateWindow(WIDTH, HEIGHT, "Simple FPS", NULL, NULL);
  if (state->window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(state->window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  int fbWidth, fbHeight;
  glfwGetFramebufferSize(state->window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);
  glfwSetFramebufferSizeCallback(state->window, framebuffer_size_callback);

  glfwSetCursorPosCallback(state->window, mouse_callback);
  glfwSetScrollCallback(state->window, scroll_callback);

  glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Enables
  glEnable(GL_DEPTH_TEST);

  return true;
}

void init_game() {
  // Physics
  Physics::init();

  // Player
  state->player.init(PLAYER_SPAWN_POS);

  // Camera
  state->camera = Camera(state->player.get_pos());

  // Input
  Input::init(state->window);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (state->first_mouse) {
    state->lastX       = xpos;
    state->lastY       = ypos;
    state->first_mouse = false;
  }

  float xoffset = xpos - state->lastX;
  float yoffset = state->lastY - ypos;

  state->lastX = xpos;
  state->lastY = ypos;

  state->camera.process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  state->camera.process_mouse_scroll(static_cast<float>(yoffset));
}

void shutdown_game() {
  Physics::shutdown();
}
