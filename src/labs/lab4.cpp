#include "lab4.hh"

void utcn::ip::Lab4::runLab() {
  int op;
  do {
    utcn::ip::Lab4::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testDisplayResultsInStdOut();
        break;
      case 2:
        testDrawContour();
        break;
      case 3:
        testCenterOfMass();
        break;
      case 4:
        testAxisOfElongation();
        break;
      case 5:
        testProjections();
        break;
      case 6:
        testLessThanTH();
        break;
      case 7:
        testPhi();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

int calcArea(const cv::Mat& img, int row, int col) {
  int area = 0;
  const cv::Vec3b curr_color = img.at<cv::Vec3b>(row, col);

  // Get height & width
  const int height = img.rows;
  const int width = img.cols;

  // Calculate area
  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++)
      if (img.at<cv::Vec3b>(r, c) == curr_color) area++;

  return area;
}

cv::Point calcCenterOfMass(const cv::Mat& img, int row, int col) {
  int rowsColl = 0;
  int colsColl = 0;

  int area = calcArea(img, row, col);
  const cv::Vec3b curr_color = img.at<cv::Vec3b>(row, col);

  // Get height & width
  const int height = img.rows;
  const int width = img.cols;

  // Get aggregates for CoM
  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++)
      if (img.at<cv::Vec3b>(r, c) == curr_color) {
        rowsColl += r;
        colsColl += c;
      }

  // Calculate center of mass
  int comX = colsColl / area;
  int comY = rowsColl / area;

  const cv::Point center(comX, comY);

  return center;
}

float calcAxisOfElongation(const cv::Mat& img, int row, int col) {
  cv::Point com = calcCenterOfMass(img, row, col);
  int ri_line = com.y;
  int ci_line = com.x;

  int sum_top;
  int sum_bottom_left;
  int sum_bottom_right;

  const cv::Vec3b curr_color = img.at<cv::Vec3b>(row, col);
  const int height = img.rows;
  const int width = img.cols;

  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++)
      if (img.at<cv::Vec3b>(r, c) == curr_color) {
        sum_top = (r - ri_line) * (c - ci_line);
        sum_bottom_left = (c - ci_line) * (c - ci_line);
        sum_bottom_right = (r - ri_line) * (r - ri_line);
      }

  if (sum_top || (sum_bottom_left - sum_bottom_right == 0)) return 0;

  float phi = atan2(2 * sum_top, sum_bottom_left - sum_bottom_right) / 2;

  return phi * 180 / CV_PI;
}

void onMouse(int event, int x, int y, int flags, void* param) {
  if (event == cv::EVENT_LBUTTONDOWN) {
    cv::Mat img = *((cv::Mat*)param);
    std::cout << "Pos(x,y): " << x << "," << y << std::endl
              << "Color(RGB): " << (int)img.at<cv::Vec3b>(y, x)[2] << ","
              << (int)img.at<cv::Vec3b>(y, x)[1] << ","
              << (int)img.at<cv::Vec3b>(y, x)[0] << std::endl;
    // AREA
    const int area = calcArea(*((cv::Mat*)param), y, x);
    std::cout << "The area of the image is: " << area << std::endl;

    // CENTER OF MASS
    const cv::Point com = calcCenterOfMass(*((cv::Mat*)param), y, x);
    std::cout << "Center of mass --> x: " << com.x << " y: " << com.y
              << std::endl;

    // AXIS OF ELONGATION
    const float aoe = calcAxisOfElongation(*((cv::Mat*)param), y, x);
    std::cout << "Axis of elongation --> " << aoe << "°" << std::endl;
  }
}

void utcn::ip::Lab4::testDisplayResultsInStdOut() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src = cv::imread(abs_image_path);
    const std::string WINDOW_TITLE = "Test Area of Object";

    // Create a window
    cv::namedWindow(WINDOW_TITLE, 1);

    // set the callback function for any mouse event
    cv::setMouseCallback(WINDOW_TITLE, onMouse, &src);

    // show the image
    cv::imshow(WINDOW_TITLE, src);

    // Wait until user press some key
    cv::waitKey();
  }
}

void utcn::ip::Lab4::testDrawContour() {
  // IMPLEMENT CODE HERE
}

void utcn::ip::Lab4::testCenterOfMass() {
  // IMPLEMENT CODE HERE
}

void utcn::ip::Lab4::testAxisOfElongation() {
  // IMPLEMENT CODE HERE
}

void utcn::ip::Lab4::testProjections() {
  // IMPLEMENT CODE HERE
}

void utcn::ip::Lab4::testLessThanTH() {
  // IMPLEMENT CODE HERE
}

void utcn::ip::Lab4::testPhi() {
  // IMPLEMENT CODE HERE
}
