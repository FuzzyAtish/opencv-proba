#ifndef LAB3_HH
#define LAB3_HH

#include "lab.hh"

namespace utcn::ip {
class Lab3 : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {1, "Compute the histogram for a given grayscale image"},
      {2, "Compute the PDF"},
      {3, "Display the computed histogram using the provided function"},
      {4, "Compute the histogram for a given number of bins m ≤ 256"},
      {5, "Implement the multilevel thresholding algorithm"},
      {6,
       "Enhance the multilevel thresholding algorithm using the "
       "Floyd-Steinberg dithering"},
      {7, "Perform multilevel thresholding on a color image"},
  };

  static void testComputeHistogramGrayscale();
  static void testPDF();
  static void testDisplayHistogram();
  static void testComputeHistogramBins();
  static void testMultiLevelThresholding();
  static void testFSDithering();
  static void testMultiLevelColor();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB3_HH