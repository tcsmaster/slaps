#include "glad/glad.h"
#include "shader_s.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
// TODO: create a templated Shader class for separate vertex and fragment
// shaders

const int WIDTH{800};
const int HEIGHT{600};
const int NUM_PARTICLES{3};
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
  float vertices[] = {-.8f, -.1f, .0f, -.8f, .1f,  .0f,
                      .8f,  .1f,  .0f, .8f,  -.1f, .0f};
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};
  /*
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<std::mt19937::result_type>
  dist_angle(0.f, 1.f);
  */
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glEnableVertexArrayAttrib(VAO, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  Shader shader("4.5.shader.vert", "4.5.shader.frag");
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glm::mat4 trans = glm::mat4(1.f);
    trans = glm::rotate(trans, (float)glfwGetTime()*glm::radians(50.f), glm::vec3(.0f, 1.f, .0f));
    shader.use();
    shader.setMat4("transformation", trans);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
