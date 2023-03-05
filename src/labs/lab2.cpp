#include "lab2.hh"

void utcn::ip::Lab2::runLab() {
  int op;
  do {
    utcn::ip::Lab2::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testSplitRGB();
        break;
      case 2:
        testColorToGrayscale();
        break;
      case 3:
        testColorToBW();
        break;
      case 4:
        testRGBToHSV();
        break;
      case 5:
        testIsInside();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

void utcn::ip::Lab2::testSplitRGB() {
  // Implement code here
}

void utcn::ip::Lab2::testColorToGrayscale() {
  // Implement code here
}

void utcn::ip::Lab2::testColorToBW() {
  // Implement code here
}

void utcn::ip::Lab2::testRGBToHSV() {
  // Implement code here
}

void utcn::ip::Lab2::testIsInside() {
  // Implement code here
}