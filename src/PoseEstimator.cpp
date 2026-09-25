#include "../include/PoseEstimator.hpp"
#include <openvino/core/preprocess/pre_post_process.hpp>
#include <stdexcept>

namespace {

// The model's own input layout can be NCHW or NHWC depending on how it was
// exported; find the axis of size 3 (RGB channels) to tell them apart.
ov::Layout detect_model_layout(const ov::PartialShape &shape) {
  if (shape.rank().get_length() != 4) {
    throw std::runtime_error("PoseEstimator: expected a 4D model input");
  }
  if (shape[1].is_static() && shape[1].get_length() == 3) {
    return ov::Layout("NCHW");
  }
  if (shape[3].is_static() && shape[3].get_length() == 3) {
    return ov::Layout("NHWC");
  }
  throw std::runtime_error("PoseEstimator: could not infer model input layout");
}

} // namespace

PoseEstimator::PoseEstimator(const std::filesystem::path &model_path,
                             const std::string &device) {
  if (!std::filesystem::is_regular_file(model_path)) {
    throw std::runtime_error("PoseEstimator: model file " +
                             model_path.string() + " does not exist");
  }

  // OpenVINO's frontend reads .onnx files directly, no ONNX Runtime needed.
  auto model = core_.read_model(model_path.string());
  const auto model_layout =
      detect_model_layout(model->input().get_partial_shape());

  ov::preprocess::PrePostProcessor ppp(model);

  // Describe the buffer that infer() will actually hand over: a BGR8, HWC
  // cv::Mat straight from cv::VideoCapture, at whatever resolution the
  // camera happens to produce.
  ppp.input()
      .tensor()
      .set_shape({1, ov::Dimension::dynamic(), ov::Dimension::dynamic(), 3})
      .set_element_type(ov::element::u8)
      .set_layout("NHWC")
      .set_color_format(ov::preprocess::ColorFormat::BGR);

  // Everything below is compiled into the graph and runs on `device`
  // (e.g. GPU) instead of on the CPU via cv::cvtColor/cv::resize.
  ppp.input()
      .preprocess()
      .convert_color(ov::preprocess::ColorFormat::RGB)
      .convert_element_type(ov::element::f32)
      .resize(ov::preprocess::ResizeAlgorithm::RESIZE_LINEAR)
      .scale(255.0f); // matches pose_landmark's [0, 1] input normalization

  // Tensor layout is NHWC; tell PrePostProcessor what the model itself
  // expects so it inserts the transpose automatically if they differ.
  ppp.input().model().set_layout(model_layout);

  model = ppp.build();

  compiled_model_ = core_.compile_model(model, device);
  infer_request_ = compiled_model_.create_infer_request();
}

std::vector<float> PoseEstimator::infer(const cv::Mat &bgr_frame) {
  if (!bgr_frame.isContinuous() || bgr_frame.type() != CV_8UC3) {
    throw std::runtime_error(
        "PoseEstimator::infer expects a continuous CV_8UC3 BGR frame");
  }

  // Zero-copy: wraps the cv::Mat's own buffer, no intermediate allocation.
  ov::Tensor input_tensor(ov::element::u8,
                          {1, static_cast<size_t>(bgr_frame.rows),
                           static_cast<size_t>(bgr_frame.cols), 3},
                          bgr_frame.data);

  infer_request_.set_input_tensor(input_tensor);
  infer_request_.infer();

  const ov::Tensor &output = infer_request_.get_output_tensor();
  const float *data = output.data<float>();
  return std::vector<float>(data, data + output.get_size());
}
