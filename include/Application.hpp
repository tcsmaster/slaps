#pragma once

#include <atomic>
#include <thread>

#include "../include/Camera.hpp"
#include "../include/PoseEstimator.hpp"
#include "../include/Renderer.hpp"

class Application {
public:
  Application();
  ~Application();

  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  void run();

private:
  void cameraLoop();
  void inferenceLoop();

  std::atomic<bool> running_{false};

  std::thread cameraThread_;
  std::thread inferenceThread_;

  Camera *camera_ = nullptr;
  PoseEstimator *poseEstimator_ = nullptr;
  Renderer *renderer_ = nullptr;
};
