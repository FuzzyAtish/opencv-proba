#include "lab5.hh"

std::default_random_engine gen;
std::uniform_int_distribution<int> d(0, 255);

void utcn::ip::Lab5::runLab() {
  int op;
  do {
    utcn::ip::Lab5::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testBFS();
        break;
      case 2:
        testRandomColorEachLabel();
        break;
      case 3:
        testTwoPass();
        break;
      case 4:
        testPauseAfterEachStep();
        break;
      case 5:
        testDFS();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

std::vector<cv::Point> computeN8(const cv::Mat_<uchar>& img,
                                 const cv::Point& p) {
  int di[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int dj[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
  std::vector<cv::Point> neighbors;
  for (int k = 0; k < 8; k++)
    if (Lab2::isInside(img, p.x, p.y))
      neighbors.emplace_back(p.x + di[k], p.y + dj[k]);
  return neighbors;
}

std::vector<cv::Point> computeN4(const cv::Mat_<uchar>& img,
                                 const cv::Point& p) {
  int di[4] = {-1, 0, 1, 0};
  int dj[4] = {0, -1, 0, 1};
  std::vector<cv::Point> neighbors;
  for (int k = 0; k < 4; k++)
    if (Lab2::isInside(img, p.x, p.y))
      neighbors.emplace_back(p.x + di[k], p.y + dj[k]);
  return neighbors;
}

std::vector<cv::Point> computeNp(const cv::Mat_<uchar>& img,
                                 const cv::Point& p) {
  int di[4] = {0, -1, -1, -1};
  int dj[4] = {-1, -1, 0, 1};
  std::vector<cv::Point> neighbors;
  for (int k = 0; k < 4; k++)
    if (Lab2::isInside(img, p.x, p.y))
      neighbors.emplace_back(p.x + di[k], p.y + dj[k]);
  return neighbors;
}

cv::Mat_<uchar> calcBFS(const cv::Mat_<uchar>& img) {
  int height = img.rows;
  int width = img.cols;
  int label = 0;
  cv::Mat_<uchar> labels = cv::Mat::zeros(height, width, CV_8UC1);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (img(i, j) == 0 && labels(i, j) == 0) {
        label++;
        std::queue<cv::Point> Q;
        labels(i, j) = label;
        Q.push(cv::Point(i, j));
        while (!Q.empty()) {
          const cv::Point& q = Q.front();
          Q.pop();
          for (const cv::Point& p : computeN8(img, q)) {
            if (img(p.x, p.y) == 0 && labels(p.x, p.y) == 0) {
              labels(p.x, p.y) = label;
              Q.push(p);
            }
          }
        }
      }
    }
  }
  return labels;
}

void utcn::ip::Lab5::testBFS() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<uchar> img =
        cv::imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    const cv::Mat_<uchar> labels = calcBFS(img);
    std::cout << "Labels: " << std::endl << labels << std::endl;
  }
}

cv::Mat_<cv::Vec3b> createColoredImage(const cv::Mat_<uchar>& labels) {
  int height = labels.rows;
  int width = labels.cols;
  cv::Mat_<cv::Vec3b> colored_image = cv::Mat::zeros(height, width, CV_8UC3);

  int max_label = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (labels(i, j) > max_label) max_label = labels(i, j);
    }
  }

  std::vector<cv::Vec3b> colors;

  colors.emplace_back(255, 255, 255);

  for (int i = 0; i < max_label; i++) {
    const cv::Vec3b color(d(gen), d(gen), d(gen));
    colors.push_back(color);
  }

  for (int i = 0; i < labels.rows; i++) {
    for (int j = 0; j < labels.cols; j++) {
      colored_image(i, j) = colors.at(labels(i, j));
    }
  }

  return colored_image;
}

void utcn::ip::Lab5::testRandomColorEachLabel() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    const cv::Mat_<uchar> img =
        cv::imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    const cv::Mat_<uchar> labels = calcBFS(img);
    const cv::Mat_<cv::Vec3b> colored_image = createColoredImage(labels);
    cv::imshow("Original image", img);
    cv::imshow("Colored image", colored_image);
    cv::waitKey();
  }
}

void utcn::ip::Lab5::testTwoPass() {}

void utcn::ip::Lab5::testPauseAfterEachStep() {}

void utcn::ip::Lab5::testDFS() {}