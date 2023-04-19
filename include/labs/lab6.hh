#ifndef LAB6_HH
#define LAB6_HH

#include "lab.hh"

namespace utcn::ip {
class Lab6 : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {1, "Implement the border tracing algorithm and draw the object contour on an image having a single object"},
      {2, "Write the algorithm that builds the chain code and derivative chain code for an object. Compute and display (command line or output text file) both codes (chain code and derivative chain code) for an image with a single object"},
      {3, "Reconstruct"}};

  static void testBorderTrace();
  static void testChainCode();
  static void testReconstruct();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB6_HH
