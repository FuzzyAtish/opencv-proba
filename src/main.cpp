/* #include "file-util.hh"
#include <opencv2/highgui.hpp>

using fileutil = utcn::ip::FileUtil;

int main() {
   const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src;
    src = cv::imread(abs_image_path);
    cv::imshow("image", src);
    cv::waitKey();
  }
  return 0;
}
 */
#include "lab1.hh"

using Lab1 = utcn::ip::Lab1;

int main() {
  Lab1 lab1;
  lab1.runLab();
  return 0;
}