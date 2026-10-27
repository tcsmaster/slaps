#pragma once

#include <cstdlib>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <random>

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
class RNG {
public:
  // Access the single global instance
  static RNG &instance() {
    static RNG rng; // constructed once, thread-safe since C++11
    return rng;
  }

  // Delete copy/move so nobody accidentally duplicates state
  RNG(const RNG &) = delete;
  RNG &operator=(const RNG &) = delete;

  // Integer in [min, max] inclusive
  int uniformInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine_);
  }

  // Real number in [min, max)
  double uniformReal(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine_);
  }
  glm::vec3 randomvec3(float min, float max) {
    std::uniform_real_distribution<float> inp(min, max);
    float x = inp(engine_);
    float y = inp(engine_);
    float z = inp(engine_);
    return glm::vec3(x, y, z);
  }
  // Access raw engine if you need something else (e.g. std::shuffle)
  std::mt19937 &engine() { return engine_; }

  // Optional: reseed for reproducible tests
  void seed(unsigned s) { engine_.seed(s); }

private:
  RNG() : engine_(std::random_device{}()) {}
  std::mt19937 engine_;
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
  void RefillDeadParticle(RNG *rng_engine);
};
