#include "camera.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
// INFO: create a stream, write the webcam feed intot the stream (potentially
// display it)
//
int main(int argc, char **argv) {
  VideoCapture capture(0);
  if (!capture.isOpened()) {
    // error in opening the video input
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  cv::Mat old_frame, old_gray;

  capture >> old_frame;
  cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);

  while (true) {
    Mat frame2, next;
    capture >> frame2;
    if (frame2.empty())
      break;
    cvtColor(frame2, next, COLOR_BGR2GRAY);

    Mat flow(old_gray.size(), CV_32FC2);
    calcOpticalFlowFarneback(old_gray, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

    // visualization
    Mat flow_parts[2];
    split(flow, flow_parts);
    Mat magnitude, angle, magn_norm;
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));

    // build hsv image
    Mat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cvtColor(hsv8, bgr, COLOR_HSV2BGR);
    // calculate optical flow
    imshow("Frame", bgr);

    int keyboard = waitKey(30);
    if (keyboard == 'q' || keyboard == 27)
      break;

    // Now update the previous frame and previous points
    old_gray = next.clone();
  }
}
