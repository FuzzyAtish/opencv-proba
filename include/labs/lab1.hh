#ifndef LAB1_HH
#define LAB1_HH

#include <chrono>
#include <filesystem>

#include "file-util.hh"
#include "image-util.hh"
#include "lab.hh"

using fileutil = utcn::ip::FileUtil;
using imageutil = utcn::ip::ImageUtil;
namespace fs = std::filesystem;

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
  };

  static void testOpenImage();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB1_HH