#include "camera.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
// INFO: create a stream, write the webcam feed intot the stream (potentially
// display it)
//
int main(int argc, char **argv) {

  VideoCapture capture("http://172.31.240.1:8080");
  // VideoCapture capture(0);
  if (!capture.isOpened()) {
    // error in opening the video input
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  // Create some random colors
  std::vector<cv::Scalar> colors;
  cv::RNG rng;
  for (int i = 0; i < 100; i++) {
    int r = rng.uniform(0, 256);
    int g = rng.uniform(0, 256);
    int b = rng.uniform(0, 256);
    colors.emplace_back(r, g, b);
  }

  cv::Mat old_frame, old_gray;

  // Take first frame and find corners in it
  capture >> old_frame;
  cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);

  while (true) {
    Mat frame, frame_gray;

    capture >> frame;
    if (frame.empty())
      break;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

    // calculate optical flow
    std::vector<uchar> status;
    std::vector<float> err;
    TermCriteria criteria =
        TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
    Mat flow(old_gray.size(), CV_32FC2);
    calcOpticalFlowFarneback(old_gray, frame_gray, flow, 0.5, 3, 15, 3, 5, 1.2,
                             0);
    // Mat black = Mat::zeros(frame.size(), frame // visualization
    Mat flow_parts[2];
    split(flow, flow_parts);
    Mat magnitude, angle, magn_norm;
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));
    Mat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cvtColor(hsv8, bgr, COLOR_HSV2BGR);
    Mat img; // build hsv image
    add(frame, bgr, img);

    imshow("Frame", img);

    int keyboard = waitKey(30);
    if (keyboard == 'q' || keyboard == 27)
      break;

    // Now update the previous frame and previous points
    old_gray = frame_gray.clone();
  }
}
