#include "lab2.hh"

void utcn::ip::Lab2::runLab() {
  int op;
  do {
    utcn::ip::Lab2::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testSplitRGB();
        break;
      case 2:
        testColorToGrayscale();
        break;
      case 3:
        testColorToBW();
        break;
      case 4:
        testRGBToHSV();
        break;
      case 5:
        testIsInside();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

std::array<cv::Mat, 3> getSplitMat(const cv::Mat& color_image) {
  int height = color_image.rows;
  int width = color_image.cols;

  cv::Mat_<uchar> blue(height, width, CV_8UC1);
  cv::Mat_<uchar> green(height, width, CV_8UC1);
  cv::Mat_<uchar> red(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    const auto* ptr = color_image.ptr<cv::Vec3b>(i);
    for (int j = 0; j < width; j++) {
      blue(i, j) = ptr[j][0];
      green(i, j) = ptr[j][1];
      red(i, j) = ptr[j][2];
    }
  }

  std::array<cv::Mat, 3> retArr{blue, green, red};

  return retArr;
}

void utcn::ip::Lab2::testSplitRGB() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<cv::Vec3b> src = imread(abs_image_path, cv::IMREAD_COLOR);

    const std::array<cv::Mat, 3> three_colors = getSplitMat(src);

    cv::imshow("Original", src);
    cv::imshow("Blue", three_colors.at(0));
    cv::imshow("Green", three_colors.at(1));
    cv::imshow("Red", three_colors.at(2));
    cv::waitKey();
  }
}

cv::Mat converToGrayscale(const cv::Mat& color_image) {
  int height = color_image.rows;
  int width = color_image.cols;

  cv::Mat_<uchar> gs(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    const auto* ptr = color_image.ptr<cv::Vec3b>(i);
    for (int j = 0; j < width; j++) {
      gs(i, j) = (ptr[j][0] + ptr[j][1] + ptr[j][2]) / 3;
    }
  }

  return gs;
}

void utcn::ip::Lab2::testColorToGrayscale() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<cv::Vec3b> src = imread(abs_image_path, cv::IMREAD_COLOR);

    cv::imshow("Color", src);
    cv::imshow("Grayscale", converToGrayscale(src));
    cv::waitKey();
  }
}

cv::Mat converToBW(const cv::Mat& grayscale_image, const int threshold) {
  int height = grayscale_image.rows;
  int width = grayscale_image.cols;

  cv::Mat_<uchar> bw(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    const auto* ptr = grayscale_image.ptr<uchar>(i);
    for (int j = 0; j < width; j++) {
      uchar val;
      if (ptr[j] < threshold) {
        val = 0;
      } else {
        val = 255;
      }
      bw(i, j) = val;
    }
  }

  return bw;
}

void utcn::ip::Lab2::testColorToBW() {
  int threshold;
  std::cout << "Please enter a threshold value (0 < x 255): ";
  std::cin >> threshold;

  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<uchar> src = imread(abs_image_path, cv::IMREAD_GRAYSCALE);

    cv::imshow("Grayscale", src);
    cv::imshow("B & W", converToBW(src, threshold));
    cv::waitKey();
  }
}

std::array<cv::Mat_<uchar>, 3> getHSV(const cv::Mat& color_image) {
  int height = color_image.rows;
  int width = color_image.cols;

  cv::Mat_<uchar> hue_norm(height, width, CV_8UC1);
  cv::Mat_<uchar> sat_norm(height, width, CV_8UC1);
  cv::Mat_<uchar> val_norm(height, width, CV_8UC1);

  const int NORMDIV = 255;

  for (int i = 0; i < height; i++) {
    const auto* ptr = color_image.ptr<cv::Vec3b>(i);
    for (int j = 0; j < width; j++) {
      const float b = (float)ptr[j][0] / NORMDIV;
      const float g = (float)ptr[j][1] / NORMDIV;
      const float r = (float)ptr[j][2] / NORMDIV;

      const float M = std::max(b, std::max(g, r));
      const float m = std::min(b, std::min(g, r));
      const float C = M - m;

      val_norm(i, j) = M * 255;

      sat_norm(i, j) = M != 0 ? (C / M) * 255 : 0;

      float H;

      if (C != 0) {
        if (M == r) H = 60 * (g - b) / C;
        if (M == g) H = 120 + 60 * (b - r) / C;
        if (M == b) H = 240 + 60 * (r - g) / C;
      } else {
        H = 0;
      }

      if (H < 0) {
        H = H + 360;
      }

      hue_norm(i, j) = H * 255 / 360;
    }
  }

  std::array<cv::Mat_<uchar>, 3> retArr{val_norm, sat_norm, hue_norm};

  return retArr;
}

void utcn::ip::Lab2::testRGBToHSV() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<cv::Vec3b> src = imread(abs_image_path, cv::IMREAD_COLOR);

    const std::array<cv::Mat_<uchar>, 3> hsv = getHSV(src);

    cv::imshow("Original", src);
    cv::imshow("Value", hsv.at(0));
    cv::imshow("Saturation", hsv.at(1));
    cv::imshow("Hue", hsv.at(2));
    cv::waitKey();
  }
}

bool isInside(const cv::Mat& img, int i, int j) {
  return i >= 0 && i <= img.rows && j >= 0 && j <= img.cols;
}

void utcn::ip::Lab2::testIsInside() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat src = imread(abs_image_path, cv::IMREAD_UNCHANGED);

    int i = 6;
    int j = 7;

    std::cout << "The pixel is" << (isInside(src, i, j) ? "" : " NOT")
              << " inside the image" << std::endl;
    std::cin.get();
    std::cin.get();
  }
}