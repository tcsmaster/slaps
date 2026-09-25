#pragma once

#include <glad/glad.h>
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

#include <array>
#include <vector>
constexpr int starting_particles{100};
struct Particle {
  glm::vec3 offset;
  glm::vec3 velocity;
  float homeLayer;   // which layer this belongs to (e.g. 0, 1, 2)
  float layerOffset; // dynamic, decays back to 0 over time
  float influence;
  float life;
  Particle()
      : offset(glm::vec3(0.f)), velocity(glm::vec3(0.f)), layerOffset(0.f),
        influence(0.f), life(6.f) {};
  Particle(glm::vec3 offset, glm::vec3 velocity, float homeLayer,
           float layerOffset, float influence, float life)
      : offset(offset), velocity(velocity), homeLayer(homeLayer),
        layerOffset(layerOffset), influence(influence), life(life) {}
};
class ParticleSystem {
public:
  ParticleSystem(std::vector<Particle> &particles);
  const std::vector<Particle> getParticles() const;
  const std::array<float, 24> getVertices() const;
  const std::array<GLuint, 6> getIndices() const;
  // exercise: define all 5
  ParticleSystem(const ParticleSystem &mesh) = default;
  ParticleSystem &operator=(const ParticleSystem &mesh) = default;

private:
  // render data
  static constexpr std::array<float, 24> quad_vertices{
      // coordinates(3) normals(3)
      -0.1f, 0.1f,  0.f, 0.f, 0.f, 1.0f, 0.1f,  0.1f,  0.f, 0.f, 0.f, 1.0f,
      0.1f,  -0.1f, 0.f, 0.f, 0.f, 1.0f, -0.1f, -0.1f, 0.f, 0.f, 0.f, 1.0f};
  static constexpr std::array<GLuint, 6> indices{0, 1, 2, 0, 2, 3};
  std::size_t LastDeadElement = 0;
  std::vector<Particle> particles;
  void findLastDeadParticle();
  void RefillDeadParticle();
};
