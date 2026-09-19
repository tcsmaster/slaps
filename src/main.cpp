#include "glad/glad.h"
#include "shader_s.hpp"
#include "src/Rendering/particle.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>

const int WIDTH{800};
const int HEIGHT{600};
const float rect_height{0.2f};
const float rect_width{0.4f};
int main() {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window{
      glfwCreateWindow(WIDTH, HEIGHT, "Slappe", nullptr, nullptr)};
  if (!window) {
    std::cerr << "Failed to create GLHF window!";
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD!" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *, int w, int h) { glViewport(0, 0, w, h); });
  glClearColor(0.0f, 0.8f, 0.4f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  Shader shader("lightning.vert", "lightning.frag");
  glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(.0f, .0f, -3.0f));
  glm::mat4 perspective = glm::ortho(0.f, 800.f, 0.f, 600.f, 0.1f, 100.f);
  perspective = perspective * view;
  shader.setMat4("view", perspective);
  std::random_device r;
  std::default_random_engine gen2(r());
  // render loop
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    shader.use();
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
