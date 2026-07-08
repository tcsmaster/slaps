#include "camera.hpp"
#include "glad/glad.h"
#include "shader_s.hpp"
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

  glm::vec2 translations[100];
  int index = 0;
  float offset = 0.1f;
  for (int y = -10; y < 10; y += 2) {
    for (int x = -10; x < 10; x += 2) {
      glm::vec2 translation;
      translation.x = (float)x / 10.0f + offset;
      translation.y = (float)y / 10.0f + offset;
      translations[index++] = translation;
    }
  }

  // store instance data in an array buffer
  // --------------------------------------
  unsigned int instanceVBO;
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float quadVertices[] = {
      // positions     // colors
      -0.05f, 0.05f, 1.0f,   0.0f,   0.0f, 0.05f, -0.05f, 0.0f,
      1.0f,   0.0f,  -0.05f, -0.05f, 0.0f, 0.0f,  1.0f,

      -0.05f, 0.05f, 1.0f,   0.0f,   0.0f, 0.05f, -0.05f, 0.0f,
      1.0f,   0.0f,  0.05f,  0.05f,  0.0f, 1.0f,  1.0f};
  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  // also set instance data
  glEnableVertexAttribArray(2);
  glBindBuffer(
      GL_ARRAY_BUFFER,
      instanceVBO); // this attribute comes from a different vertex buffer
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glVertexAttribDivisor(
      2, 1); // tell OpenGL this is an instanced vertex attribute.
  Shader shader("4.5.shader.vert", "4.5.shader.frag");
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw 100 instanced quads
    shader.use();
    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                          100); // 100 triangles of 6 vertices each
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);

  glfwTerminate();
  return 0;
}
