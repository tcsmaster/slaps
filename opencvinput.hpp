#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
namespace OpenCVInput {
enum class OpticalFlowMethod {
  DIS,
  Farneback,
  SparseToDense,
  DenseRLOF,
  DualTVL1
};
using FlowFunc = std::function<void(cv::Mat &, cv::Mat &, cv::Mat &)>;
FlowFunc CalculateOpticalFlow(OpticalFlowMethod method);
cv::Mat visualizeOpticalFlowArrows(cv::Mat &flow, cv::Mat &background,
                                   int step = 16);
cv::Mat OptflowPostprocessing(cv::Mat &optflow, float threshold);
cv::Mat visualizeOpticalFlow(cv::Mat &flow);
// INFO: create a stream, write the webcam feed intot the stream (potentially
// display it)
} // namespace OpenCVInput
