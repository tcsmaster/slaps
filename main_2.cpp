#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char **argv) {
  Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
  Ort::SessionOptions session_options;

  // Add OpenVINO EP, targeting GPU
  std::unordered_map<std::string, std::string> ov_options;
  ov_options["device_type"] = "GPU";
  session_options.AppendExecutionProvider("OpenVINO", ov_options);

  Ort::Session session(env, "pose_landmark_full.onnx", session_options);
  cv::VideoCapture camera(0);
  if (!camera.isOpened()) {
    // error in opening the video input
    std::cerr << "Unable to open file!" << std::endl;
    return 0;
  }

  cv::Mat old_frame, old_gray, flow;
  camera >> old_frame;
  cvtColor(old_frame, old_gray, cv::COLOR_BGR2GRAY);
  cv::Mat frame2, next;
  while (true) {
    // TODO: flip the output of the optical flow horizontally to mirror it
    camera >> frame2;
    cvtColor(frame2, next, cv::COLOR_BGR2GRAY);
    int keyboard = cv::waitKey(30);
    if (keyboard == 'q' || keyboard == 27)
      break;

    // Now update the previous frame and previous points
    old_gray = next.clone();
  }
  return 0;
}
