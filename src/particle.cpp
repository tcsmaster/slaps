#include <cstddef>
#include <cstdlib>
#include <glad/glad.h>

#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include "particle.hpp"
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
ParticleSystem::ParticleSystem(std::vector<Particle> &particles) {
  setupParticleSystem();
}
void ParticleSystem::setupParticleSystem() {
  // create buffers/arrays
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  // load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, quad_vertices.size() * sizeof(float),
               quad_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (const GLvoid *)0);
  // TODO: finish the SSBO;
  glGenBuffers(1, &particlesSSBO);
  glBindBuffer(GL_ARRAY_BUFFER, particlesSSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size(), particles.data(),
               GL_DYNAMIC_READ);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void ParticleSystem::render(Shader &frag_vec_shader, Shader &comp_shader) {
  comp_shader.use();
  glDispatchCompute(512, 512, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  // TODO: decide if this needs lighting at all
  // TODO: also set viewPos once.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  frag_vec_shader.use();
  frag_vec_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
  frag_vec_shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
  frag_vec_shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  frag_vec_shader.setFloat("material.shininess", 32.0f);
  frag_vec_shader.setVec3("viewPos", glm::vec3(0.f, 0.f, -2.f));
  frag_vec_shader.setVec3("viewPos", glm::vec3(0.f, 0.f, -2.f));
  frag_vec_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  frag_vec_shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  frag_vec_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
  frag_vec_shader.setVec3("light.direction", 0.f, .0f, -1.f);
  GL_DRAW_ELEMENTS_INSTANCED();
}
void ParticleSystem::findLastDeadParticle() {
  for (std::size_t i{LastDeadElement}; i < particles.size(); ++i) {
    if (particles[i].life <= 0.0f) {
      LastDeadElement = i;
    }
  }
  // otherwise, do a linear search
  for (std::size_t i{0}; i < LastDeadElement; ++i) {
    if (particles[i].life <= 0.0f) {
      LastDeadElement = i;
    }
  }
  // override first particle if all others are alive
  LastDeadElement = 0;
}
void ParticleSystem::RefillDeadParticle() {
  Particle &p = particles[LastDeadElement];
  p.offset = p.offset + rand() * 0.1;
  p.velocity = p.velocity + rand() * 0.1;
  // TODO: finish this
};
