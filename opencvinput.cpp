#include "opencvinput.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>
#include <opencv2/optflow/rlofflow.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
namespace OpenCVInput {
FlowFunc CalculateOpticalFlow(OpticalFlowMethod method) {
  switch (method) {
  case OpticalFlowMethod::SparseToDense:
    return [](const cv::Mat &prev, const cv::Mat &next, cv::Mat &flow) {
      cv::optflow::calcOpticalFlowSparseToDense(prev, next, flow);
    };
  case OpticalFlowMethod::DenseRLOF: {
    auto denser = cv::optflow::RLOFOpticalFlowParameter::create();
    return [denser](const cv::Mat &prev, const cv::Mat &next, cv::Mat &flow) {
      cv::optflow::calcOpticalFlowDenseRLOF(prev, next, flow, denser);
    };
  }
  case OpticalFlowMethod::DIS: {
    // create the instance once, capture it by shared_ptr in the lambda
    auto dis = cv::DISOpticalFlow::create(cv::DISOpticalFlow::PRESET_MEDIUM);
    return [dis](const cv::Mat &prev, const cv::Mat &next, cv::Mat &flow) {
      dis->calc(prev, next, flow);
    };
  }
  case OpticalFlowMethod::DualTVL1: {
    auto denser = cv::optflow::DualTVL1OpticalFlow::create();
    return [denser](const cv::Mat &prev, const cv::Mat &next, cv::Mat flow) {
      denser->calc(prev, next, flow);
    };
  }
  case OpticalFlowMethod::Farneback:
    return [](const cv::Mat &prev, const cv::Mat &next, cv::Mat &flow) {
      cv::calcOpticalFlowFarneback(prev, next, flow, 0.5, 5, 20, 5, 10, 1.1,
                                   cv::OPTFLOW_USE_INITIAL_FLOW);
    };
  }
  throw std::invalid_argument("Unhandled OpticalFlowMethod");
}
cv::Mat visualizeOpticalFlowArrows(cv::Mat &flow, cv::Mat &background,
                                   int step) {
  cv::Mat out;
  if (background.channels() == 1)
    cvtColor(background, out, cv::COLOR_GRAY2BGR);
  else
    out = background.clone();

  for (int y = 0; y < flow.rows; y += step) {
    for (int x = 0; x < flow.cols; x += step) {
      const cv::Point2f &fxy = flow.at<cv::Point2f>(y, x);
      cv::Point start(x, y);
      cv::Point end(cvRound(x + fxy.x), cvRound(y + fxy.y));
      cv::arrowedLine(out, start, end, cv::Scalar(0, 255, 0), 1, cv::LINE_AA, 0,
                      0.3);
      cv::circle(out, start, 1, cv::Scalar(0, 0, 255), -1);
    }
  }
  return out;
}
cv::Mat OptflowPostprocessing(cv::Mat &optflow, float threshold) {
  cv::Mat magnitude, angle, mask, masked_magn_1, masked_magn_2, output;
  cv::Mat flow_parts[2];
  split(optflow, flow_parts);
  cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
  cv::threshold(magnitude, mask, threshold, 0.0, 0);
  cv::bitwise_or(flow_parts[0], mask, masked_magn_1);
  cv::bitwise_or(flow_parts[1], mask, masked_magn_2);
  std::vector<cv::Mat> mats{flow_parts[0], flow_parts[1]};
  cv::merge(mats.data(), 2, output);
  return output;
}
cv::Mat visualizeOpticalFlow(cv::Mat &flow) {
  cv::Mat flow_parts[2];
  split(flow, flow_parts);
  cv::Mat magnitude, angle, magn_norm;
  cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
  // TODO: mask short vectors out with 0
  normalize(magnitude, magn_norm, 0.0f, 1.0f, cv::NORM_MINMAX);
  angle *= ((1.f / 360.f) * (180.f / 255.f));

  cv::Mat _hsv[3], hsv, hsv8, bgr;
  _hsv[0] = angle;
  _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
  _hsv[2] = magn_norm;
  merge(_hsv, 3, hsv);
  hsv.convertTo(hsv8, CV_8U, 255.0);
  cvtColor(hsv8, bgr, cv::COLOR_HSV2BGR);
  return bgr;
}
// INFO: create a stream, write the webcam feed intot the stream (potentially
// display it)
//
} // namespace OpenCVInput
