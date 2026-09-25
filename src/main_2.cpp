#include "../include/PoseEstimator.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char **argv) {
  PoseEstimator pose_estimator("pose_landmark_full.onnx", "GPU");

  cv::VideoCapture camera(0);
  if (!camera.isOpened()) {
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  cv::Mat frame;
  while (true) {
    camera >> frame;

    std::vector<float> pose_data = pose_estimator.infer(frame);

    int keyboard = cv::waitKey(30);
    if (keyboard == 'q' || keyboard == 27)
      break;
  }
  return 0;
}
