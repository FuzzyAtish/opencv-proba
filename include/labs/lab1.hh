#ifndef LAB1_HH
#define LAB1_HH

#include <chrono>
#include <filesystem>

#include "lab.hh"

namespace utcn::ip {
class Lab1 : public Lab {
  static inline std::map<int, std::string> LAB1_MENU = {
      {1, "Open image"},
      {2, "Open BMP images from folder"},
      {3, "Image negative"},
      {4, "Image negative (fast)"},
      {5, "BGR->Gray"},
      {6, "BGR->Gray (fast, save result to disk) "},
      {7, "BGR->HSV"},
      {8, "Resize image"},
      {9, "Canny edge detection"},
      {10, "Edges in a video sequence"},
      {11, "Snap frame from live video"},
      {12, "Mouse callback demo"},
      {13, "Change the gray levels of an image by an additive factor"},
      {14, "Change the gray levels of an image by a multiplicative factor"},
      {15, "Draw a square with four different colors"},
      {16, "3x3 float matrix, determine its inverse"},
  };

  static inline uchar ADDITIVE_FACTOR = 54;

  static inline uchar MULTIPLICATIVE_FACTOR = 3;

  static inline cv::Vec3b WHITE{255, 255, 255};
  static inline cv::Vec3b RED{0, 0, 255};
  static inline cv::Vec3b GREEN{0, 255, 0};
  static inline cv::Vec3b YELLOW{0, 255, 255};
  static inline float MATRIX_VALS[9] = {2, 3, 1, 3, 4, 1, 3, 7, 2};
  static inline cv::Mat MATRIX3X3{3, 3, CV_32FC1, MATRIX_VALS};

  static void testOpenImage();

  static void testOpenImagesFld();

  static void testNegativeImage();

  static void testNegativeImageFast();

  static void testColor2Gray();

  static void testImageOpenAndSave();

  static void testBGR2HSV();

  static void testResize();

  static void testCanny();

  static void testVideoSequence();

  static void testSnap();

  static void myCallBackFunc(int event, int x, int y, int flags, void* param);

  static void testMouseClick();

  static cv::Mat changeByFactor(const cv::Mat& orig_pic, bool isAdditive,
                                uchar factor);

  static void testChangeGrayLevelsAdditive();

  static void testChangeGrayLevelsMultiplicative();

  static void testDrawFourSquare();

  static void testPrintInverseOfMatrix();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB1_HH