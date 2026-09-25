#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstdlib>
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/trigonometric.hpp>

#include "../include/particle.hpp"
#include "../include/renderer.hpp"
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
void Renderer::initializeOpenGL() {
  const int WIDTH{800};
  const int HEIGHT{600};
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
}
void Renderer::initializeBufferIDs() {
  glGenVertexArrays(1, &ParticleVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &particlesSSBO);
}
void Renderer::initializeVertexBuffer(std::vector<float> &vertices,
                                      std::array<GLuint, 6> &indices) {
  glBindVertexArray(ParticleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (const GLvoid *)0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Renderer::initializeParticleBuffer(std::vector<Particle> &particles) {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particlesSSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size(), particles.data(),
               GL_DYNAMIC_READ);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}
void Renderer::initializePoseBuffer(std::vector<float> &keypoints) {
  glBindBuffer(GL_ARRAY_BUFFER, posecoordinatesVBO);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void Renderer::renderParticles(Shader &frag_vec_shader, Shader &comp_shader,
                               std::array<GLuint, 6> &indices,
                               std::vector<Particle> &particles) {
  comp_shader.use();
  glDispatchCompute(512, 512, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  // TODO: decide if this needs lighting at all
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  frag_vec_shader.use();
  frag_vec_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
  frag_vec_shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
  frag_vec_shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  frag_vec_shader.setFloat("material.shininess", 32.0f);
  frag_vec_shader.setVec3("viewPos", glm::vec3(0.f, 0.f, -2.f));
  frag_vec_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  frag_vec_shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  frag_vec_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
  frag_vec_shader.setVec3("light.direction", 0.f, .0f, -1.f);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices.data(),
                          particles.size());
}
