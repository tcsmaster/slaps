#include "glad/glad.h"
#include "particle.hpp"
#include "shader_s.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include <cstddef>
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
  // render loop
  std::array<glm::vec3, NUM_PARTICLES> offsets;
  constexpr std::array<float, 10> coords{-.91f, -.69f, -.5f, -.3f, -1.5f,
                                         .0f,   .2f,   .4f,  .6f,  .7f};
  for (std::size_t i{0}; i < NUM_PARTICLES; i++) {
    int x_c = i / 10;
    int y_c = i % 10;
    offsets[i] = glm::vec3(coords.at(y_c), -coords.at(x_c), .0f);
  }
  Mesh mesh(offsets);
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw 100 instanced quads
    auto view = glm::translate(glm::mat4(1.f), glm::vec3(.0f, .0f, -3.0f));
    auto perspective = glm::perspective(
        glm::radians(45.f),
        static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 10.f);
    shader.use();
    perspective = perspective * view;
    shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);
    shader.setVec3("viewPos", glm::vec3(0.f, 0.f, -0.1f));
    shader.setMat4("view", perspective);
    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.direction", 0.f, .0f, -1.f);
    mesh.draw();
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
