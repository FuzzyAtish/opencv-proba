#include "lab3.hh"

#include <opencv2/core/hal/interface.h>

void utcn::ip::Lab3::runLab() {
  int op;
  do {
    utcn::ip::Lab3::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testComputeHistogramGrayscale();
        break;
      case 2:
        testPDF();
        break;
      case 3:
        testDisplayHistogram();
        break;
      case 4:
        testComputeHistogramBins();
        break;
      case 5:
        testMultiLevelThresholding();
        break;
      case 6:
        testFSDithering();
        break;
      case 7:
        testMultiLevelColor();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

void showHistogram(const std::string& name, const std::array<int, 256> hist,
                   const int hist_cols, const int hist_height) {
  cv::Mat imgHist(hist_height, hist_cols, CV_8UC3,
                  CV_RGB(255, 255, 255));  // constructs a white image

  // computes histogram maximum
  int max_hist = 0;
  for (int i = 0; i < hist_cols; i++)
    if (hist[i] > max_hist) max_hist = hist[i];
  double scale = 1.0;
  scale = (double)hist_height / max_hist;
  int baseline = hist_height - 1;

  for (int x = 0; x < hist_cols; x++) {
    cv::Point p1 = cv::Point(x, baseline);
    cv::Point p2 = cv::Point(x, baseline - cvRound(hist[x] * scale));
    line(imgHist, p1, p2,
         CV_RGB(255, 0, 255));  // histogram bins colored in magenta
  }

  cv::imshow(name, imgHist);
}

std::array<int, 256> getHistogram(std::string img_path) {
  cv::Mat image = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
  std::array<int, 256> histogram{};

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      int val = image.at<uchar>(i, j);
      histogram[val]++;
    }
  }

  return histogram;
}

void utcn::ip::Lab3::testComputeHistogramGrayscale() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    std::array<int, 256> histogram = getHistogram(abs_image_path);
    for (int i = 0; i < 256; i++) {
      printf("%d ", histogram[i]);
    }
  }
  char c;
  std::cin >> c;
}

std::array<float, 256> getPDF(std::string img_path) {
  std::array<int, 256> histogram = getHistogram(img_path);

  cv::Mat image = cv::imread(img_path, cv::IMREAD_GRAYSCALE);

  long M = image.rows * image.cols;

  std::array<float, 256> pdf{};
  for (int i = 0; i < 256; i++) {
    pdf[i] = (float)histogram[i] / M;
  }
  return pdf;
}

void utcn::ip::Lab3::testPDF() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    std::array<float, 256> pdf = getPDF(abs_image_path);
    for (const float val : pdf) {
      if (val != 0) {
        printf("%f\n", val);
      }
    }
  }
  char c;
  std::cin >> c;
}

void utcn::ip::Lab3::testDisplayHistogram() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat img = cv::imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    std::array<int, 256> histogram = getHistogram(abs_image_path);
    showHistogram(abs_image_path, histogram, 256, 100);
    cv::waitKey();
  }
}

void utcn::ip::Lab3::testComputeHistogramBins() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    std::array<int, 256> histogram{};

    cv::Mat image = cv::imread(abs_image_path, cv::IMREAD_GRAYSCALE);

    int bins = 100;

    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {
        int val = image.at<uchar>(i, j) * bins / 255;
        histogram[val]++;
      }
    }
    showHistogram("Hist with bins", histogram, 256, 100);
    cv::waitKey();
  }
}

int closestHistogramMaximum(int value, std::array<int, 256> maximums) {
  int closestMaximum = maximums[0];
  int currentDiff = abs(closestMaximum - value);
  for (int i = 1; i < 256; i++) {
    if (abs(maximums[i] - value) < currentDiff) {
      currentDiff = abs(maximums[i] - value);
      closestMaximum = maximums[i];
    }
  }
  return closestMaximum;
}

void utcn::ip::Lab3::testMultiLevelThresholding() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat image = cv::imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    cv::Mat newImage(image.rows, image.cols, CV_8UC1);
    std::array<float, 256> pdf = getPDF(abs_image_path);
    int WH = 5;         // window height
    int windowWidth = 2 * WH + 1;
    float TH = 0.0003;  // threshold

    std::array<int, 256> maximums{};
    int index = 1;
    for (int k = 0 + WH; k <= 255 - WH; k++) {
      // compute average of pdf values
      float v = 0;
      for (int i = k - WH; i <= k + WH; i++) {
        v += pdf[i];
      }
      v /= (2 * WH + 1);

      // find the max value in pdf in interval [k-WH, k+WH]
      float maxPDFInterval = 0;
      for (int i = k - WH; i < -k + WH; i++) {
        if (pdf[i] > maxPDFInterval) {
          maxPDFInterval = pdf[i];
        }
      }

      if (pdf[k] > v + TH && pdf[k] >= maxPDFInterval) {
        // k = histogram maximum
        maximums[index++] = k;
      }
    }
    maximums[0] = 0;
    maximums[255] = 255;

    // thresholding
    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {
        // find closest value
        newImage.at<uchar>(i, j) =
            closestHistogramMaximum(image.at<uchar>(i, j), maximums);
      }
    }
    cv::imshow("Original image", image);
    cv::imshow("Thresholded image", newImage);
    cv::waitKey();
  }
}

void utcn::ip::Lab3::testFSDithering() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat_<uchar> image = cv::imread(abs_image_path, cv::IMREAD_GRAYSCALE);
    cv::Mat_<uchar> new_image(image.rows, image.cols);

    std::array<float, 256> pdf = getPDF(abs_image_path);
    int WH = 5;         // window height
    int windowWidth = 2 * WH + 1;
    float TH = 0.0003;  // threshold

    std::array<int, 256> maximums{};
    int index = 1;
    for (int k = 0 + WH; k <= 255 - WH; k++) {
      // compute average of pdf values
      float v = 0;
      for (int i = k - WH; i <= k + WH; i++) {
        v += pdf[i];
      }
      v /= (2 * WH + 1);

      // find the max value in pdf in interval [k-WH, k+WH]
      float maxPDFInterval = 0;
      for (int i = k - WH; i < -k + WH; i++) {
        if (pdf[i] > maxPDFInterval) {
          maxPDFInterval = pdf[i];
        }
      }

      if (pdf[k] > v + TH && pdf[k] >= maxPDFInterval) {
        // k = histogram maximum
        maximums[index++] = k;
      }
    }
    maximums[0] = 0;
    maximums[255] = 255;

    for (int i = 0; i < new_image.rows; i++) {
      for (int j = 0; j < new_image.cols; j++) {
        uchar old_pixel = new_image(i, j);
        uchar new_pixel = closestHistogramMaximum(old_pixel, maximums);
        new_image(i, j) = new_pixel;
        int error = old_pixel - new_pixel;
        if (j + 1 < new_image.cols) {
          new_image(i, j + 1) += (uchar)(error * 7.0 / 16.0);
        }
        if (i + 1 < new_image.rows && j - 1 >= 0) {
          new_image(i + 1, j - 1) += (uchar)(error * 3.0 / 16.0);
        }
        if (i + 1 < new_image.rows) {
          new_image(i + 1, j) += (uchar)(error * 5.0 / 16.0);
        }
        if (i + 1 < new_image.rows && j + 1 < new_image.cols) {
          new_image(i + 1, j + 1) += (uchar)(error * 1.0 / 16.0);
        }
      }
    }

    cv::imshow("Original image", image);
    cv::imshow("FS Dithering", new_image);
    cv::waitKey();
  }
}

void utcn::ip::Lab3::testMultiLevelColor() {
  // Implement code here
}