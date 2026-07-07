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
const Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
constexpr glm::vec3 lightpos(0.7f, 0.9f, -.4f);
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
  float vertices[] = {
      -.8f, -.1f, .0f, 0.f, 0.f, -1.f, -.8f, .1f,  .0f, 0.f, 0.f, -1.f,
      .8f,  .1f,  .0f, 0.f, 0.f, -1.f, .8f,  -.1f, .0f, 0.f, 0.f, -1.f,
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int VBO, EBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // light cube VAO — reuses VBO's vertex data, needs its own attribute pointer
  // and needs the EBO bound again since it's part of VAO state
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glEnable(GL_DEPTH_TEST);
  Shader shader("4.5.shader.vert", "4.5.shader.frag");
  Shader lightshader("4.5.lightshader.vert", "4.5.lightshader.frag");
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 trans = glm::mat4(1.f);
    glm::mat4 projection = glm::ortho(-1.f,1.f,-1.f,1.f, -5.f, 5.f);
    trans = glm::rotate(trans,
                        static_cast<float>(glfwGetTime()) * glm::radians(45.f),
                        glm::vec3(.0f, 1.f, .0f));
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    shader.use();
    shader.setVec3("viewpos", camera.Position);
    shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);
    shader.setVec3("light.position", lightpos);
    shader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader.setMat4("model", trans);
    shader.setMat4("view", view);
    shader.setMat4("projection", glm::mat4(1.f));
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    lightshader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightpos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightshader.setMat4("model", model);
    lightshader.setMat4("view", view);
    lightshader.setMat4("projection", glm::mat4(1.f));
    glBindVertexArray(lightCubeVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
