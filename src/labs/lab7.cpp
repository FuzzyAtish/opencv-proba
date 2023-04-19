#include "lab7.hh"

void utcn::ip::Lab7::runLab() {
  int op;
  do {
    utcn::ip::Lab7::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testBasicMorphologicalOperations();
        break;
      case 2:
        testBasicMorphologicalOperationsNTimes();
        break;
      case 3:
        testBoundaryExtractionAlgorithm();
        break;
      case 4:
        testRegionFillingAlgorithm();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

cv::Mat applyDilation(cv::Mat& img, int i, int j, int neighbors) {
  int di[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
  int dj[8] = {0, -1, 0, 1, -1, 1, -1, 1};
  for (int k = 0; k < neighbors; k++) {
    if (Lab2::isInside(img, i + di[k], j + dj[k])) {
      img.at<uchar>(i + di[k], j + dj[k]) = 0;
    }
  }
  return img;
}

cv::Mat dilation(cv::Mat& img, int neighbors) {
  int rows = img.rows;
  int columns = img.cols;
  cv::Mat new_image(rows, columns, CV_8UC1);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      new_image.at<uchar>(i, j) = 255;
    }
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (img.at<uchar>(i, j) == 0) {
        new_image.at<uchar>(i, j) = 0;
        new_image = applyDilation(new_image, i, j, neighbors);
      }
    }
  }
  cv::imshow("Original Image", img);
  cv::imshow("Dilated Image", new_image);
  cv::waitKey(0);
  return new_image;
}

bool onlyObject(cv::Mat& img, int i, int j, int neighbors) {
  bool ret_val = true;

  int di[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
  int dj[8] = {0, -1, 0, 1, -1, 1, -1, 1};

  for (int k = 0; k < neighbors && ret_val; k++) {
    if (Lab2::isInside(img, i + di[k], j + dj[k]) &&
        img.at<uchar>(i + di[k], j + dj[k]) == 255) {
      ret_val = false;
    }
  }

  return ret_val;
}

cv::Mat erosion(cv::Mat& img, int neighbors) {
  int rows = img.rows;
  int columns = img.cols;
  cv::Mat new_image(rows, columns, CV_8UC1);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      new_image.at<uchar>(i, j) = 255;
    }
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (img.at<uchar>(i, j) != 255) {
        if (onlyObject(img, i, j, neighbors)) {
          new_image.at<uchar>(i, j) = img.at<uchar>(i, j);
        }
      }
    }
  }
  cv::imshow("Original Image", img);
  cv::imshow("Eroded Image", new_image);
  cv::waitKey(0);
  return new_image;
}

cv::Mat opening(cv::Mat& img, int neighbors) {
  cv::Mat eroded = erosion(img, neighbors);
  cv::Mat dilated = dilation(eroded, neighbors);
  cv::imshow("Original Image", img);
  cv::imshow("Opening Image", dilated);
  cv::waitKey();
  return dilated;
}

cv::Mat closing(cv::Mat& img, int neighbors) {
  cv::Mat dilated = dilation(img, neighbors);
  cv::Mat eroded = erosion(dilated, neighbors);
  cv::imshow("original Image", img);
  cv::imshow("closing Image", eroded);
  cv::waitKey();
  return eroded;
}

cv::Mat boundaryExtraction(cv::Mat& img) {
  cv::Mat eroded = erosion(img, 8);
  cv::Mat new_image(img.rows, img.cols, CV_8UC1);
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      if (img.at<uchar>(i, j) != 255 && eroded.at<uchar>(i, j) == 255) {
        new_image.at<uchar>(i, j) = img.at<uchar>(i, j);
      } else {
        new_image.at<uchar>(i, j) = 255;
      }
    }
  }
  cv::imshow("Original Image", img);
  cv::imshow("Boundary Extracted Image", new_image);
  cv::waitKey();
  return new_image;
}

cv::Mat copyImg(cv::Mat& img) {
  cv::Mat copied(img.rows, img.cols, CV_8UC1);
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      copied.at<uchar>(i, j) = img.at<uchar>(i, j);
    }
  }
  return copied;
}

bool areEqual(cv::Mat& img1, cv::Mat& img2) {
  for (int i = 0; i < img1.rows; i++) {
    for (int j = 0; j < img1.cols; j++) {
      if (img1.at<uchar>(i, j) != img2.at<uchar>(i, j)) {
        return false;
      }
    }
  }
  return true;
}

cv::Mat regionFilling(cv::Mat& img) {
  cv::Mat complimentary(img.rows, img.cols, CV_8UC1);
  cv::Mat x(img.rows, img.cols, CV_8UC1);
  bool firstBorder = true;
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      complimentary.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);
      if (firstBorder && img.at<uchar>(i, j) == 255 &&
          Lab2::isInside(img, i - 1, j) && Lab2::isInside(img, i, j - 1) &&
          img.at<uchar>(i - 1, j) == 0 && img.at<uchar>(i, j - 1) == 0) {
        x.at<uchar>(i, j) = 0;
        firstBorder = false;
      } else {
        x.at<uchar>(i, j) = 255;
      }
    }
  }
  cv::Mat prevX(img.rows, img.cols, CV_8UC1);
  int h = 0;
  do {
    prevX = copyImg(x);
    x = dilation(x, 4);
    for (int i = 0; i < img.rows; i++) {
      for (int j = 0; j < img.cols; j++) {
        if (x.at<uchar>(i, j) == 0 && complimentary.at<uchar>(i, j) == 0) {
          x.at<uchar>(i, j) = 0;
        } else {
          x.at<uchar>(i, j) = 255;
        }
      }
    }
    h++;
  } while (!areEqual(x, prevX));
  cv::Mat result(img.rows, img.cols, CV_8UC1);
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      if (x.at<uchar>(i, j) == 0 || img.at<uchar>(i, j) == 0) {
        result.at<uchar>(i, j) = 0;
      } else {
        result.at<uchar>(i, j) = 255;
      }
    }
  }
  imshow("Original Image", img);
  imshow("Intermediary", x);
  imshow("Region Filled Image", result);
  cv::waitKey();
  return result;
}

cv::Mat applyDilationNTimes(cv::Mat& img, int n) {
  cv::Mat dilated(img.rows, img.cols, CV_8UC1);
  dilated = copyImg(img);
  for (int i = 0; i < n; i++) {
    dilated = dilation(dilated, 4);
  }
  imshow("Original Image", img);
  imshow("Dilated Image", dilated);
  cv::waitKey();
  return img;
}

cv::Mat applyErosionNTimes(cv::Mat& img, int n) {
  cv::Mat eroded(img.rows, img.cols, CV_8UC1);
  eroded = copyImg(img);
  for (int i = 0; i < n; i++) {
    eroded = erosion(eroded, 4);
  }
  imshow("original Image", img);
  imshow("eroded Image", eroded);
  cv::waitKey();
  return img;
}

void utcn::ip::Lab7::testBasicMorphologicalOperations() {}

void utcn::ip::Lab7::testBasicMorphologicalOperationsNTimes() {}

void utcn::ip::Lab7::testBoundaryExtractionAlgorithm() {}

void utcn::ip::Lab7::testRegionFillingAlgorithm() {}
