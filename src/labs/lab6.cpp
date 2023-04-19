#include "lab6.hh"

void utcn::ip::Lab6::runLab() {
  int op;
  do {
    utcn::ip::Lab6::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testBorderTrace();
        break;
      case 2:
        testChainCode();
        break;
      case 3:
        testReconstruct();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

cv::Point getFirstBlackPixel(const cv::Mat_<uchar>& img) {
  int height = img.rows;
  int width = img.cols;

  // Find the first black pixel
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (img.at<uchar>(i, j) == 0)
        return cv::Point(i, j);
}

std::vector<cv::Point> getBorderTrace(const cv::Mat_<uchar>& img, bool is4connected = true) {
  std::vector<cv::Point> border;

  int height = img.rows;
  int width = img.cols;

  // Step 1
  const cv::Point P0 = getFirstBlackPixel(img);
  border.push_back(P0);

  const int dir = is4connected ? 0 : 7;

  // Step 2


  return border;
}

void utcn::ip::Lab6::testBorderTrace() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<uchar> src = imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    const std::vector<cv::Point> border = getBorderTrace(src);
  }
}

void utcn::ip::Lab6::testChainCode() {

}

void utcn::ip::Lab6::testReconstruct() {

}