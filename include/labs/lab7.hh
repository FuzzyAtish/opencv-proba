#ifndef LAB7_HH
#define LAB7_HH

#include "lab.hh"
#include "lab2.hh"

using Lab2 = utcn::ip::Lab2;

namespace utcn::ip {
class Lab7 : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {1, "Test basic morphological operations"},
      {2, "Test basic morphological operations n times"},
      {3, "Test boundary extraction algorithm"},
      {4, "Test region filling algorithm"}};

  static void testBasicMorphologicalOperations();
  static void testBasicMorphologicalOperationsNTimes();
  static void testBoundaryExtractionAlgorithm();
  static void testRegionFillingAlgorithm();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB7_HH
