#ifndef MESH_H
#define MESH_H

#include <cstddef>
#include <glad/glad.h>

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.hpp"

#include <array>
const float SIDE_LENGTH_HORIZONTAL{0.05f};
const float SIDE_LENGTH_VERTICAL{0.01f};
const int NUM_PARTICLES{100};
float quadVertices[] = {
    // positions
    -0.05f, 0.05f,  0.f, 0.05f,  -0.05f, 0.f,
    -0.05f, -0.05f, 0.f, -0.05f, 0.05f,  0.f};
struct Vertex {
  glm::vec3 position;
  glm::vec3 velocity;
};
class Mesh {
public:
  std::array<Vertex, NUM_PARTICLES> vertices;
  std::array<GLuint, NUM_PARTICLES> indices;
  std::array<float, NUM_PARTICLES> accelerations{};
  std::array<float, NUM_PARTICLES> speeds{0.01f};
  GLuint VAO;
  Mesh(std::array<Vertex, NUM_PARTICLES> vertices,
       std::array<GLuint, NUM_PARTICLES> indices)
      : vertices{vertices}, indices{indices} {
    setupMesh();
  }
  // TODO: Gather data from camera, update the parameters adn send position to gpu w/ glbuffersubdata
  void update();
  // render the mesh
  void Draw() {
    update();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  // render data
  unsigned int VBO, EBO;

  // initializes all the buffer objects/arrays
  void setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex velocity
    glBindVertexArray(0);
  }
  static glm::vec3 position_mapping(glm::vec3 position);
  void calculate_velocity(std::vector<Vertex> vertices) {
    for (auto &vertex : vertices) {
      // TODO: create the mapping from coordinate to velocity
      vertex.position = position_mapping(vertex.position);
    }
  }
  void calculate_position(std::vector<Vertex> &verticies,
                          const float time_step) {
    for (auto &vertex : verticies) {
      vertex.position =
          vertex.position + vertex.velocity * time_step +
          glm::vec3(0.5f * vertex.acceleration * time_step * time_step);
    }
  }
  glm::vec3 position_mapping(glm::vec3 position) {}
};
#endif
