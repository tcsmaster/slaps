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

#include "../include/particle.hpp"
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
const std::vector<Particle> ParticleSystem::getParticles() const {
  return particles;
}
const std::array<float, 24> ParticleSystem::getVertices() const {
  return quad_vertices;
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
