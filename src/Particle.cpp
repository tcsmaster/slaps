#include <cstddef>
#include <cstdlib>
#include <glad/glad.h>

#include "../include/particle.hpp"
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
void ParticleSystem::RefillDeadParticle(RNG *rng_engine) {
  Particle &p = particles[LastDeadElement];
  glm::vec3 new_offset = rng_engine->randomvec3(-1, 1);
  glm::vec3 new_velocity = rng_engine->randomvec3(-1, 1);
  p.offset = p.offset + new_offset * 0.1f;
  p.velocity = p.velocity + new_velocity * 0.1f;
};
