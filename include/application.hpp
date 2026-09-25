#pragma once

#include "PoseEstimator.hpp"
#include "particle.hpp"
class Application {

public:
  void updateKeypointBuffer(PoseEstimator &estimator);
  void updateParticleBuffer(ParticleSystem &particlesystem);
};
