#ifndef LAB_HH
#define LAB_HH

#include <iostream>
#include <map>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

namespace utcn::ip {
class Lab {
 protected:
  virtual void runLab() = 0;
  static void printMenu(const std::map<int, std::string>& entries);

 public:
  virtual ~Lab() = default;
};
}  // namespace utcn::ip

#endif  // LAB_HH
