#ifndef LAB4_HH
#define LAB4_HH

#include "lab.hh"

namespace utcn::ip {
class Lab4 : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {1,
       "For a specific object in a labeled image selected by a mouse click, "
       "compute the object’s\n"
       "area, center of mass, axis of elongation, perimeter, thinness ratio "
       "and aspect ratio.\n"
       "Display the results in the standard output"},
      {2,
       "In a separate image (source image clone):\n"
       "Draw the contour points of the selected object"},
      {3, "Display the center of mass of the selected object"},
      {4,
       "Display the axis of elongation of the selected object by using the "
       "line function from OpenCV"},
      {5,
       "Compute and display the projections of the selected object in a "
       "separate image (source image clone)"},
      {6,
       "Create a new processing function which takes as input a labeled image "
       "and keeps in the output image only the objects that:\n"
       "a. have their area < TH_area"},
      {7,
       "have a specific orientation phi, where phi_LOW < phi < phi_HIGH\n"
       "where TH_area, phi_LOW, phi_HIGH are given by the user"},
  };

  static void testDisplayResultsInStdOut();
  static void testDrawContour();
  static void testCenterOfMass();
  static void testAxisOfElongation();
  static void testProjections();
  static void testLessThanTH();
  static void testPhi();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // LAB4_HH
