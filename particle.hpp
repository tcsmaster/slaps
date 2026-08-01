#pragma once

#include <cstddef>
#include <glad/glad.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <array>
#include <glm/trigonometric.hpp>
const int NUM_PARTICLES{100};
class Mesh {
public:
  std::array<glm::vec3, NUM_PARTICLES> offsets;
  std::array<glm::vec3, NUM_PARTICLES> velocities;
  std::array<float, NUM_PARTICLES> accelerations;
  std::array<float, NUM_PARTICLES> speeds{};
  std::array<glm::mat4, NUM_PARTICLES> model_matrices;
  static constexpr std::array<float, 12> quad_vertices{
      -0.1f, 0.1f, 0.f, 0.1f, 0.1f, 0.f, 0.1f, -0.1f, 0.f, -0.1f, -0.1f, 0.f};
  static constexpr std::array<GLuint, 6> indices{0, 1, 2, 0, 2, 3};
  static constexpr std::array<glm::vec3, 5> vortices{
      glm::vec3(.0f, .0f, .0f), glm::vec3(.5f, .5f, .0f),
      glm::vec3(-.5f, .5f, .0f), glm::vec3(.5f, -.5f, .0f),
      glm::vec3(-.5f, .5f, .0f)};
  GLuint VAO;
  Mesh(std::array<glm::vec3, NUM_PARTICLES> offsets) : offsets{offsets} {
    for (auto &matrix : model_matrices) {
      matrix = glm::mat4(1.f);
    }
    for (auto &acc : speeds) {
      acc = 0.f;
    }
    for (auto &acc : accelerations) {
      acc = 0.f;
    }
    setupMesh();
  }
  // TODO: Gather data from camera, update the parameters and send position to
  // gpu w/ glbuffersubdata
  void update() {
    calculate_velocity();
    calculate_offsets(0.01f);
    boundary_check();
    create_model_matrices();
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model_matrices),
                    &model_matrices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  void draw() {
    update();
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_PARTICLES);
    glBindVertexArray(0);
  }

private:
  // render data
  GLuint instanceVBO, VBO, EBO;

  void setupMesh() {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (const GLvoid *)0);
    // instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_matrices), model_matrices.data(),
                 GL_DYNAMIC_DRAW);
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (const GLvoid *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (const GLvoid *)(vec4Size));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (const GLvoid *)(2 * vec4Size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (const GLvoid *)(3 * vec4Size));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  void create_model_matrices() {
    for (std::size_t i{0}; i < model_matrices.size(); i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, offsets.at(i));
      model = glm::rotate(model, glm::radians(45.0f), velocities.at(i));
      model_matrices.at(i) = model;
    }
  }
  void calculate_velocity() {
    for (std::size_t i{0}; i < NUM_PARTICLES; i++) {
      // TODO: create the mapping from coordinate to velocity
      velocities.at(i) = position_mapping(offsets.at(i));
    }
  }
  void calculate_offsets(const float time_step) {
    for (std::size_t i{0}; i < offsets.size(); i++) {
      offsets.at(i) =
          offsets.at(i) + velocities.at(i) * time_step +
          glm::vec3(0.5f * accelerations.at(i) * time_step * time_step);
    }
  }
  static glm::vec3 position_mapping(glm::vec3 &position) {
    glm::vec3 v(.0f);
    for (const auto &vortex : vortices) {
      glm::vec3 diff = position - vortex;
      float r2 = glm::dot(diff, diff) + 0.1f;
      v += glm::vec3(-diff.y, diff.x, .0f) / r2;
    }
    return v;
  }
  void boundary_check() {
    for (auto &offset : offsets) {
      if (offset.x > 1.f) {
        offset.x = -1.f;
      } else if (offset.x < -1.f) {
        offset.x = 1.f;
      } else if (offset.y > 1.f) {
        offset.y = -1.f;
      } else if (offset.y < -1.f) {
        offset.y = 1.f;
      }
    }
  }
};
