#include <iostream>

#include <opencv2/highgui.hpp>

#include "pfd.h"

int main(int, char **) {    
    const std::vector<std::string> selectedFile = pfd::open_file("Select a single file").result();
    std::cout << selectedFile.at(0) << std::endl;
    cv::Mat src;
    src = cv::imread(selectedFile.at(0));
    imshow("image", src);
    cv::waitKey();
}
