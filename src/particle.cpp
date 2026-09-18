#include <algorithm>
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
#include <algorithm>
#include <array>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
namespace Particle {
ParticleSystem::ParticleSystem(std::vector<glm::vec3> &offsets) {
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
  // TODO: incorporate the position translation into the model matrix, and
  // only update the model matrix on the gpu using glbuffersubdata

  //  set the vertex attribute pointers
  //  vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (const GLvoid *)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float),
                        (const GLvoid *)(3 * sizeof(float)));
  // instanceVBO;
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  std::size_t vec4Size = sizeof(glm::vec4);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        (const GLvoid *)0);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        (const GLvoid *)(vec4Size));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        (const GLvoid *)(2 * vec4Size));
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        (const GLvoid *)(3 * vec4Size));

  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void ParticleSystem::findLastDeadParticle() {
  for (unsigned int i = LastDeadElement; i < particles.size(); ++i) {
    if (particles[i].life <= 0.0f) {
      LastDeadElement = i;
    }
  }
  // otherwise, do a linear search
  for (unsigned int i = 0; i < LastDeadElement; ++i) {
    if (particles[i].life <= 0.0f) {
      LastDeadElement = i;
    }
  }
  // override first particle if all others are alive
  LastDeadElement = 0;
}
void ParticleSystem::RefillDeadParticle(Particle &particle) {
  Particle &p = particles[LastDeadElement];
  p.offset = particle.offset + rand() * 0.1;
  p.velocity = particle.velocity + rand() * 0.1;
  // TODO: finish this
};
}; // namespace Particle
