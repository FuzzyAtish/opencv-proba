#include "lab1.hh"

void utcn::ip::Lab1::runLab() {
  int op;
  do {
    utcn::ip::Lab1::printMenu(LAB1_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testOpenImage();
        break;
      case 2:
        testOpenImagesFld();
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

void utcn::ip::Lab1::testOpenImage() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src;
    src = cv::imread(abs_image_path);
    cv::imshow("image", src);
    cv::waitKey();
  }
}

// Not recursive
void utcn::ip::Lab1::testOpenImagesFld() {
  const std::string dir_abs_path = fileutil::getDirectoryAbsPath();
  if (!dir_abs_path.empty()) {
    const fs::path path_to_traverse(dir_abs_path);
    if (fs::exists(path_to_traverse) && fs::is_directory(path_to_traverse)) {
      for (const auto &entry : fs::directory_iterator(path_to_traverse)) {
        auto abs_file_path = entry.path().string();
        cv::Mat src;
        src = cv::imread(abs_file_path);
        cv::imshow(entry.path().filename().string(), src);
      }
      cv::waitKey();
    }
  }
}