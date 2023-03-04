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
        break;
      case 3:
        testNegativeImage();
        break;
      case 4:
        testNegativeImageFast();
        break;
      case 5:
        testColor2Gray();
        break;
      case 6:
        testImageOpenAndSave();
        break;
      case 7:
        testBGR2HSV();
        break;
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
  const auto abs_file_paths = fileutil::getAllFilesInDirectory();
  for (const auto &abs_file_path : abs_file_paths) {
    cv::Mat src;
    src = cv::imread(abs_file_path);
    const std::filesystem::path path = abs_file_path;
    cv::imshow(path.filename().string(), src);
  }
  cv::waitKey();
}

void utcn::ip::Lab1::testNegativeImage() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src = imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    cv::Mat dst = cv::Mat(height, width, CV_8UC1);

    auto t1 = std::chrono::high_resolution_clock::now();

    // Accessing individual pixels in an 8 bits/pixel image
    // Inefficient way -> slow
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        uchar val = src.at<uchar>(i, j);
        uchar neg = 255 - val;
        dst.at<uchar>(i, j) = neg;
      }
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    // Compute the time difference [ms]
    std::cout << "It took "
              << std::chrono::duration<double, std::milli>(t2 - t1)
              << std::endl;

    cv::imshow("input image", src);
    cv::imshow("negative image", dst);
    cv::waitKey();
  }
}

void utcn::ip::Lab1::testNegativeImageFast() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src = imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    cv::Mat dst = cv::Mat(height, width, CV_8UC1);

    auto t1 = std::chrono::high_resolution_clock::now();

    // The fastest approach of accessing the pixels -> using pointers
    uchar *lpSrc = src.data;
    uchar *lpDst = dst.data;
    int w = (int)src.step;  // no dword alignment is done !!!
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++) {
        uchar val = lpSrc[i * w + j];
        lpDst[i * w + j] = 255 - val;
      }

    auto t2 = std::chrono::high_resolution_clock::now();

    // Compute the time difference [ms]
    std::cout << "It took "
              << std::chrono::duration<double, std::milli>(t2 - t1)
              << std::endl;

    cv::imshow("input image", src);
    cv::imshow("negative image", dst);
    cv::waitKey();
  }
}

void utcn::ip::Lab1::testColor2Gray() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src = cv::imread(abs_image_path);

    int height = src.rows;
    int width = src.cols;

    cv::Mat dst = cv::Mat(height, width, CV_8UC1);

    // Accessing individual pixels in a RGB 24 bits/pixel image
    // Inefficient way -> slow
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        cv::Vec3b v3 = src.at<cv::Vec3b>(i, j);
        uchar b = v3[0];
        uchar g = v3[1];
        uchar r = v3[2];
        dst.at<uchar>(i, j) = (r + g + b) / 3;
      }
    }

    cv::imshow("input image", src);
    cv::imshow("gray image", dst);
    cv::waitKey();
  }
}

void utcn::ip::Lab1::testImageOpenAndSave() {
  cv::Mat src, dst;
  const std::string path_to_src = ASSETS_DIR "Images/Lena_24bits.bmp";
  src = cv::imread(path_to_src, cv::IMREAD_COLOR);  // Read the image

  if (!src.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }

  // Get the image resolution
  cv::Size src_size = cv::Size(src.cols, src.rows);

  // Display window
  const char *WIN_SRC = "Src";  // window for the source image
  cv::namedWindow(WIN_SRC, cv::WINDOW_AUTOSIZE);
  cv::moveWindow(WIN_SRC, 0, 0);

  const char *WIN_DST = "Dst";  // window for the destination (processed) image
  cv::namedWindow(WIN_DST, cv::WINDOW_AUTOSIZE);
  cv::moveWindow(WIN_DST, src_size.width + 10, 0);

  cvtColor(src, dst,
           cv::COLOR_BGR2GRAY);  // converts the source image to a grayscale one

  const std::string path_to_dst = ASSETS_DIR "Images/Lena_24bits_gray.bmp";
  imwrite(path_to_dst, dst);  // writes the destination to
                              // file

  cv::imshow(WIN_SRC, src);
  cv::imshow(WIN_DST, dst);

  cv::waitKey(0);
}

void utcn::ip::Lab1::testBGR2HSV() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src = cv::imread(abs_image_path);
    int height = src.rows;
    int width = src.cols;

    // HSV components
    cv::Mat H = cv::Mat(height, width, CV_8UC1);
    cv::Mat S = cv::Mat(height, width, CV_8UC1);
    cv::Mat V = cv::Mat(height, width, CV_8UC1);

    // Defining pointers to each matrix (8 bits/pixels) of the individual
    // components H, S, V
    uchar *lpH = H.data;
    uchar *lpS = S.data;
    uchar *lpV = V.data;

    cv::Mat hsvImg;
    cv::cvtColor(src, hsvImg, cv::COLOR_BGR2HSV);

    // Defining the pointer to the HSV image matrix (24 bits/pixel)
    uchar *hsvDataPtr = hsvImg.data;

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        int hi = i * width * 3 + j * 3;
        int gi = i * width + j;

        lpH[gi] = hsvDataPtr[hi] * 510 / 360;  // lpH = 0 .. 255
        lpS[gi] = hsvDataPtr[hi + 1];          // lpS = 0 .. 255
        lpV[gi] = hsvDataPtr[hi + 2];          // lpV = 0 .. 255
      }
    }

    cv::imshow("input image", src);
    cv::imshow("H", H);
    cv::imshow("S", S);
    cv::imshow("V", V);

    cv::waitKey();
  }
}