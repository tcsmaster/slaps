#pragma once

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
namespace Particle {
constexpr int NUM_PARTICLES{100};
class Mesh {
public:
  GLuint VAO;
  Mesh(std::vector<glm::vec3> &offsets);
  // exercise: define all 5
  Mesh(const Mesh &mesh) = default;
  Mesh &operator=(const Mesh &mesh) = default;
  void update();
  void draw();

private:
  // render data
  GLuint instanceVBO, VBO, EBO;
  static constexpr std::array<float, 24> quad_vertices{
      // coordinates(3) normals(3)
      -0.1f, 0.1f,  0.f, 0.f, 0.f, 1.0f, 0.1f,  0.1f,  0.f, 0.f, 0.f, 1.0f,
      0.1f,  -0.1f, 0.f, 0.f, 0.f, 1.0f, -0.1f, -0.1f, 0.f, 0.f, 0.f, 1.0f};
  static constexpr std::array<GLuint, 6> indices{0, 1, 2, 0, 2, 3};
  std::vector<glm::vec3> offsets;
  std::vector<glm::vec3> velocities;
  std::vector<float> accelerations;
  std::vector<float> speeds;
  std::vector<glm::mat4> model_matrices;
  void setupMesh();
  void create_model_matrices();
  void calculate_velocity();
  void calculate_offsets(const float time_step);
  static glm::vec3 position_mapping(glm::vec3 &position);
  void boundary_check();
};
} // namespace Particle
