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
#include <array>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
namespace Particle {
Mesh::Mesh(std::vector<glm::vec3> &offsets)
    : offsets{offsets}, velocities(NUM_PARTICLES), accelerations(NUM_PARTICLES),
      speeds(NUM_PARTICLES), model_matrices(NUM_PARTICLES) {
  for (auto &matrix : model_matrices) {
    matrix = glm::mat4(1.f);
  }
  for (auto &acc : speeds) {
    acc = 0.1f;
  }
  for (auto &acc : accelerations) {
    acc = 0.f;
  }
  setupMesh();
}
// TODO: Gather data from camera, update the parameters and send position to
// gpu w/ glbuffersubdata
void Mesh::update() {
  // TODO: prolly insert the optical flow here
  calculate_velocity();
  calculate_offsets(0.01f);
  boundary_check();
  create_model_matrices();
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, model_matrices.size() * sizeof(glm::mat4),
                  &model_matrices[0]);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::draw() {
  update();
  glBindVertexArray(VAO);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_PARTICLES);
  glBindVertexArray(0);
}

void Mesh::setupMesh() {
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
  glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4),
               model_matrices.data(), GL_DYNAMIC_DRAW);
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
void Mesh::create_model_matrices() {
  for (std::size_t i{0}; i < model_matrices.size(); i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, offsets.at(i));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(.0f, 0.f, 1.f));
    model_matrices.at(i) = model;
  }
}
void Mesh::calculate_velocity() {
  for (std::size_t i{0}; i < NUM_PARTICLES; i++) {
    // TODO: create the mapping from coordinate to velocity
    // amíg van optical flow vector (pl. vektor hossz nagyobb, mint egy
    // threshold), addig az a velocity, utána vissza a normálba
    velocities.at(i) = position_mapping(offsets.at(i));
  }
}
void Mesh::calculate_offsets(const float time_step) {
  for (std::size_t i{0}; i < offsets.size(); i++) {
    offsets.at(i) =
        offsets.at(i) + speeds.at(i) * velocities.at(i) * time_step +
        glm::vec3(0.5f * accelerations.at(i) * time_step * time_step);
  }
}
glm::vec3 Mesh::position_mapping(glm::vec3 &position) {
  glm::vec3 v(.0f);
  v += glm::vec3(-1.f, .0f, .5f);
  v += glm::vec3(-0.3f, -0.5f, .5f);
  return v;
}
void Mesh::boundary_check() {
  for (auto &offset : offsets) {
    if (offset.x > 1.f) {
      offset.x = -1.f;
    } else if (offset.x < -1.f) {
      offset.x = 1.f;
    }
    if (offset.y > 1.f) {
      offset.y = -1.f;
    } else if (offset.y < -1.f) {
      offset.y = 1.f;
    }
    if (offset.z > 1.f) {
      offset.z = -1.f;
    } else if (offset.z < -1.f) {
      offset.z = 1.f;
    }
  }
}
}; // namespace Particle
