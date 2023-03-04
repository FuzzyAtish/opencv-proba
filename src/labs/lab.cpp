#include "lab.hh"

void utcn::ip::Lab::printMenu(const std::map<int, std::string>& entries) {
  system("clear || cls");
  cv::destroyAllWindows();
  std::cout << "Menu:" << std::endl;
  for (const auto& [key, value] : entries)
    std::cout << key << ((key < 10) ? "  " : " ") << "- " << value << std::endl;
  std::cout << " 0 - Exit" << std::endl << std::endl;
  std::cout << "Option: " << std::endl;
}