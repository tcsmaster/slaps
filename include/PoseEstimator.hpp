#pragma once

#include <filesystem>
#include <opencv2/core.hpp>
#include <openvino/openvino.hpp>
#include <string>
#include <vector>

// Loads an ONNX pose model directly through OpenVINO (not ONNX Runtime) so
// that image preprocessing (resize/color/layout/normalize) can be baked into
// the compiled graph via ov::preprocess::PrePostProcessor and executed on
// `device` instead of on the CPU via OpenCV.
class PoseEstimator {
public:
  PoseEstimator(const std::filesystem::path &model_path,
                const std::string &device = "GPU");

  // Runs inference on a raw BGR8 HWC frame straight from cv::VideoCapture.
  // Any resolution is accepted; resizing to the model's expected input size
  // happens inside the compiled graph.
  std::vector<float> infer(const cv::Mat &bgr_frame);

private:
  ov::Core core_;
  ov::CompiledModel compiled_model_;
  ov::InferRequest infer_request_;
};
