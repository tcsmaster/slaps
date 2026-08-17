#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>

cv::Mat visualizeOpticalFlowArrows(const cv::Mat &flow,
                                   const cv::Mat &background, int step = 16) {
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

// Encodes a dense optical flow field as a BGR image: hue maps direction,
// value maps magnitude (normalized per-frame).
cv::Mat visualizeOpticalFlow(const cv::Mat &flow, float threshold) {
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

  while (true) {
    cv::Mat frame2, next;
    capture >> frame2;
    if (frame2.empty())
      break;
    cvtColor(frame2, next, cv::COLOR_BGR2GRAY);

    cv::Mat flow(old_gray.size(), CV_32FC2);
    cv::optflow::calcOpticalFlowSparseToDense(old_gray, next, flow, 8, 128,
                                              0.05f, false);

    cv::Mat bgr = visualizeOpticalFlowArrows(flow, frame2);
    cv::Mat bgr_2 = visualizeOpticalFlow(flow);
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
