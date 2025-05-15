#include "Player/player.h"
#include "camera.h"
#include "model.h"
#include "shader.h"
#include <cstdlib>
#include <cstring>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <GLFW/glfw3.h>

bool init_window();
void init_state();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void destroy_state();

// Game
const unsigned int WIDTH  = 800;
const unsigned int HEIGHT = 600;

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
  init_state();

  if (!init_window()) {
    return -2;
  }

  // Shaders
  Shader shader("shaders/vert.glsl", "shaders/frag.glsl");

  // Models
  Model scene("res/objects/Map_v1/Map_v1.obj");

  while (!glfwWindowShouldClose(state->window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    state->delta_time  = currentFrame - state->last_frame;
    state->last_frame  = currentFrame;

    process_input(state->window);

    /* Render */

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    /* Transforms */
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    // View Matrix
    glm::mat4 view = glm::lookAt(
        state->player.get_pos(), state->player.get_pos() + state->camera.Front, state->camera.Up);
    glm::mat4 projection = glm::perspective(
        glm::radians(state->camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Draw call
    scene.Draw(shader);

    glfwSwapBuffers(state->window);
    glfwPollEvents();
  }

  glfwTerminate();

  destroy_state();

  return 0;
}

bool init_window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  state->window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
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

void init_state() {
  state = new game_state{};

  // Player
  state->player.init(glm::vec3(10.0f, 2.0f, 0.0f));

  // Camera
  state->camera = Camera(state->player.get_pos());
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    state->player.pressed_move_forward(state->delta_time, state->camera);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    state->player.pressed_move_backward(state->delta_time, state->camera);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    state->player.pressed_move_left(state->delta_time, state->camera);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    state->player.pressed_move_right(state->delta_time, state->camera);
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

  state->camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  state->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void destroy_state() {
  delete state;
}
