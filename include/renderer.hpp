#pragma once

#include "particle.hpp"
#include "shader_s.hpp"
#include <array>
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
class Renderer {
public:
  void initializeOpenGL();
  void initializeBufferIDs();
  void initializeVertexBuffer(std::vector<float> &vertices,
                              std::array<GLuint, 6> &indices);
  void initializeParticleBuffer(std::vector<Particle> &particles);
  void initializePoseBuffer(std::vector<float> &keypoints);
  void renderParticles(Shader &frag_vec_shader, Shader &comp_shader,
                       std::array<GLuint, 6> &indices,
                       std::vector<Particle> &particles);
  void UpdatePoseEstimatorData(std::vector<float> &keypoints);
  void shutdown();
  void updateCameraFrame();
  void updatePose();
  void updateParticles();

private:
  // render data
  GLuint ParticleVAO, particlesSSBO, VBO, EBO, posecoordinatesVBO;
};
