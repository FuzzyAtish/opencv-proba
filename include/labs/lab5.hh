#ifndef LAB5_HH
#define LAB5_HH

#include <random>

#include "lab.hh"
#include "lab2.hh"

using Lab2 = utcn::ip::Lab2;

namespace utcn::ip {
class Lab5 : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {1, "Breadth first traversal component labeling algorithm (Algorithm 1)"},
      {2, "Assigning a random color to each label"},
      {3, "Two-pass component labeling algorithm (Algorithm 2)"},
      {4, "Pause after each step"},
      {5, "Depth first traversal component labeling algorithm"}};

  static void testBFS();
  static void testRandomColorEachLabel();
  static void testTwoPass();
  static void testPauseAfterEachStep();
  static void testDFS();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB5_HH
