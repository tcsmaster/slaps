#ifndef MESH_H
#define MESH_H

#include <cstddef>
#include <glad/glad.h>

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.hpp"

#include <string>
#include <vector>
const float SIDE_LENGTH_HORIZONTAL{0.05f};
const float SIDE_LENGTH_VERTICAL{0.01f};
float quadVertices[] = {
    // positions
    -0.05f, 0.05f,  0.f, 0.05f,  -0.05f, 0.f,
    -0.05f, -0.05f, 0.f, -0.05f, 0.05f,  0.f};
class Mesh {
public:
  std::vector<glm::vec3> velocities;
  std::vector<GLuint> indices;
  std::vector<float> speeds;
  std::vector<float> accelerations;
  GLuint VAO;

  // constructor
  Mesh(std::vector<glm::vec3> velocities, std::vector<GLuint> indices,
       std::vector<float> speeds, std::vector<float> accelerations) {
    this->velocities = velocities;
    this->indices = indices;
    this->speeds = speeds;
    this->accelerations = accelerations;

    setupMesh();
  }

  // render the mesh
  void Draw() {
    // draw mesh
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
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Position));
    // vertex texture coords
    glBindVertexArray(0);
  }
  glm::vec3 position_mapping(glm::vec3 position);
  void calculate_velocity(std::vector<glm::vec3> velocities, const std::vector<glm::vec3>& positions){
    if (positions.size() != velocities.size()) {
      std::cerr << "Particle position update failed: Expected the number of "
                   "particles to all have velocities. #Velocities"
                << velocities.size() << ", #Particles: " << positions.size()
                << std::endl;
      exit(1);
    }
    for(std::size_t i{0};i < positions.size(); i++){
	//TODO: create the mapping from coordinate to velocity	
	velocities.at(i) = position_mapping(positions.at(i));
    }
  }
  void calculate_position(std::vector<glm::vec3> &positions,
                          const float time_step) {
    if (positions.size() != velocities.size()) {
      std::cerr << "Particle position update failed: Expected the number of "
                   "particles to all have velocities. #Velocities"
                << velocities.size() << ", #Particles: " << positions.size()
                << std::endl;
      exit(1);
    }
    for (std::size_t i{0}; i < velocities.size(); i++) {
      positions.at(i) =
          velocities.at(i) * time_step +
          glm::vec3(0.5f * accelerations.at(i) * time_step * time_step);
    }
  }
  glm::vec3 position_mapping(glm::vec3 position){
		
  }
};
#endif
