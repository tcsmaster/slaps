#include "opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
int main(int argc, char **argv) {
  cv::VideoCapture capture(0);
  if (!capture.isOpened()) {
    // error in opening the video input
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  cv::Mat old_frame, old_gray;

  capture >> old_frame;
  cvtColor(old_frame, old_gray, cv::COLOR_BGR2GRAY);
  auto disopt = cv::DISOpticalFlow::create();
  while (true) {
    cv::Mat frame2, next;
    capture >> frame2;
    if (frame2.empty())
      break;
    cvtColor(frame2, next, cv::COLOR_BGR2GRAY);

    cv::Mat flow(old_gray.size(), CV_8UC1);
    // cv::optflow::calcOpticalFlowSparseToDense(old_gray, next, flow, 8, 128,
    //                                           0.05f, false);
    disopt->calc(old_gray, next, flow);
    cv::Mat bgr = Popencv::visualizeOpticalFlowArrows(flow, frame2);
    cv::Mat bgr_2 = Popencv::visualizeOpticalFlow(flow);
    cv::flip(bgr_2, bgr_2, 1);
    imshow("real", bgr);
    imshow("Frame", bgr_2);
    // TODO: flip the output of the optical flow horizontally to mirror it
    int keyboard = cv::waitKey(30);
    if (keyboard == 'q' || keyboard == 27)
      break;

    // Now update the previous frame and previous points
    old_gray = next.clone();
  }
}
