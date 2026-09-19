#pragma once

#include "shader_s.hpp"
#include <cstdlib>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <array>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
constexpr int starting_particles{100};
struct Particle {
  glm::vec3 offset;
  glm::vec3 velocity;
  float phi;
  float theta;
  float life;
  Particle()
      : offset(glm::vec3(0.f)), velocity(glm::vec3(0.f)), phi(0.f),
        theta(glm::radians(90.f)), life(6.f) {};
  Particle(glm::vec3 offset, glm::vec3 velocity, float phi, float theta,
           float life)
      : offset(offset), velocity(velocity), phi(phi), theta(theta), life(life) {
  }
};
class ParticleSystem {
public:
  GLuint VAO;
  ParticleSystem(std::vector<Particle> &particles);
  // exercise: define all 5
  ParticleSystem(const ParticleSystem &mesh) = default;
  ParticleSystem &operator=(const ParticleSystem &mesh) = default;

private:
  // render data
  GLuint particlesSSBO, VBO, EBO;
  static constexpr std::array<float, 24> quad_vertices{
      // coordinates(3) normals(3)
      -0.1f, 0.1f,  0.f, 0.f, 0.f, 1.0f, 0.1f,  0.1f,  0.f, 0.f, 0.f, 1.0f,
      0.1f,  -0.1f, 0.f, 0.f, 0.f, 1.0f, -0.1f, -0.1f, 0.f, 0.f, 0.f, 1.0f};
  static constexpr std::array<GLuint, 6> indices{0, 1, 2, 0, 2, 3};
  std::size_t LastDeadElement = 0;
  std::vector<Particle> particles;
  void render(Shader &vec_frag_shader, Shader &comp_shader);
  void findLastDeadParticle();
  void RefillDeadParticle(Particle particle);
  void setupParticleSystem();
  void calculate_velocity();
  void calculate_offsets(const float time_step);
  static glm::vec3 position_mapping(glm::vec3 &position);
  void boundary_check();
};
