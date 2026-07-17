#include "camera.hpp"
#include "glad/glad.h"
#include "shader_s.hpp"
#include <GLFW/glfw3.h>
#include <array>
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
// TODO: create a templated Shader class for separate vertex and fragment
// shaders

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

  // store instance data (per-instance model matrices) in an array buffer
  // ----------------------------------------------------------------------
  GLuint modelsVBO;
  glGenBuffers(1, &modelsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, modelsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(models), &models[0], GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float quadVertices[] = {
      // positions     // colors
      -0.05f, 0.05f, 0.f,  1.0f,   0.0f,   0.0f, 0.05f, -0.05f, 0.f,
      0.0f,   1.0f,  0.0f, -0.05f, -0.05f, 0.f,  0.0f,  0.0f,   1.0f,

      -0.05f, 0.05f, 0.f,  1.0f,   0.0f,   0.0f, 0.05f, -0.05f, 0.f,
      0.0f,   1.0f,  0.0f, 0.05f,  0.05f,  0.f,  0.0f,  1.0f,   1.0f};
  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  // per-instance model matrix attribute (occupies locations 3,4,5,6 - one vec4
  // per column)
  glBindBuffer(GL_ARRAY_BUFFER, modelsVBO);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                        (void *)(sizeof(glm::vec4)));
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                        (void *)(2 * sizeof(glm::vec4)));
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                        (void *)(3 * sizeof(glm::vec4)));

  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  int inde = 0;
  Shader shader("4.5.shader.vert", "4.5.shader.frag");
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw 100 instanced quads
    glBindBuffer(GL_ARRAY_BUFFER, modelsVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(models), &models[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    shader.use();
    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_PARTICLES);
    glBindVertexArray(0);
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse
    // moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &modelsVBO);

  glfwTerminate();
  return 0;
}
