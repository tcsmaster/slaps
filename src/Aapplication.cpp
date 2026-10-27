#include "../include/Application.hpp"

Application::Application() {
  poseEstimator_ = new PoseEstimator("models/pose.onnx");
  renderer_ = new Renderer();

  renderer_->initialize();
  poseEstimator_->initialize();
}

Application::~Application() {
  running_ = false;

  if (cameraThread_.joinable())
    cameraThread_.join();

  if (inferenceThread_.joinable())
    inferenceThread_.join();

  delete renderer_;
  delete poseEstimator_;
  delete camera_;
}

void Application::run() {
  running_ = true;

  cameraThread_ = std::thread(&Application::cameraLoop, this);

  inferenceThread_ = std::thread(&Application::inferenceLoop, this);

  // OpenGL stays on this/main thread.
  renderer_->run();
}

void Application::cameraLoop() {
  while (running_) {
    // Capture camera frame
    // Put latest frame into shared buffer
  }
}

void Application::inferenceLoop() {
  while (running_) {
    // Get latest camera frame
    // Run OpenVINO inference
    // Put latest pose into shared buffer
  }
}
